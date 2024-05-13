
#include <PubSubClient.h>
#include "WiFi.h"
#include "esp_camera.h"

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Wifi variables
const char* ssid = "ATTiyMY3i2";
const char* password = "cc%e=nm8t8gj";
// const char* ssid = "ATTpgi2ziE";
// const char* password = "Dung1965";
// const char* ssid = "11411 Langdon Ln";
// const char* password = "11411LDLN";

// MQTT Broker variables
// const char* mqtt_broker = "91.121.93.94";
const char* mqtt_broker = "192.168.1.252";
const int mqtt_port = 1883;
const char* control_topic = "robot/control";
const char* camera_topic = "robot/camera";
WiFiClient esp_wifi;
PubSubClient cam_client(esp_wifi);

const uint8_t LED_PIN = 4;
const int MAX_IMG_SIZE = 1024 * 23; // 23552 bytes

// Frame variables
float fps = 30;
unsigned long current_time = 0;
unsigned long last_frame_time = 0;
uint16_t interval_time = 0;

void connect_to_wifi()
{
  // Begin WiFi connection
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Failed to connnect to Wifi. Retrying...");
  }
  Serial.println("Wifi connected successfully.");
}

void setup() 
{
  Serial.begin(115200);
  initialize_camera();
  connect_to_wifi();
  connect_to_mqtt();
  interval_time = (uint16_t)(1.0/fps * 1000);
  pinMode (LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() 
{
  current_time = millis();
  if(current_time - last_frame_time >= interval_time)
  {
    grab_image();
  }
  cam_client.loop();

}


void initialize_camera()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG)
  {

    if(psramFound())
    {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } 
    else 
    {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } 
  else 
  {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
    #if CONFIG_IDF_TARGET_ESP32S3
        config.fb_count = 2;
    #endif
  }

  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) 
  {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG)
  {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

  #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
  #endif

  #if defined(CAMERA_MODEL_ESP32S3_EYE)
    s->set_vflip(s, 1);
  #endif

}

// void initialize_camera(){
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
//   config.frame_size = FRAMESIZE_VGA; // 640x480
//   config.jpeg_quality = 10;
//   config.fb_count = 2;

//   // camera init
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.printf("Camera init failed with error 0x%x", err);
//     ESP.restart();
//     return;
//   }
// }

void grab_image()
{
  camera_fb_t * fb = esp_camera_fb_get();
  // Serial.println(fb->len);
  if(fb != NULL && fb->format == PIXFORMAT_JPEG && fb->len < MAX_IMG_SIZE)
  {
    cam_client.publish(camera_topic, fb->buf, fb->len);
    // bool result = client.publish(ESP32CAM_PUBLISH_TOPIC, (const char*)fb->buf, fb->len);
    last_frame_time = current_time;
  }
  esp_camera_fb_return(fb);

}

void connect_to_mqtt()
{
  // Connect to MQTT Broker
  cam_client.setServer(mqtt_broker, mqtt_port);
  cam_client.setCallback(subscriber_callback);

  while (!cam_client.connected()) 
  {
    if (cam_client.connect("ESPCAMClient")) 
    {
      Serial.println("Connected to MQTT Broker!");
    } 
    else 
    {
      Serial.println("Connection to MQTT failed. Retrying...");
      delay(500);
    }
    cam_client.subscribe(control_topic);
  }
}

void subscriber_callback(char* topic, byte* payload, unsigned int length) 
{
  
  // Create a character array to hold the payload plus a null terminator
  char message[length + 1];
  
  // Copy the payload to the message array
  for (unsigned int i = 0; i < length; i++) 
  {
    message[i] = (char)payload[i];
  }

  // Null-terminate the array
  message[length] = '\0';

  if (strncmp(message, "lights_on", 9) == 0) 
  {
    digitalWrite(LED_PIN, HIGH);
  } 
  else if (strncmp(message, "lights_off", 10) == 0) 
  {
    digitalWrite(LED_PIN, LOW);
  }  
}