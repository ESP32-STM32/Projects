#include <OneWire.h> // Библиотека для общения с датчиком
#include <DallasTemperature.h> // Библиотека для работы с датчиком
#define ONE_WIRE_BUS 15 // Пин к которому подключен датчик

OneWire oneWire(ONE_WIRE_BUS);        // Создание объекта для общения с датчиком
DallasTemperature sensors(&oneWire);  // Создание объекта для датчика чтобы запрашивать и получать температуру

static uint32_t tmr;  // Переменная для таймера

void setup(void) {
  Serial.begin(115200);  // Инициализация передачи данных по UART(USB)
  sensors.begin();       // Инициализация объекта датчика
}

void loop(void) {
  if (millis() - tmr >= 800) {
    tmr = millis();                            // Обнуление таймера
    sensors.requestTemperatures();             // Запрос температуры
    float tempC = sensors.getTempCByIndex(0);  // Получение температуры
    if (tempC != -127) {                       // Проверка показаний датчика
      Serial.print(tempC);                     // Печать температуры
      Serial.println("°C");
    } else {
      Serial.println("Error");
    }
  }
}
