#include "Audio.h"
#include "SD.h"
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include "buttons.h"

#define I2S_DOUT 25  // DIN
#define I2S_BCLK 27  // BCK
#define I2S_LRC 26   // LRCK | LCK

//Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN); // LEFT(26) | RIGHT(25) | Аналоговый сигнал DAC
Audio audio;  // I2S PCM5102A GND = FMT, SCK, DEMP | 3V3 = XSMT // GND = FLT | Цифровой сигнал I2S

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite play = TFT_eSprite(&tft);
TFT_eSprite dur = TFT_eSprite(&tft);
TFT_eSprite list = TFT_eSprite(&tft);

String input_string;
int currentTimeINT = 0;
String currentTimeSTR = "0:00";
int fileDurationINT = 0;
String fileDurationSTR = "0:00";

int i = 4;  // Уровень громкости 0 - 21
String music[10];
String musicName[10];
int sizeM = 0;
int track = 0;
int m = 0;
bool flag = false;
bool flagLoop = false;
unsigned long tmrDUR;
int32_t duration = 39;
bool pauseTrack = false;
bool flagList = true;
bool flagPicture = false;
bool flagDur = false;

void listDir(fs::FS& fs, const char* dirname, uint8_t levels) {
  Serial.println("Список треков на SD карте");

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Не удалось открыть каталог");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Директории не существует");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      String text = file.name();
      if (text.endsWith(".mp3") || text.endsWith(".ogg") || text.endsWith(".m4a") || text.endsWith(".opus") || text.endsWith(".wav")) {
        Serial.println(file.name());
        music[m] = dirname + text;
        musicName[m] = text;
        m++;
      }
    }
    file = root.openNextFile();
  }
  sizeM = m;
  m = 0;
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) return 0;
  list.pushImage(x, y, w, h, bitmap);
  return 1;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  if (!SD.begin()) {  // Подключение SD карты 23 - MOSI 19 - MISO, 18 - CLK, 5 - CS
    return;
  }

  tft.init();                 // Инициализация объекта дисплея
  tft.setRotation(1);         // Горизонтальная ориентация экрана
  tft.fillScreen(TFT_BLACK);  // Цвет фона экрана
  tft.setCursor(0, 0);

  list.createSprite(220, 120);
  list.setTextFont(2);
  list.setTextColor(TFT_WHITE, TFT_BLACK);  // Цвет текста
  list.setSwapBytes(true);
  play.createSprite(220, 40);
  play.setSwapBytes(true);
  dur.createSprite(220, 16);

  TJpgDec.setCallback(tft_output);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // Закомментировать при DAC
  audio.setVolume(i);  // 0...21

  listDir(SD, "/", 0);
  //Play(music[0]);
  List();
  DurStop();
  DrawPausePlay();
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
  audio.loop();
  NextPlay();
  Duration();
}

