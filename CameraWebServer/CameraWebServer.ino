#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"  // 必要なピン設定が含まれる

// Wi-Fi設定
const char* ssid = "ponco";
const char* password = "chio0101";

void setup() {
  Serial.begin(115200);
  delay(1000);  // シリアル通信の安定化
  Serial.println("\nConnecting to WiFi...");

  // Wi-Fi接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // カメラ設定
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;  // JPEG形式で出力
  config.frame_size = FRAMESIZE_240X240; // 低解像度で安定化
  config.jpeg_quality = 15;              // JPEG品質を下げてメモリ使用量を削減
  config.fb_count = 1;                   // フレームバッファを1に設定
  config.fb_location = CAMERA_FB_IN_DRAM; // DRAMを使用して安定化

  // PSRAMの検出と設定
  if (psramFound()) {
    Serial.println("PSRAM found");
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    Serial.println("PSRAM not found");
  }

  // カメラ初期化
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  } else {
    Serial.println("Camera init succeeded.");
  }

  // カメラサーバーを開始
  extern void startCameraServer();
  startCameraServer();

  Serial.println("Camera ready! Use 'http://' and your IP address to connect.");
}

void loop() {
  delay(1000);  // サーバー処理はバックグラウンドで動作
}