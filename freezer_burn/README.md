# Arduino Uno based Humidity Sensors
## DHT11 Low Quality Cheapo Sensor ~$4
This sensor does not have much precision but it is okay to get moderate readings. It can use the default ChillHub Arduino library because it really only has a range of 0C-100C and 20-80% Relative Humidity.

### Required Libraries
* [DHT11](http://playground.arduino.cc/Main/DHT11Lib)
* [ChillHub-Arduino](https://github.com/FirstBuild/ChillHubArduino)

## SHT15 High Precision Calibrated Sensor ~$40
This is a nice sensor but it requires modifications to the ChillHub Arduino library to support int16_t values.

### Required Libraries
* [SHT1x](https://github.com/practicalarduino/SHT1x)
* [Quelab/ChillHub-Arduino](https://github.com/Quelab/ChillHubArduino)

## HTU21D ~$14
### Required Libraries
* [HTU21D Breakout](https://github.com/sparkfun/HTU21D_Breakout/tree/master/Libraries/Arduino)
* [Quelab/ChillHub-Arduino](https://github.com/Quelab/ChillHubArduino)