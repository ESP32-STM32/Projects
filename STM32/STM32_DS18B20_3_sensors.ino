#include <microDS18B20.h>
#define DS_PIN PB9

uint8_t s1_addr[] = {0x28, 0xFF, 0x64, 0xE, 0x77, 0x9A, 0xF3, 0x74};
uint8_t s2_addr[] = {0x28, 0xFF, 0x64, 0xE, 0x76, 0x6E, 0xFA, 0xE1};
uint8_t s3_addr[] = {0x28, 0xFF, 0x64, 0xE, 0x77, 0xD3, 0x9D, 0x27};

MicroDS18B20<DS_PIN, s1_addr> sensor1;
MicroDS18B20<DS_PIN, s2_addr> sensor2;
MicroDS18B20<DS_PIN, s3_addr> sensor3;

static uint32_t tmr;

void setup() {
  Serial.begin(115200);//USB
}
void loop() {
  if (millis() - tmr >= 1000) {
    tmr = millis();
    DS18(sensor1, "Улица: ");
    DS18(sensor2, "Дом: ");
    DS18(sensor3, "Гараж: ");
    Serial.println();
  }
}
template <class B>
void DS18(B sensor, String names) {
  if (sensor.readTemp()) {
    Serial.print(names + String(sensor.getTemp()) + "°C");
    Serial.print(' ');
  } else {
    Serial.print("error");
  }
  sensor.requestTemp();
}
