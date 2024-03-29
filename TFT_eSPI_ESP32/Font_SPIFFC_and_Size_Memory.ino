#include "FS.h"        // Библиотека для работы с файловой системой SPIFFS
#include "SPIFFS.h"    // Библиотека для работы с файловой системой SPIFFS
#include <TFT_eSPI.h>  // Библиотека для работы с дисплеем

TFT_eSPI tft = TFT_eSPI();  // Создание объекта для дисплея

String input_string;  // Буфер для приема данных через Serial port

String font1 = "goosberry_version_0220";

void setup() {
  Serial.begin(115200);  // Инициализация передачи данных по Serial port
  if (!SPIFFS.begin()) {
    while (1) yield();
  }
  tft.init();                              // Инициализация объекта дисплея
  tft.setRotation(1);                      // Выбор ориентации дисплея
  tft.fillScreen(TFT_BLACK);               // Заполнение дисплея черным цветом
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Установка цвета текста шрифта и цвета фона текста
  tft.setCursor(0, 4);
  tft.loadFont(font1);   // Загрузка шрифта
  tft.print("Привет");  // Вывод текста на дисплей из буфера
  tft.unloadFont();     // Выгрузка шрифта
}

void loop() {
  while (Serial.available() > 0) {  // Цикл для считывания команд из Serial port
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
  if (input_string.equals("6") == true) {

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 4);
    tft.loadFont(font1);   // Загрузка шрифта
    tft.print("Привет");  // Вывод текста на дисплей из буфера
    tft.unloadFont();     // Выгрузка шрифта

  } else if (input_string.equals("7") == true) {

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 4);
    tft.loadFont(font1);   // Загрузка шрифта
    listDir(SPIFFS, "/", 0);  // Вывод списка файлов и папок на дисплей
    tft.unloadFont();     // Выгрузка шрифта
  }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  File root = fs.open(dirname);
  if (!root) {
    tft.println("Не удалось открыть каталог");
    return;
  }
  if (!root.isDirectory()) {
    tft.println("Каталог несуществует");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      tft.println(file.name());  // Вывод имени папки на дмсплей
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      tft.print(file.name());  // Вывод имени файла на дмсплей
      tft.print(" | ");
      if (file.size() >= 1048576) {
        tft.print((float(file.size()) / (1024 * 1024)));  // Мегабайты
        tft.println(" МБ");
      } else if (file.size() < 1048576 && file.size() >= 1024) {
        tft.print(file.size() / 1024);  // Килобайты
        tft.println(" КБ");
      } else if (file.size() < 1024) {
        tft.print(file.size());  // Байты
        tft.println(" Б");
      } else tft.println("Ошибка");
    }
    file = root.openNextFile();
  }
}
