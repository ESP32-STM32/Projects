#include <SD.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  if (!SD.begin()) {
    return;
  }
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.loadFont("LozungCapsBlack28", SD);
  tft.println("Hello");
  tft.println("Привет");
  tft.unloadFont();

  tft.setCursor(0, 144, 2);
  tft.print("Ob'em pamyati: ");
  if (SD.totalBytes() > 1073741824) {
    tft.print(float(SD.totalBytes()) / (1048576 * 1024));
    tft.println(" GB");
  } else SystemSize(SD.totalBytes());
  tft.print("Zanyato pamyati: ");
  if (SD.usedBytes() > 1073741824) {
    tft.print(float(SD.usedBytes()) / (1048576 * 1024));
    tft.println(" GB");
  } else SystemSize(SD.usedBytes());
}

void loop() {
}

void SystemSize(int32_t fs) {
  if (fs >= 1048576) {
    tft.print((float(fs) / (1024 * 1024)));  // Мегабайты
    tft.println(" MB");
  } else if (fs < 1048576 && fs >= 1024) {
    tft.print(fs / 1024);  // Килобайты
    tft.println(" KB");
  } else if (fs < 1024) {
    tft.print(fs);  // Байты
    tft.println(" B");
  } else tft.println("Error");
}