void Command() {
  if (input_string.equals("1") == true) {  // Показать текущее и общее время трека
    currentTimeINT = audio.getAudioCurrentTime() / 60;
    if (currentTimeINT < 10) Serial.print("0");
    Serial.printf("%d:", currentTimeINT);
    currentTimeINT = audio.getAudioCurrentTime() - currentTimeINT * 60;
    if (currentTimeINT < 10) Serial.print("0");
    Serial.println(currentTimeINT);

    fileDurationINT = audio.getAudioFileDuration() / 60;
    if (fileDurationINT < 10) Serial.print("0");
    Serial.printf("%d:", fileDurationINT);
    fileDurationINT = audio.getAudioFileDuration() - fileDurationINT * 60;
    if (fileDurationINT < 10) Serial.print("0");
    Serial.println(fileDurationINT);
  } else if (input_string.equals("2") == true) {  // Громкость +
    if (i < 21) {
      i += 1;
      audio.setVolume(i);
      Serial.println(i);
    }
  } else if (input_string.equals("3") == true) {  // Громкость -
    if (i > 0) {
      i -= 1;
      audio.setVolume(i);
      Serial.println(i);
    }
  } else if (input_string.equals("4") == true) {  // Плей
    track = 0;
    Play(music[track]);
    List();
  } else if (input_string.equals("5") == true) {  // Пауза/Плей
    if (audio.isRunning()) {
      flag = false;
      audio.pauseResume();
    } else if (audio.getFilePos()) {
      flag = true;
      audio.pauseResume();
    }
    Serial.println("Пауза/Плей");
    DrawPausePlay();
  } else if (input_string.equals("6") == true) {  // Перемотка
    audio.setAudioPlayPosition(50);               // Перемотка на 50-ую секунду
    Serial.println("Перемотка");
  } else if (input_string.equals("7") == true) {  // Стоп
    flag = false;
    audio.stopSong();
    flagPicture = false;
    Serial.println("Стоп");
    track = 0;
    List();
    DrawPausePlay();
    flagDur = false;
    DurStop();
  } else if (input_string.equals("8") == true) {  // Трек вперед
    if (track < sizeM - 1) {
      track++;
      Play(music[track]);
    } else if (track == sizeM) {
      track = 0;
      Play(music[track]);
    }
  } else if (input_string.equals("9") == true) {  // Трек назад
    if (track > 0) {
      track--;
      Play(music[track]);
    }
  } else if (input_string.equals("10") == true) {  // Заполнить массив треков и показать в Serial
    listDir(SD, "/", 0);
  } else if (input_string.equals("11") == true) {  // Включить повтор
    flagLoop = true;
    Serial.println("Повтор включен");
    DrawPausePlay();
    // if (track == sizeM && flagLoop) {
    //   track = -1;
    // }
  } else if (input_string.equals("12") == true) {  // Выключить повтор
    flagLoop = false;
    Serial.println("Повтор выключен");
    DrawPausePlay();
  } else if (input_string.equals("13") == true) {  // Обложка включена
    flagList = false;
    List();
    Serial.println("Обложка включена");
  } else if (input_string.equals("14") == true) {  // Обложка выключена
    flagList = true;
    Serial.println("Обложка выключена");
    List();
  } else if (input_string.equals("15") == true) {  // Обложка выключена
    Serial.print("track ");
    Serial.println(track);
  }
}

void Play(String song) {
  flagPicture = false;
  pauseTrack = true;
  flag = true;
  flagDur = true;
  audio.stopSong();
  audio.connecttoFS(SD, song.c_str());
  Serial.printf("Трек %d ", track);
  Serial.println(song);
  DrawPausePlay();
  List();
  // if (track == sizeM && flagLoop) {
  //   track = -1;
  // }
}

void NextPlay() {
  if (flag) {
    if (!audio.isRunning()) {
      pauseTrack = false;
      DrawPausePlay();
      if (track < sizeM - 1) {
        track++;
        Play(music[track]);
      } else {
        if (flagLoop) {
          track = 0;
          Play(music[track]);
        } else {
          track = 0;
          List();
          flagDur = false;
          flag = false;
          DurStop();
        }
      }
    }
  }
}

void DrawPausePlay() {
  play.fillRect(0, 0, 220, 40, TFT_BLACK);
  play.drawLine(0, 0, 220, 0, TFT_GREEN);
  play.pushImage(15, 5, 30, 30, mback); 
  play.pushImage(95, 5, 30, 30, mstop);
  play.pushImage(135, 5, 30, 30, mnext);
  if (flagLoop) {
    play.pushImage(175, 5, 30, 30, mloop);
  } else {
    play.pushImage(175, 5, 30, 30, mloopoff);
  }

  if (pauseTrack && flag) {
    play.pushImage(55, 5, 30, 30, mpause);
  } else {
    play.pushImage(55, 5, 30, 30, mplay);
  }
  play.pushSprite(0, 136, TFT_TRANSPARENT);
}

void DurStop() {
  dur.fillRect(0, 0, 220, 15, TFT_BLACK);
  dur.drawLine(0, 0, 220, 0, TFT_GREEN);
  dur.drawWideLine(39, 8, 179, 8, 4, 0x0378, TFT_BLACK);
  dur.setTextFont(2);
  dur.setTextColor(TFT_WHITE, TFT_BLACK);
  dur.setCursor(0, 1);
  dur.print("00:00");
  dur.setCursor(184, 1);
  dur.print("00:00");
  dur.fillRect(39, 2, 5, 12, TFT_RED);
  dur.pushSprite(0, 120, TFT_TRANSPARENT);
}

