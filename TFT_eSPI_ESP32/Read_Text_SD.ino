#include <SD.h>        // Библиотека для работы с SD-картой
#include <TFT_eSPI.h>  // Библиотека для работы с дисплеем

#include "zBrushT20.h"  // Подключение файла со шрифтом

#define FONT zBrushT20

TFT_eSPI tft = TFT_eSPI();             // Создание объекта для дисплея
TFT_eSprite test = TFT_eSprite(&tft);  // Создание объекта для слоя

String input_string;  // Буфер для приема данных через Serial port
String buffer = "";   // Текст с сд-карты
int bufSize = 0;      // Переменная для количества строк
int i = 0;            // Переменная для координат текста

void setup() {
  Serial.begin(115200);  // Инициализация передачи данных по Serial port
  if (!SD.begin()) {     // Инициализация SD-карты
    return;
  }
  tft.init();                              // Инициализация объекта дисплея
  tft.setRotation(1);                      // Выбор ориентации дисплея
  tft.fillScreen(TFT_BLACK);               // Заполнение дисплея черным цветом
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Установка цвета текста шрифта и цвета фона текста

  test.createSprite(220, 176);              // Создание слоя на весь экран
  test.setTextColor(TFT_GREEN, TFT_BLACK);  // Установка цвета текста шрифта и цвета фона текста

  readFile(SD, "/Сказка.txt");         // Чтение файла с SD-карты в буфер
  bufSize = buffer.length() / 2 / 24;  // Расчет примерного количества строк при шрифте 20px

  //tft.loadFont("Шрифты/zCyrillicOld20", SD);
  tft.loadFont(FONT);  // Загрузка шрифта
  tft.print(buffer);   // Вывод текста на дисплей из буфера
  tft.unloadFont();    // Выгрузка шрифта
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

    if (i != 0) i = i + 19;                    // Перемещаем текст вверх
    test.fillRect(0, 0, 220, 176, TFT_BLACK);  // Рисуем прямоугольник на весь экран
    test.setCursor(0, i);
    //test.loadFont("Шрифты/zCyrillicOld20", SD);
    test.loadFont(FONT);  // Загрузка шрифта
    test.print(buffer);   // Вывод текста на дисплей из буфера
    test.unloadFont();    // Выгрузка шрифта
    test.pushSprite(0, 0, TFT_TRANSPARENT);

  } else if (input_string.equals("7") == true) {

    if (i != bufSize * (-19)) i = i - 19;
    test.fillRect(0, 0, 220, 176, TFT_BLACK);  // Рисуем прямоугольник на весь экран
    test.setCursor(0, i);
    //test.loadFont("Шрифты/zCyrillicOld20", SD);
    test.loadFont(FONT);  // Загрузка шрифта
    test.print(buffer);   // Вывод текста на дисплей из буфера
    test.unloadFont();    // Выгрузка шрифта
    test.pushSprite(0, 0, TFT_TRANSPARENT);

  } else if (input_string.equals("8") == true) {

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.loadFont(FONT);  // Загрузка шрифта
    //listDir(SD, "/Шрифты", 0);
    listDir(SD, "/", 0);  // Вывод списка файлов и папок на дисплей
    tft.unloadFont();     // Выгрузка шрифта
  }
}

void readFile(fs::FS &fs, const char *path) {
  File file = fs.open(path);  // Открываем файл
  if (!file) {                // Проверяем существует ли файл
    tft.println("Не удалось открыть файл для чтения");
    return;
  }
  while (file.available())  // Читаем содержимое файла
  {
    buffer = file.readStringUntil('\n');  // Записывает текст в буфер
  }
  file.close();  // Закрываем файл
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
      tft.println(file.name()); // Вывод имени папки на дмсплей
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      tft.print(file.name()); // Вывод имени файла на дмсплей
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
