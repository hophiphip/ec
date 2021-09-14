#include "esp_camera.h"
#include "camera_model.h"
#include "camera_pins.h"

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

static camera_config_t camera_config = {
   .ledc_channel = LEDC_CHANNEL_0,
   .ledc_timer   = LEDC_TIMER_0,
  
   .pin_d0 = Y2_GPIO_NUM,
   .pin_d1 = Y3_GPIO_NUM,
   .pin_d2 = Y4_GPIO_NUM,
   .pin_d3 = Y5_GPIO_NUM,
   .pin_d4 = Y6_GPIO_NUM,
   .pin_d5 = Y7_GPIO_NUM,
   .pin_d6 = Y8_GPIO_NUM,
   .pin_d7 = Y9_GPIO_NUM,
  
   .pin_xclk     = XCLK_GPIO_NUM,
   .pin_pclk     = PCLK_GPIO_NUM,
   .pin_vsync    = VSYNC_GPIO_NUM,
   .pin_href     = HREF_GPIO_NUM,
   .pin_sscb_sda = SIOD_GPIO_NUM,
   .pin_sscb_scl = SIOC_GPIO_NUM,
   .pin_pwdn     = PWDN_GPIO_NUM,
   .pin_reset    = RESET_GPIO_NUM,
  
   .xclk_freq_hz = 20000000,
  
   .pixel_format = PIXFORMAT_JPEG,
  
   // If PSRAM is present
   .frame_size = FRAMESIZE_QVGA,
   .jpeg_quality = 10,
   .fb_count = 2
};


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

void app_main()
{
    printf("Hello world!\n");


    esp_err_t err = ESP_OK;
    err = camera_init();
    err = camera_capture();

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
