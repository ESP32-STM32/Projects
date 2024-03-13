#include <OneWire.h>            // Библиотека для общения с датчиком
#include <DallasTemperature.h>  // Библиотека для работы с датчиком
#define ONE_WIRE_BUS 15          // Пин к которому подключен датчик

OneWire oneWire(ONE_WIRE_BUS);        // Создание объекта для общения с датчиком
DallasTemperature sensors(&oneWire);  // Создание объекта для датчика чтобы запрашивать и получать температуру
DeviceAddress sensor1 = { 0x28, 0xFF, 0x64, 0xE, 0x76, 0x6E, 0xFA, 0xE1 };
DeviceAddress sensor2 = { 0x28, 0xFF, 0x64, 0xE, 0x77, 0x9A, 0xF3, 0x74 };
DeviceAddress sensor3 = { 0x28, 0xFF, 0x64, 0xE, 0x77, 0xD3, 0x9D, 0x27 };
static uint32_t tmr;  // Переменная для таймера

void setup(void) {
  Serial.begin(115200);  // Инициализация передачи данных по UART(USB)
  sensors.begin();       // Инициализация объекта датчика
}
void loop(void) {
  if (millis() - tmr >= 800) {
    tmr = millis();                 // Обнуление таймера
    sensors.requestTemperatures();  // Запрос температуры
    printTemp("Улица: ", sensor1);
    printTemp("Дом: ", sensor2);
    printTemp("Гараж: ", sensor3);
    Serial.println();
  }
}
void printTemp(String name, DeviceAddress& sensor) {  // Функция для печати температуры
  if (sensors.getTempC(sensor) != -127) {
    Serial.print(name);
    Serial.print(sensors.getTempC(sensor));
    Serial.print("°C ");
  } else {
    Serial.print("Error ");
  }
}
