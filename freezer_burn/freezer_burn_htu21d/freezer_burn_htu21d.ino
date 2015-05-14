
#include <EEPROM.h>
#include <string.h>
#include <stddef.h>
#include <Wire.h>
#include <TimerOne.h>

#include "chillhub.h"
#include "crc.h"
#include "HTU21D.h"

#define MAX_UUID_LENGTH 48
#define EEPROM_SIZE 1024

// Define this if no debug uart is available
#define DebugUart_UartPutString(...)

// Variables
const int startPin = 2;
const int led = LED_BUILTIN;
const uint32_t uSecondTimerValue = 50000;
const uint16_t tenSecondsOfCycles = 10000000/uSecondTimerValue;
int ledState = LOW;

uint8_t button_state = LOW; // use volatile for shared variables
uint8_t last_button_state = LOW;
uint8_t logic_state = LOW;
uint8_t last_logic_state = LOW;

// These are shared with the interupt code
volatile uint8_t read_temp_humidity = 0;
volatile uint8_t run_experiment = LOW;

enum E_CloudIDs {
  ExperimentStateID = 0x91,
  remoteUpdateExperimentID = 0x92,
  HumidityID = 0x93,
  TemperatureID= 0x94,
  LastID
};

//Create an instance of the humidity sensor object
HTU21D myHumidity;

// Define the EEPROM data structure.
typedef struct Store {
  char UUID[MAX_UUID_LENGTH+1];
  unsigned int crc; 
} Store;

// A union to make reading and writing the EEPROM convenient.
typedef union Eeprom {
    Store store;
    unsigned char bytes[sizeof(Store)];
} Eeprom;

// The RAM copy of the EEPROM.
Eeprom eeprom;

// A default UUID to use if none has been assigned.
// Each device needs it's own UUID.
const char defaultUUID[] = "644a5f55-1d08-43fa-874a-404ce401430f";

// register the name (type) of this device with the chillhub
// syntax is ChillHub.setup(device type, UUID);
chInterface ChillHub;

//
// Function prototypes
//
static void saveEeprom(void);
static void initializeEeprom(void);
void keepaliveCallback(uint8_t dummy);
void setDeviceUUID(char *pUUID);
static void remoteSetExperimentState(uint8_t state);

void updateTemperatureAndHumidity(uint8_t experiment_status) {
    float humidity_f, temperature_f;
    int16_t humidity, temperature;
    humidity_f = myHumidity.readHumidity();
    temperature_f = myHumidity.readTemperature();
    humidity = humidity_f;
    temperature = temperature_f;
    ChillHub.updateCloudResourceI16(HumidityID, humidity);
    ChillHub.updateCloudResourceI16(TemperatureID, temperature);
    ChillHub.updateCloudResourceU16(ExperimentStateID,  (uint16_t)experiment_status);
}

// This function gets called when you plug the Arduino into the chillhub.
// It registers the Arduino with the chill hub.
// It registers resources in the cloud.
// It registers listeners for Arduino resources which can be controlled
// via the cloud.
void deviceAnnounce() { 
  // Each device has a "type name" and a UUID.
  // A type name could be something like "toaster" or "light bulb"
  // Each device must has a unique version 4 UUID.  See
  // http://en.wikipedia.org/wiki/Universally_unique_identifier#Version_4_.28random.29
  // for details.
  ChillHub.setup("freezer_burn_htu21d", eeprom.store.UUID);
  
  // add a listener for device ID request type
  // Device ID is a request from the chill hub for the Arduino to register itself.
  ChillHub.subscribe(deviceIdRequestType, (chillhubCallbackFunction)deviceAnnounce);

  // add a listener for keepalive from chillhub
  // The chillhub sends this periodcally.
  ChillHub.subscribe(keepAliveType, (chillhubCallbackFunction)keepaliveCallback);

  // add a listener for setting the UUID of the device
  // The UUID is set via the USB port and the set-device-uuid.js script as part of
  // chillhub-firmware.
  // No cloud listener is required for this.
  ChillHub.subscribe(setDeviceUUIDType, (chillhubCallbackFunction)setDeviceUUID);

  ChillHub.addCloudListener(remoteUpdateExperimentID, (chillhubCallbackFunction)remoteSetExperimentState);

  ChillHub.createCloudResourceI16("Humidity", HumidityID, 0, 0);
  ChillHub.createCloudResourceI16("Temperature", TemperatureID, 0, 0);
  ChillHub.createCloudResourceU16("ExperimentState", ExperimentStateID, 0, 0);
  ChillHub.createCloudResourceU16("StartStopExperiment", remoteUpdateExperimentID, 1, 0);
}

