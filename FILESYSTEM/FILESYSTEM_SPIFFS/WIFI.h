#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "FS.h"
#include "SPIFFS.h"
#include "SD.h"
#include "web.h"

#define LOCAL_ON  // Подключатся к домашней сети
//#define AP_ON     // Создать точку доступа

#define LOCAL_SSID "default"       // Имя домашней WIFI сети
#define LOCAL_PASS "12345678test"  // Пароль домашней WIFI сети

#define AP_SSID "ESP32"     // Имя точки доступа
#define AP_PASS "12345678"  // Пароль точки доступа

const char *host = "esp32";  // http://esp32.local
unsigned long tmrHandle;
unsigned long tmrLCD;
uint64_t usedBytes = 0;
uint64_t totalBytes = 0;

File fsUploadFile;
String filesys = "SPIFFS";

WebServer server(80);

IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void handleDelete();
void SystemSize();
void handleFileList();
void handleFileUpload();

void init() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

#ifdef LOCAL_ON
  WiFi.begin(LOCAL_SSID, LOCAL_PASS);
#endif

#ifdef AP_ON
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
#endif
  // if (!SD.begin()) {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  MDNS.begin(host);
  server.on("/", HTTP_DELETE, handleDelete);
  server.on("/getSize", HTTP_POST, SystemSize);
  server.on("/getFiles", HTTP_POST, handleFileList);
  server.on(
    "/", HTTP_POST, []() {
      server.send(200, "text/plain", "1");
    },
    handleFileUpload);
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", Web_page);
  });
  server.begin();
  MDNS.addService("http", "tcp", 80);
}

void handleUP() {
  if (millis() - tmrHandle >= 2) {
    tmrHandle = millis();
    server.handleClient();
  }
  if (millis() - tmrLCD >= 300) {
    tmrLCD = millis();
    digitalWrite(2, !digitalRead(2));
  }
}

void getFiles(fs::FS &fs) {
  File root = fs.open("/");
  String output = "[";
  if (root.isDirectory()) {
    File file = root.openNextFile();
    while (file) {
      if (output != "[") {
        output += ',';
      }
      output += "{\"type\":\"";
      output += (file.isDirectory()) ? "dir" : "file";
      output += "\",\"name\":\"";
      output += String(file.path()).substring(1);
      output += "\"}";
      file = root.openNextFile();
    }
  }
  output += "]";
  server.send(200, "text/json", output);
}

void handleFileList() {
  getFiles(SPIFFS);
  // if (filesys == "SPIFFS") {
  //   getFiles(SPIFFS);
  // } else if (filesys == "SD") {
  //   getFiles(SD);
  // }
}

void filesUpload(fs::FS &fs) {
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }

    fsUploadFile = fs.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
  }
}

void handleFileUpload() {
  filesUpload(SPIFFS);
  // if (filesys == "SPIFFS") {
  //   filesUpload(SPIFFS);
  // } else if (filesys == "SD") {
  //   filesUpload(SD);
  // }
}

String SystemSize1(uint64_t bytes) {
  if (bytes >= 1073741824) {
    return String(float(bytes) / (1048576 * 1024)) + " ГБ";
  } else if (bytes < 1073741824 && bytes >= 1048576) {
    return String((float(bytes) / (1024 * 1024))) + " МБ";  // Мегабайты
  } else if (bytes < 1048576 && bytes >= 1024) {
    return String(bytes / 1024) + " КБ";  // Килобайты
  } else if (bytes < 1024) {
    return String(bytes) + " Б";  // Байты
  } else return "0";
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

uint64_t map1(uint64_t x, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SystemSize() {
  if (server.args() == 0) {
    Serial.println(server.args());
    Serial.println("BAD ARGS");
    return returnFail("BAD ARGS");
  }

  filesys = server.arg(0);

  usedBytes = SPIFFS.usedBytes();    // Занято
  totalBytes = SPIFFS.totalBytes();  // Всего
  // if (filesys == "SPIFFS") {
  //   usedBytes = SPIFFS.usedBytes();    // Занято
  //   totalBytes = SPIFFS.totalBytes();  // Всего
  // } else if (filesys == "SD") {
  //   usedBytes = SD.usedBytes();    // Занято
  //   totalBytes = SD.totalBytes();  // Всего
  // }

  int val = map1(usedBytes, 0, totalBytes, 0, 100);

  String res = "Занято: ";
  res += SystemSize1(usedBytes);
  res += " из: ";
  res += SystemSize1(totalBytes);

  String output = "{\"used\":\"";
  output += res;
  output += "\",\"total\":\"";
  output += String(val);
  output += "\"}";
  server.send(200, "text/json", output);
}

void deleteRecursive(fs::FS &fs, String path) {
  File file = fs.open((char *)path.c_str());
  if (!file.isDirectory()) {
    file.close();
    fs.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true) {
    File entry = file.openNextFile();
    if (!entry) {
      break;
    }
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(fs, entryPath);
    } else {
      entry.close();
      fs.remove((char *)entryPath.c_str());
    }
    yield();
  }

  fs.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete1(fs::FS &fs, String path) {
  if (path == "/" || !fs.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    Serial.println("BAD PATH");
    return;
  }
  deleteRecursive(fs, path);
  server.send(200, "text/plain", "1");
}

void handleDelete() {
  if (server.args() == 0) {
    Serial.println(server.args());
    Serial.println("BAD ARGS");
    return returnFail("BAD ARGS");
  }

  String path = "/";
  path += String(server.arg(0));

  handleDelete1(SPIFFS, path);
  // if (filesys == "SPIFFS") {
  //   handleDelete1(SPIFFS, path);
  // } else if (filesys == "SD") {
  //   handleDelete1(SD, path);
  // }
}
