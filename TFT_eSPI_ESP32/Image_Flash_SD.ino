//#include <SD.h>            // Библиотека для работы с SD-картой
//#include <TJpg_Decoder.h>  // Библиотека для открытия Jpeg файлов
#include <TFT_eSPI.h>  // Библиотека для работы с дисплеем

#include "stalker.h"  // Файл с массивом картинки

TFT_eSPI tft = TFT_eSPI();  // Создание объекта для работы с дисплеем

// bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
//   if (y >= tft.height()) return 0;
//   tft.pushImage(x, y, w, h, bitmap);
//   return 1;
// }

void setup() {
  // if (!SD.begin()) {
  //   return;
  // }

  tft.init();                 // Инициализация объекта дисплея
  tft.setRotation(1);         // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_BLACK);  // Цвет фона экрана
  tft.setSwapBytes(true);     // Меняем порядок байтов цвета при рендеринге

  // TJpgDec.setJpgScale(8);
  // TJpgDec.setCallback(tft_output);
  // TJpgDec.drawSdJpg(0, 0, "/Нива.jpg");

  tft.pushImage(0, 0, 48, 48, stalker);
}

void loop() {
}