void Duration() {
  if (flagDur) {
    if (millis() - tmrDUR >= 500) {
      tmrDUR = millis();
      dur.fillRect(0, 0, 220, 15, TFT_BLACK);
      dur.drawLine(0, 0, 220, 0, TFT_GREEN);
      dur.drawWideLine(39, 8, 179, 8, 4, 0x0378, TFT_BLACK);
      dur.setTextFont(2);
      dur.setTextColor(TFT_WHITE, TFT_BLACK);
      dur.setCursor(0, 1);
      currentTimeINT = audio.getAudioCurrentTime() / 60;
      if (currentTimeINT < 10) dur.print("0");
      dur.printf("%d:", currentTimeINT);
      currentTimeINT = audio.getAudioCurrentTime() - currentTimeINT * 60;
      if (currentTimeINT < 10) dur.print("0");
      dur.print(currentTimeINT);

      dur.setCursor(184, 1);
      fileDurationINT = audio.getAudioFileDuration() / 60;
      if (fileDurationINT < 10) dur.print("0");
      dur.printf("%d:", fileDurationINT);
      fileDurationINT = audio.getAudioFileDuration() - fileDurationINT * 60;
      if (fileDurationINT < 10) dur.print("0");
      dur.print(fileDurationINT);

      duration = map(audio.getAudioCurrentTime(), 0, audio.getAudioFileDuration(), 39, 174);

      if (duration >= 39) {
        dur.fillRect(duration, 2, 5, 12, TFT_RED);
      } else {
        dur.fillRect(39, 2, 5, 12, TFT_RED);
      }
      dur.pushSprite(0, 120, TFT_TRANSPARENT);
    }
  }
}

void List() {
  if (flagList) {
    list.fillRect(0, 0, 220, 120, TFT_BLACK);
    list.setCursor(0, 0);
    for (int i; i < sizeM; i++) {
      if (i == track) {
        list.print(">");
      } else {
        list.print(" ");
      }
      list.printf("%d ", i);
      list.println(musicName[i]);
    }
    list.pushSprite(0, 0, TFT_TRANSPARENT);
  } else if (!flagList && flagPicture) {
    uint16_t w = 0, h = 0;
    TJpgDec.getSdJpgSize(&w, &h, "/cover.jpg");
    if (w <= 300 && h <= 300) {
      TJpgDec.setJpgScale(4);  // 1,2,4,8
      list.fillRect(0, 0, 220, 120, TFT_BLACK);
      list.setCursor(0, 98);
      list.print(musicName[track]);
      TJpgDec.drawSdJpg(110 - w / 8, 40 - h / 8, "/cover.jpg");
      list.pushSprite(0, 0, TFT_TRANSPARENT);
    } else if (w > 300 && h > 300) {
      TJpgDec.setJpgScale(8);  // 1,2,4,8
      list.fillRect(0, 0, 220, 120, TFT_BLACK);
      list.setCursor(0, 98);
      list.print(musicName[track]);
      TJpgDec.drawSdJpg(110 - w / 16, 40 - h / 16, "/cover.jpg");
      list.pushSprite(0, 0, TFT_TRANSPARENT);
    }
  } else if (!flagList && !flagPicture) {
    TJpgDec.setJpgScale(4);  // 1,2,4,8
    list.fillRect(0, 0, 220, 120, TFT_BLACK);
    list.setCursor(0, 98);
    list.print(musicName[track]);
    TJpgDec.drawSdJpg(73, 3, "/folder.jpg");
    list.pushSprite(0, 0, TFT_TRANSPARENT);
  }
}

void audio_id3image(File& file, const size_t pos, const size_t size) {  // cover image
  flagPicture = true;
  uint8_t buf[1024];
  file.seek(pos + 1);
  for (uint8_t i = 0u; i < 255; i++) {
    buf[i] = file.read();
    if (uint8_t(buf[i]) == 0) break;
  }
  File coverFile = SD.open("/cover.jpg", FILE_WRITE);
  size_t len = size;
  while (len) {
    uint16_t bytesRead = file.read(buf, sizeof(buf));
    if (len >= bytesRead) len -= bytesRead;
    else {
      bytesRead = len;
      len = 0;
    }
    coverFile.write(buf, bytesRead);
  }
  coverFile.close();
  if (!flagList) {
    List();
  }
}
