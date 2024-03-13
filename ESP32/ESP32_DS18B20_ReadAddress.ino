#include <OneWire.h>            // Библиотека для общения с датчиком
#include <DallasTemperature.h>  // Библиотека для работы с датчиком
#define ONE_WIRE_BUS 15          // Пин к которому подключен датчик

OneWire oneWire(ONE_WIRE_BUS);        // Создание объекта для общения с датчиком
DallasTemperature sensors(&oneWire);  // Создание объекта для датчика чтобы запрашивать и получать температуру
DeviceAddress tempDeviceAddress;      // Переменная для хранения адресов датчиков
static uint32_t tmr;                  // Переменная для таймера
int numberOfDevices;                  // Переменная для хранения количества подключенных датчиков

void setup() {
  Serial.begin(115200);                        // Инициализация передачи данных по UART(USB)
  sensors.begin();                             // Инициализация объекта датчика
  numberOfDevices = sensors.getDeviceCount();  // Получение количества подключенных датчиков
}
void loop() {
  if (millis() - tmr >= 800) {
    tmr = millis();                 // Обнуление таймера
    sensors.requestTemperatures();  // Запрос температуры
    for (int i = 0; i < numberOfDevices; i++) {
      if (sensors.getAddress(tempDeviceAddress, i)) {
        float tempC = sensors.getTempC(tempDeviceAddress);  // Получение температуры
        printAddress(tempDeviceAddress);                    // Печать адреса
        Serial.print(" (");
        Serial.print(tempC);  // Печать температуры
        Serial.print("°C) ");
      }
    }
    Serial.println();
  }
}
void printAddress(DeviceAddress deviceAddress) {  // функция печати адреса устройства
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(deviceAddress[i], HEX);
    if (i != 7) Serial.print(", ");
  }
}
