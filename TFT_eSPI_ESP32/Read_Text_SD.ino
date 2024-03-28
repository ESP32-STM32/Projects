#include <SD.h>
#include <TFT_eSPI.h>

#include "zCyrillicOld20.h"

#define FONT zCyrillicOld20

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite test = TFT_eSprite(&tft);

String input_string;
String buffer = "";  //Текст с сд-карты
int bufSize = 0;
int i = 0;

void setup() {
  Serial.begin(115200);
  if (!SD.begin()) {
    return;
  }
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  test.createSprite(220, 176);
  test.setTextColor(TFT_GREEN, TFT_BLACK);

  readFile(SD, "/Сказка.txt");
  bufSize = buffer.length() / 2 / 24;

  //tft.loadFont("Шрифты/zCyrillicOld20", SD);
  tft.loadFont(FONT);
  tft.print(buffer);
  tft.unloadFont();
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
}

void Command() {
  if (input_string.equals("6") == true)
  {
    if (i != 0) i = i + 19;
    test.fillRect(0, 0, 220, 176, TFT_BLACK);
    test.setCursor(0, i);
    //test.loadFont("Шрифты/zCyrillicOld20", SD);
    test.loadFont(FONT);
    test.print(buffer);
    test.unloadFont();
    test.pushSprite(0, 0, TFT_TRANSPARENT);
  } else if (input_string.equals("7") == true) {
    if (i != bufSize * (-19)) i = i - 19;
    test.fillRect(0, 0, 220, 176, TFT_BLACK);
    test.setCursor(0, i);
    //test.loadFont("Шрифты/zCyrillicOld20", SD);
    test.loadFont(FONT);
    test.print(buffer);
    test.unloadFont();
    test.pushSprite(0, 0, TFT_TRANSPARENT);
  }
}

void readFile(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  if (!file) {
    tft.println("Не удалось открыть файл для чтения");
    return;
  }
  while (file.available())  //Читаем содержимое файла
  {
    buffer = file.readStringUntil('\n');
  }
  file.close();
}
