#ifndef EC_H_
#define EC_H_

#include "esp_camera.h"
#include "esp_log.h"

// ------- Set camera model ---------------------------------------------------- //
#ifndef EC_CAMERA_MODEL_
#define EC_CAMERA_MODEL_

//#define CAMERA_MODEL_WROVER_KIT     // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE        // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM  // Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE	  // Has PSRAM
//#define CAMERA_MODEL_AI_THINKER     // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//
// ..Or set the model externally before #include

#endif // EC_CAMERA_MODEL_
// ----------------------------------------------------------------------------- //


// ------- Camera pins --------------------------------------------------------- //
#ifndef EC_CAMERA_PINS_
#define EC_CAMERA_PINS_

#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_ESP_EYE)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_V2_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WIDE)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_ESP32CAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
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

#elif defined(CAMERA_MODEL_TTGO_T_JOURNAL)
#define PWDN_GPIO_NUM      0
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#else
#error "Camera model not selected"
#endif

#endif // EC_CAMERA_PINS_
// ----------------------------------------------------------------------------- //

esp_err_t ec_camera_init(camera_config_t *config);
esp_err_t ec_camera_capture();
void ec_process_image(
        const size_t width, 
        const size_t height, 
        const pixformat_t format, 
        const uint8_t* buf, 
        const size_t len
); 
#endif // EC_H_


// Both EC_CAMERA_IMPLEMENTATION and EC_NET_IMPLEMENTATION require TAG
#if defined(EC_CAMERA_IMPLEMENTATION) || defined(EC_NET_IMPLEMENTATION)
    #define TAG "ec:ec"
#endif // EC_CAMERA_IMPLEMENTATION || EC_NET_IMPLEMENTATION


// Camera related functions implementation
#if defined(EC_CAMERA_IMPLEMENTATION)

esp_err_t ec_camera_init(camera_config_t *config)
{
    esp_err_t err = esp_camera_init(config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "'ec_camera_init' failed, error: 0x%x", err);
        return err;
    }

    return ESP_OK;
}

esp_err_t ec_camera_capture() 
{
    // Acquire a frame
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        return ESP_FAIL;
    }

    //ec_process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
  
    // Return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);

    return ESP_OK;
}

void ec_process_image(
        const size_t width, 
        const size_t height, 
        const pixformat_t format, 
        const uint8_t* buf, 
        const size_t len) 
{
    /* ... */
}

#endif // EC_CAMERA_IMPLEMENTATION


// Network related functions implementation
#if defined(EC_NET_IMPLEMENTATION) && defined(EC_CAMERA_IMPLEMENTATION)
#endif // EC_NET_IMPLEMENTATION && EC_CAMERA_IMPLEMENTATION


// Undefine TAG to prevent variable name collision
#if defined(EC_CAMERA_IMPLEMENTATION) || defined(EC_NET_IMPLEMENTATION)
    #undef TAG
#endif // EC_CAMERA_IMPLEMENTATION || EC_NET_IMPLEMENTATION
