#include <WiFi.h>
#include "Audio.h"

// #define I2S_DOUT 25  // DIN
// #define I2S_BCLK 27  // BCK
// #define I2S_LRC 26   // LRCK

Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);  // LEFT(26) | RIGHT(25) | Аналоговый сигнал DAC
//Audio audio;  // I2S PCM5102A GND = FMT, SCK, DEMP | 3V3 = XSMT // GND = FLT | Цифровой сигнал I2S

const char *SSID = "iPhone 5s";
const char *PASSWORD = "12345678";

const char *URL[] = { "http://vladfm.ru:8000/vfm",                   // Владивосток FM
                      "http://listen.vdfm.ru:8000/marusya",          // Маруся FM
                      "http://79.133.160.197:8000/1068-128.mp3",     // Авторадио (Омск)
                      "http://79.133.160.197:8000/1019-128win.mp3",  // Европа Плюс (Омск)
                      "https://www.nsktv.ru/onair2/VestiFM",         // Вести FM (Новосибирск)
                      "https://github.com/ESP32-STM32/Projects/raw/refs/heads/main/ESP32_Audio_I2S/Music/cheap-talk-push-play.opus" };

String input_string;
int sizeM = sizeof(URL) / sizeof(URL[0]);  // Размер массива со станциями
int station = 0;

int i = 21;  // Уровень громкости 0...21

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  //audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);  // Закомментировать при DAC
  audio.setVolume(i);
  audio.connecttohost(URL[0]);
  Serial.println();
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '=') {
      Command();
      input_string = "";
    } else {
      input_string += c;
    }
  }
  audio.loop();
}

void Command() {
  if (input_string.equals("1") == true) {  // Громкость +
    if (i < 21) {
      i += 1;
      audio.setVolume(i);
      Serial.println(i);
    }
  } else if (input_string.equals("2") == true) {  // Громкость -
    if (i > 0) {
      i -= 1;
      audio.setVolume(i);
      Serial.println(i);
    }
  } else if (input_string.equals("3") == true) {  // Плей
    station = 0;
    audio.connecttohost(URL[0]);
  } else if (input_string.startsWith("!") == true) {
    input_string.replace("!", "");
    audio.connecttohost(input_string.c_str());
    Serial.println(input_string);
  } else if (input_string.equals("4") == true) {  // Стоп
    audio.stopSong();
    Serial.println("Стоп");
  } else if (input_string.equals("5") == true) {  // Станция вперед
    if (station < sizeM - 1) {
      station++;
      audio.connecttohost(URL[station]);
      Serial.println(URL[station]);
    }
  } else if (input_string.equals("6") == true) {  // Станция назад
    if (station > 0) {
      station--;
      audio.connecttohost(URL[station]);
      Serial.println(URL[station]);
    }
  }
}

void audio_showstation(const char *info) {
  Serial.print("Станция: ");
  Serial.println(info);
}
void audio_showstreamtitle(const char *info) {
  Serial.print("Насвание потока: ");
  Serial.println(info);
}
