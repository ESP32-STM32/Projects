#include <TFT_eSPI.h>  // Библиотека для работы с дисплеем

#include "music.h"

TFT_eSPI tft = TFT_eSPI();  // Создание объекта для работы с дисплеем

int count = 0;           // Переменная для счетчика
static uint32_t tmrBAT;  // Переменная для таймера

int count1 = 0;           // Переменная для счетчика
static uint32_t tmrBAT1;  // Переменная для таймера

void setup() {
  tft.init();                 // Инициализация объекта дисплея
  tft.setRotation(1);         // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_WHITE);  // Цвет фона экрана
  tft.setSwapBytes(true);     // Меняем порядок байтов цвета при рендеринге
}

void loop() {
  if (millis() - tmrBAT >= 40) {  // Условие которое срабатывает через каждые 300 миллисекунд
    tmrBAT = millis();
    tft.pushImage(0, 0, 50, 50, battery[count]);
    count++;
    if (count > 27) count = 0;
  }
}
