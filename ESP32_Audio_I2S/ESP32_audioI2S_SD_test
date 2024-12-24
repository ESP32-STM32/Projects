#include "Audio.h"
#include "SD.h"
// #include "FS.h"
// #include "SPIFFS.h"

#define FILESYSTEM SPIFFS // SPIFFS или SD

// #define I2S_DOUT 25  // DIN
// #define I2S_BCLK 27  // BCK
// #define I2S_LRC 26   // LRCK | LCK

Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);  // LEFT(26) | RIGHT(25) | Аналоговый сигнал DAC
//Audio audio; // I2S PCM5102A GND = FMT, SCK, DEMP | 3V3 = XSMT // GND = FLT | Цифровой сигнал I2S

void setup() {
  if (!FILESYSTEM.begin()) { // Подключение SD карты 23 - MOSI 19 - MISO, 18 - CLK, 5 - CS
    return;
  }
  //audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);  // Закомментировать при DAC
  audio.setVolume(21);        // Громкость 0...21
  audio.connecttoFS(FILESYSTEM, "Collide.mp3");
}

void loop() {
  audio.loop();
}
