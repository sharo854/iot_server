#include <Wire.h>

#define SHT31_ADDR 0x44

#define SINGLE_SHOT_HIGH_MSB 0x24
#define SINGLE_SHOT_HIGH_LSB 0x00


void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
unsigned int dac[6];
  unsigned int i, t ,h;
  float temperature, humidity;
  
  Serial.println("hello");
  Wire.beginTransmission(SHT31_ADDR);
  Wire.write(SINGLE_SHOT_HIGH_MSB);
  Wire.write(SINGLE_SHOT_HIGH_LSB);
  Wire.endTransmission();
  delay(1000);
  
  Wire.requestFrom(SHT31_ADDR, 6);
  for (i=0; i<6; i++){
      dac[i] = Wire.read();
  }
  Wire.endTransmission();

  t = (dac[0] << 8) | dac[1];
  temperature = (float)(t) * 175 / 65535.0 - 45.0;
  h = (dac[3] << 8) | dac[4];
  humidity = (float)(h) / 65535.0 * 100.0;

  Serial.println("温度："+String(temperature)+"℃");
  Serial.println("湿度："+String(humidity)+"％");
  
  delay(1000);
}
