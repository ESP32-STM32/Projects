#include <TFT_eSPI.h>           // Библиотека для работы с дисплеем
#include <OneWire.h>            // Библиотека для общения с датчиком
#include <DallasTemperature.h>  // Библиотека для работы с датчиком

#define ONE_WIRE_BUS 13  // Пин к которому подключены датчики

#include "zBrushT24.h"  // Шрифт из флэш пямяти

OneWire oneWire(ONE_WIRE_BUS);        // Создание объекта для общения с датчиком
DallasTemperature sensors(&oneWire);  // Создание объекта для датчика чтобы запрашивать и получать температуру

TFT_eSPI tft = TFT_eSPI();             // Объект для работы с дисплеем
TFT_eSprite test = TFT_eSprite(&tft);  // Объект для слоя на дисплее

DeviceAddress sensor1 = { 0x28, 0xFF, 0x64, 0xE, 0x76, 0x6E, 0xFA, 0xE1 };
DeviceAddress sensor2 = { 0x28, 0xFF, 0x64, 0xE, 0x77, 0x9A, 0xF3, 0x74 };
DeviceAddress sensor3 = { 0x28, 0xFF, 0x64, 0xE, 0x77, 0xD3, 0x9D, 0x27 };
DeviceAddress sensor4 = { 0x28, 0x61, 0x64, 0xA, 0xBF, 0xB4, 0x2, 0x99 };

static uint32_t tmr;  // Переменная для таймера

void setup(void) {
  sensors.begin();                         // Инициализация объекта датчика
  tft.init();                              // Инициализация объекта дисплея
  tft.setRotation(1);                      // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_BLACK);               // Цвет фона экрана
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Цвет текста

  test.createSprite(70, 24);                // Создание слоя
  test.fillScreen(TFT_BLACK);               // Цвет фона слоя
  test.setTextColor(TFT_WHITE, TFT_BLACK);  // Цвет текста

  tft.loadFont(zBrushT24);  // Загрузка шрифта
  tft.setCursor(20, 30);
  tft.print("Дом");
  tft.setCursor(130, 30);
  tft.print("Гараж");
  tft.setCursor(20, 100);
  tft.print("Улица");
  tft.setCursor(130, 100);
  tft.print("Теплица");
  tft.unloadFont();  // Выгрузка шрифта
}
void loop(void) {
  if (millis() - tmr >= 800) {      // Условие которое срабатывает через каждые 800 миллисекунд
    tmr = millis();                 // Обнуление таймера
    sensors.requestTemperatures();  // Запрос температуры

    test.loadFont(zBrushT24);  // Загрузка шрифта
    printTemp(sensor1);
    test.pushSprite(20, 54, TFT_TRANSPARENT);  // Отрисовка слоя

    printTemp(sensor2);
    test.pushSprite(130, 54, TFT_TRANSPARENT);  // Отрисовка слоя

    printTemp(sensor3);
    test.pushSprite(20, 124, TFT_TRANSPARENT);  // Отрисовка слоя

    printTemp(sensor4);
    test.pushSprite(130, 124, TFT_TRANSPARENT);  // Отрисовка слоя
    test.unloadFont();                           // Выгрузка шрифта
  }
}
void printTemp(DeviceAddress& sensor) {  // Функция для печати температуры
  if (sensors.getTempC(sensor) != -127) {
    test.setCursor(0, 0);
    test.fillScreen(TFT_BLACK);
    test.print(sensors.getTempC(sensor));
    test.print("°C ");
  } else {
    test.setCursor(0, 0);
    test.fillScreen(TFT_BLACK);  // Заливка черным цветом фона слоя
    test.print("Нет");
  }
}
