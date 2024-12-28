#include "Audio.h"
#include "SD.h"

#define FILESYSTEM SD  // SPIFFS или SD

#define I2S_DOUT 25  // DIN
#define I2S_BCLK 27  // BCK
#define I2S_LRC 26   // LRCK

//Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN); // LEFT(26) | RIGHT(25) | Аналоговый сигнал DAC
Audio audio;  // I2S PCM5102A GND = FMT, SCK, DEMP | 3V3 = XSMT // GND = FLT | Цифровой сигнал I2S

String input_string;
int currentTimeINT = 0;
String currentTimeSTR = "00:00";
int fileDurationINT = 0;
String fileDurationSTR = "00:00";

int i = 21;  // Уровень громкости 0 - 21
String music[10];
int sizeM = 0;
int track = 0;
int m = 0;
bool flag = false;
bool flagLoop = false;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
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
      if (text.endsWith(".mp3") || text.endsWith(".m4a") || text.endsWith(".opus") || text.endsWith(".wav")) {
        Serial.println(file.name());
        music[m] = dirname + text;
        m++;
      }
    }
    file = root.openNextFile();
  }
  sizeM = m;
  m = 0;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  if (!FILESYSTEM.begin()) {  // Подключение SD карты 23 - MOSI 19 - MISO, 18 - CLK, 5 - CS
    return;
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);  // Закомментировать при DAC
  audio.setVolume(i);                            // 0...21

  listDir(FILESYSTEM, "/", 0);
  Play(music[0]);
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
}

void Command() {
  if (input_string.equals("1") == true) {  // Показать текущее и общее время трека
    Serial.print("Текущее время ");
    currentTimeINT = audio.getAudioCurrentTime() / 60;
    if (currentTimeINT < 10) Serial.print("0");
    Serial.printf("%d:", currentTimeINT);
    currentTimeINT = audio.getAudioCurrentTime() - currentTimeINT * 60;
    if (currentTimeINT < 10) Serial.print("0");
    Serial.println(currentTimeINT);
    Serial.print("Общее время ");
    fileDurationINT = audio.getAudioFileDuration() / 60;
    if (fileDurationINT < 10) Serial.print("0");
    Serial.printf("%d:", fileDurationINT);
    fileDurationINT = audio.getAudioFileDuration() - fileDurationINT * 60;
    if (fileDurationINT < 10) Serial.print("0");
    Serial.println(fileDurationINT);
    Serial.println();
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
    Play(music[0]);
  } else if (input_string.equals("5") == true) {  // Пауза/Плей
    if (audio.isRunning()) {
      flag = false;
      audio.pauseResume();
    } else {
      audio.pauseResume();
      flag = true;
    }
    Serial.println("Пауза/Плей");
  } else if (input_string.startsWith("p") == true) {  // Перемотка
    input_string.replace("p", "");
    audio.setAudioPlayPosition(input_string.toInt());  // Перемотка на 50-ую секунду
    Serial.println("Перемотка " + input_string);
  } else if (input_string.equals("7") == true) {  // Стоп
    flag = false;
    audio.stopSong();
    Serial.println("Стоп");
  } else if (input_string.equals("8") == true) {  // Трек вперед
    if (track < sizeM - 1) {
      track++;
      Play(music[track]);
    }
  } else if (input_string.equals("9") == true) {  // Трек назад
    if (track > 0) {
      track--;
      Play(music[track]);
    }
  } else if (input_string.equals("10") == true) {  // Заполнить массив треков и показать в serial
    listDir(FILESYSTEM, "/", 0);
  } else if (input_string.equals("11") == true) {  // Включить повтор
    flagLoop = true;
    Serial.println("Повтор включен");
  } else if (input_string.equals("12") == true) {  // Выключить повтор
    flagLoop = false;
    Serial.println("Повтор выключен");
  }
}

void Play(String song) {
  flag = true;
  audio.stopSong();
  audio.connecttoFS(FILESYSTEM, song.c_str());
  Serial.printf("Трек %d ", track);
  Serial.println(song);
  if (track == sizeM - 1 && flagLoop) {
    track = -1;
  }
}

void NextPlay() {
  if (flag) {
    if (!audio.isRunning()) {
      if (track < sizeM - 1) {
        track++;
        Play(music[track]);
      }
    }
  }
}
