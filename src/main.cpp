extern "C" {
  #include "esp_camera.h"
  #include "camera_model.h"
  #include "camera_pins.h"
}

#include <Arduino.h>
#include <WiFi.h>

static const char *TAG = "main.c";

static camera_config_t camera_config = {};

esp_err_t camera_init()
{
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed, error: 0x%x", err);
        return err;
    }

    return ESP_OK;
}


void process_image(
        const size_t width, 
        const size_t height, 
        const pixformat_t format, 
        const uint8_t* buf, 
        const size_t len) 
{
    // TODO: ...
}


// TODO: Pass a function pointer
esp_err_t camera_capture()
{
    // Acquire a frame
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        return ESP_FAIL;
    }

    process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
  
    // Return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);

    return ESP_OK;
}

void setup() {
  Serial.begin(115200);

  camera_config.ledc_channel = LEDC_CHANNEL_0;
  camera_config.ledc_timer   = LEDC_TIMER_0;
  
  camera_config.pin_d0 = Y2_GPIO_NUM;
  camera_config.pin_d1 = Y3_GPIO_NUM;
  camera_config.pin_d2 = Y4_GPIO_NUM;
  camera_config.pin_d3 = Y5_GPIO_NUM;
  camera_config.pin_d4 = Y6_GPIO_NUM;
  camera_config.pin_d5 = Y7_GPIO_NUM;
  camera_config.pin_d6 = Y8_GPIO_NUM;
  camera_config.pin_d7 = Y9_GPIO_NUM;
  
  camera_config.pin_xclk     = XCLK_GPIO_NUM;
  camera_config.pin_pclk     = PCLK_GPIO_NUM;
  camera_config.pin_vsync    = VSYNC_GPIO_NUM;
  camera_config.pin_href     = HREF_GPIO_NUM;
  camera_config.pin_sscb_sda = SIOD_GPIO_NUM;
  camera_config.pin_sscb_scl = SIOC_GPIO_NUM;
  camera_config.pin_pwdn     = PWDN_GPIO_NUM;
  camera_config.pin_reset    = RESET_GPIO_NUM;
  
  camera_config.xclk_freq_hz = 20000000;
  
  camera_config.pixel_format = PIXFORMAT_JPEG;
  
  // If PSRAM is present
  camera_config.frame_size = FRAMESIZE_QVGA;
  camera_config.jpeg_quality = 10;
  camera_config.fb_count = 2;


  esp_err_t err = ESP_OK;
  err = camera_init();
  err = camera_capture();
}

void loop() {
  delay(3000);
  Serial.println("Hi");
}
