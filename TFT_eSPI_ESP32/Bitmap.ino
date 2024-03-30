#include <TFT_eSPI.h> // Библиотека для работы с дисплеем

#include "auido.h" // Файл с массивом картинки

TFT_eSPI tft = TFT_eSPI(); // Создание объекта для работы с дисплеем

void setup() {
  tft.init();                 // Инициализация объекта дисплея
  tft.setRotation(1);         // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_BLACK);  // Цвет фона экрана

  // Вправо x, Вниз y, имя масива, ширина, высота, цвет.
  tft.drawBitmap(0, 0, audio, 50, 50, TFT_RED); 
}

void loop() {
}