static void remoteSetExperimentState(uint8_t state)
{
  static uint8_t first_time=1;
  if (first_time == 0){
     noInterrupts();
     run_experiment = !run_experiment;
     interrupts();
  } else {
    first_time = 0;
  }
}
void setExperimentState(void)
{
  // 1 second Sensor Read Timer
  read_temp_humidity++; 
  if (read_temp_humidity >= tenSecondsOfCycles){
    read_temp_humidity = 0;
  }
  
  button_state = !digitalRead(startPin);

  // Button Debounce
  if (last_button_state == button_state){
    logic_state = button_state;
  }
  
  // Falling Edge Trigger
  if(logic_state == LOW && last_logic_state  == HIGH){
    run_experiment = !run_experiment;
  }

  //  Blink LED
  if ( run_experiment == HIGH){
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  } else {
    ledState = LOW;
  }
  
  digitalWrite(led, ledState);
  last_logic_state = logic_state;
  last_button_state = button_state;
}

void setup()
{
  // Start serial port for communications with the chill hub
  Serial.begin(115200);
  delay(200);
  
  initializeEeprom();
  
  // Attempt to initialize with the chill hub
  deviceAnnounce();
  
  pinMode(startPin, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Timer1.initialize(uSecondTimerValue);
  Timer1.attachInterrupt(setExperimentState);
  myHumidity.begin();
}



void loop()
{
  uint8_t temp_run_experiment =     LOW;
  uint8_t temp_read_temp_humidity = LOW;

  ChillHub.loop();

  noInterrupts();
  temp_run_experiment = run_experiment;
  temp_read_temp_humidity = !read_temp_humidity;
  interrupts();

  if (temp_read_temp_humidity){
    updateTemperatureAndHumidity(temp_run_experiment);
  }
}

// Save the data to EEPROM.
static void saveEeprom(void) {
  int i;
  uint16_t crc = crc_init();
  unsigned char b;

  // Write the data, calculating the CRC as we go.
  for(i=0; i<offsetof(Store, crc); i++) {
    b = eeprom.bytes[i];
    EEPROM.write(i, b);
    crc = crc_update(crc, &b, 1);
  }

  // Finish calculating the CRC.
  crc = crc_finalize(crc);

  // Save the CRC to the RAM copy
  eeprom.store.crc = crc;
  
  // Write the CRC to the EEPROM.
  EEPROM.write(offsetof(Store, crc), eeprom.bytes[offsetof(Store, crc)]);
  EEPROM.write(offsetof(Store, crc)+1, eeprom.bytes[offsetof(Store, crc)+1]);
}

// Initialize the EEPROM RAM copy
// This will also initialize the EEPROM if it has not been initialized previously.
static void initializeEeprom(void) {
  int i;
  uint16_t crc = crc_init();
  unsigned char b;
  
  // Read the data from the EEPROM, calculate the CRC as we go.
  for(i=0; i<offsetof(Store, crc); i++) {
    b = EEPROM.read(i);
    eeprom.bytes[i] = b;
    crc = crc_update(crc, &b, 1);
  }
  
  // Get the stored CRC.
  eeprom.bytes[offsetof(Store, crc)] = EEPROM.read(offsetof(Store, crc));
  eeprom.bytes[offsetof(Store, crc)+1] = EEPROM.read(offsetof(Store, crc)+1);
  
  // Finish calculating the CRC.
  crc = crc_finalize(crc);
  
  // Compare the stored CRC with the calculated CRC.
  // If they are not equal, initialize the internal EEPROM.
  if (crc != eeprom.store.crc) {
    memcpy(eeprom.store.UUID, defaultUUID, sizeof(defaultUUID));
    saveEeprom();
  }
}

// This handles the keep alive message from the chillhub.
// This chillhub will send this message periodically.
void keepaliveCallback(uint8_t dummy) {
  (void)dummy;
}

void setDeviceUUID(char *pUUID) {
  uint8_t len = (uint8_t)pUUID[0];
  char *pStr = &pUUID[1];
  
  if (len <= MAX_UUID_LENGTH) {
    // add null terminator
    pStr[len] = 0;
    memcpy(eeprom.store.UUID, pStr, len+1);
    saveEeprom();
    DebugUart_UartPutString("New UUID written to device.\r\n");
  } else {
    DebugUart_UartPutString("Can't write UUID, it is too long.\r\n");
  }
}



