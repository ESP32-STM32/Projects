#include <TFT_eSPI.h> // Библиотека для работы с дисплеем

#include "battery.h" // Файл с массивом картинки
#include "battery_anim.h" // Файл с массивом картинки

TFT_eSPI tft = TFT_eSPI(); // Создание объекта для работы с дисплеем

int count = 0;           // Переменная для счетчика
static uint32_t tmrBAT;  // Переменная для таймера

void setup() {
  tft.init();                 // Инициализация объекта дисплея
  tft.setRotation(1);         // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_BLACK);  // Цвет фона экрана

  // Вправо x, Вниз y, имя масива, ширина, высота, цвет.
  tft.drawBitmap(0, 0, battery, 30, 30, TFT_RED); 
}

void loop() {
  // if (millis() - tmrBAT >= 300) {  // Условие которое срабатывает через каждые 300 миллисекунд
  //   tmrBAT = millis();
  //   tft.drawBitmap(0, 0, battery_anim[count], 30, 30, TFT_GREEN, TFT_BLACK);  // Батарейка
  //   count++;
  //   if (count > 6) count = 0;
  // }
}
