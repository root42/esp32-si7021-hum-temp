#include <Wire.h>

#define si7021 0x40

bool WireEnd() {
  unsigned char err;
  err = Wire.endTransmission();
  if( err ) {
    Serial.print("Error: ");
    Serial.println(err);
  }
  return err;
}

void setup() {
  bool err;
  unsigned int data;
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  // Reset chip
  Serial.println("Starting up SI7021 sensor...");
  Wire.beginTransmission(si7021);
    Wire.write(0xFE);
  WireEnd();
  delay(100);

  // Get serial
  Serial.print("Serial ID: ");
  Wire.beginTransmission(si7021);
    Wire.write(0xFA);
    Wire.write(0x0F);
  err = WireEnd();
  if( !err ) {
    Wire.requestFrom(si7021, 8);
    while(Wire.available()) {
      data = Wire.read();
      Serial.print(data,HEX);
      Serial.print(" ");
    }
    Wire.beginTransmission(si7021);
      Wire.write(0xFC);
      Wire.write(0xC9);
    err = WireEnd();
    if( !err ) {
      Wire.requestFrom(si7021, 6);
      while(Wire.available()) {
        data = Wire.read();
        Serial.print(data,HEX);
        Serial.print(" ");
      }
    }
    Serial.println();
  }
  
  // Get Firmware revision
  Serial.print("Firmware revision: ");
  Wire.beginTransmission(si7021);
    Wire.write(0x84);
    Wire.write(0xB8);
  err = WireEnd();
  if( !err ) {
    Wire.requestFrom(si7021, 1);
    if(Wire.available() == 1)
    {
      data = Wire.read();
      Serial.print(data, HEX);
      Serial.println();
    } else {
      Serial.println("Error reading from chip");
    }
  }
}

void loop() {
  unsigned int data[2];

  // Humidity measurement
  Wire.beginTransmission(si7021);
    Wire.write(0xF5);
  WireEnd();
  delay(100);
  Wire.requestFrom(si7021, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  unsigned int temp = ((data[0] << 8) + data[1]);
  float humidity = ((125.0 * temp) / 65536.0) - 6;
 
  // Temperature measurement
  Wire.beginTransmission(si7021);
    Wire.write(0xF3);
  WireEnd();
  delay(100);
  Wire.requestFrom(si7021, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  temp  = ((data[0] << 8) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
 
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");
  delay(1000);
}
