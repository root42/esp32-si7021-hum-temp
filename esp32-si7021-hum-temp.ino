#include <Wire.h>
 
// SI7021 I2C address is 0x40(64)
#define si7021Addr 0x40
 
void setup()
{
  unsigned int data;

  Serial.begin(9600);
  Wire.begin();
  
  // Reset Chip
  Wire.beginTransmission(si7021Addr);
  Wire.write(0xFE);
  Wire.endTransmission();
  delay(100);
  
  // Get Firmware revision
  Wire.beginTransmission(si7021Addr);
  Wire.write(0x84);
  Wire.write(0xB8);
  Wire.endTransmission();
  Wire.requestFrom(si7021Addr, 1);
  if(Wire.available() == 1)
  {
    data = Wire.read();
  }
  Serial.print("Firmware revision: ");
  switch( data ) {
    case 0xFF:
      Serial.print("1.0");
      break;
    case 0x20:
      Serial.print("2.0");
      break;
    default:
      Serial.print(data, HEX);
  }
  Serial.println();
}
 
void loop()
{
  unsigned int data[2];
 
  Wire.beginTransmission(si7021Addr);
  //Send humidity measurement command
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(100);
 
  // Request 2 bytes of data
  Wire.requestFrom(si7021Addr, 2);
  // Read 2 bytes of data to get humidity
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
 
  // Convert the data
  unsigned int temp = ((data[0] << 8) + data[1]);
  float humidity = ((125.0 * temp) / 65536.0) - 6;
 
  Wire.beginTransmission(si7021Addr);
  // Send temperature measurement command
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(100);
 
  // Request 2 bytes of data
  Wire.requestFrom(si7021Addr, 2);
 
  // Read 2 bytes of data for temperature
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
 
  // Convert the data
  temp  = ((data[0] << 8) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
 
  // Output data to serial monitor
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");
  delay(1000);
}
