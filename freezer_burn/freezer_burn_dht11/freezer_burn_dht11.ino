#include "chillhub.h"
#include "crc.h"
#include <EEPROM.h>
#include <string.h>
#include <stddef.h>
#include <dht11.h>


#define DHT11PIN 2
#define FIVE_MINUTE_TIMER_ID  0x70
#define MAX_UUID_LENGTH 48
#define EEPROM_SIZE 1024

// Define the port pin for the L LED
#define LedL 13
#define Analog 0

// Define this if no debug uart is available
#define DebugUart_UartPutString(...)

// Define cloud IDs for remote communication
dht11 DHT11;

enum E_CloudIDs {
  LedID = 0x91,
  AnalogID = 0x92,
  HumidityID = 0x93,
  TemperatureID= 0x94,
  LastID
};

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
const char defaultUUID[] = "41e1b18e-2d12-4306-9211-c1068bf7f76d";

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

void updateTemperatureAndHumidity(void) {
  static unsigned long oldTime = 0;
  unsigned long time = millis();
  
  if ((time - oldTime) > 10000) {
    oldTime = time;
    int chk = DHT11.read(DHT11PIN);
    ChillHub.updateCloudResourceU16(HumidityID, DHT11.humidity);
    ChillHub.updateCloudResourceU16(TemperatureID, DHT11.temperature);
  }  
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
  ChillHub.setup("chilldemo", eeprom.store.UUID);
  
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
  ChillHub.createCloudResourceU16("Humidity", HumidityID, 0, 0);
  ChillHub.createCloudResourceU16("Temperature", TemperatureID, 0, 0);
}

// This is the regular Arduino setup function.
void setup() {
  // Start serial port for communications with the chill hub
  Serial.begin(115200);
  delay(200);
  
  initializeEeprom();
  
  // Attempt to initialize with the chill hub
  deviceAnnounce();
}

// This is the normal Arduino run loop.
void loop() {
  ChillHub.loop();
  updateTemperatureAndHumidity();
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



