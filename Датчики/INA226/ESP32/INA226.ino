#include <Wire.h>
#include <INA226.h>

INA226 ina;

unsigned long tmr;
float calibrateINA = 0.1; // При уменьшении значения - показания ампер увеличиваются // 0.0619
String input_string;

void setup() {
  // pinMode(15, OUTPUT);
  // digitalWrite(15, HIGH);
  Serial.begin(115200);
  ina.begin(0x40);  // 0x45
  ina.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);
  ina.calibrate(calibrateINA);  

  // Узнать ток шунта 0.08мВ / 0.0619Rш = 1.29 A
  // Узнать ток шунта 0.08мВ / 0.1Rш = 0.8 A
  // Узнать ток шунта 0.08мВ / 0.08Rш = 1 A
  // Узнать ток шунта 0.08мВ / 0.05Rш = 1.6 A
  // Узнать ток шунта 0.08мВ / 0.04Rш = 2 A
  // Узнать ток шунта 0.08мВ / 0.025Rш = 3.2 A
  // Узнать ток шунта 0.08мВ / 0.02Rш = 4 A
  // Узнать ток шунта 0.08мВ / 0.01Rш = 8 A
  // Узнать ток шунта 0.08мВ / 0.008Rш = 10 A

  // Узнать сопрошивление шунта 0.08мВ / 0.8A = 0.1Rш
  // Узнать сопрошивление шунта 0.08мВ / 1A = 0.08Rш

  // Узнать мощность 3.2A * 3.2A * 0.1Rш = 1,024Вт
  // Узнать мощность 3.2A * 3.2A * 0.05Rш = 0,512Вт
  // Узнать мощность 10 * 10 * 0.008Rш = 0,8Вт
  // Узнать мощность 2A * 2A * 0.1Rш = 0,4Вт
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '-') {
      Command();
      input_string = "";
    } else {
      input_string += c;
    }
  }
  INA226_read();
}
void INA226_read() {
  if (millis() - tmr >= 100) {
    tmr = millis();

    Serial.print(ina.readBusVoltage(), 5);
    Serial.println(" V");

    Serial.print(ina.readShuntCurrent(), 5);
    Serial.println(" A");

    Serial.print(ina.readBusPower(), 5);
    Serial.println(" W");
    Serial.println();

    Serial.print("Калибровка: ");
    Serial.println(calibrateINA);
  }
}
void Command() {
  if (input_string.startsWith("q") == true) {
      input_string.replace("q", "");
      calibrateINA = input_string.toFloat();
      ina.calibrate(calibrateINA);
  }
}
/*
  // Фильтрация
  INA226_AVERAGES_1
  INA226_AVERAGES_4
  INA226_AVERAGES_16
  INA226_AVERAGES_64
  INA226_AVERAGES_128
  INA226_AVERAGES_256
  INA226_AVERAGES_512
  INA226_AVERAGES_1024

  INA226_BUS_CONV_TIME_140US
  INA226_BUS_CONV_TIME_204US
  INA226_BUS_CONV_TIME_332US
  INA226_BUS_CONV_TIME_588US
  INA226_BUS_CONV_TIME_1100US
  INA226_BUS_CONV_TIME_2116US
  INA226_BUS_CONV_TIME_4156US
  INA226_BUS_CONV_TIME_8244US

  INA226_SHUNT_CONV_TIME_140US
  INA226_SHUNT_CONV_TIME_204US
  INA226_SHUNT_CONV_TIME_332US
  INA226_SHUNT_CONV_TIME_588US
  INA226_SHUNT_CONV_TIME_1100US
  INA226_SHUNT_CONV_TIME_2116US
  INA226_SHUNT_CONV_TIME_4156US
  INA226_SHUNT_CONV_TIME_8244US

  INA226_MODE_POWER_DOWN
  INA226_MODE_SHUNT_TRIG
  INA226_MODE_BUS_TRIG
  INA226_MODE_SHUNT_BUS_TRIG
  INA226_MODE_SHUNT_CONT
  INA226_MODE_BUS_CONT
  INA226_MODE_SHUNT_BUS_CONT
*/
