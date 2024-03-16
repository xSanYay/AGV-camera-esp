#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "esp_camera.h"

// Replace with your network credentials
const char* ssid = "AstraLAN";
const char* password = "12345678";

// IP address settings
IPAddress ip(192, 168, 69, 69);  // Set your desired IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Create an instance of the ESP32 camera
camera_config_t config;

// Create an instance of the web server
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure camera
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Init Camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed!");
    return;
  }

  // Serve HTML and image
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Capture a photo as a stream and send it to the client
    camera_fb_t * fb = esp_camera_fb_get();
    if (fb) {
      request->send_P(200, "image/jpeg", fb->buf, fb->len);
      esp_camera_fb_return(fb);
    } else {
      request->send(500, "text/plain", "Camera capture failed");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // Do nothing here
}
