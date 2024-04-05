#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(5);  // SPI

unsigned long tmr;

void setup() {
  Serial.begin(115200);
  bme.begin(0x76); // По умолчанию 0x77
}

void loop() {
  BME280();
}

void BME280() {
  if (millis() - tmr >= 2000) {
    tmr = millis();

    Serial.print("Температура = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Давление = ");
    Serial.print(bme.readPressure() / 100.0F * 0.750064);
    Serial.println(" мм");

    Serial.print("Влажность = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
  }
}
