#define CAMERA_MODEL_AI_THINKER
#define EC_CAMERA_IMPLEMENTATION
#define EC_NET_IMPLEMENTATION
#include "ec.h"
#include "config.h"

#include "esp_http_server.h" // httpd_handle_t

#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_wifi.h> // tcpip_adapter_init

static const char *TAG = "ec:main";

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
 
   // XCLK 20MHz or 10MHz
   .xclk_freq_hz = 20000000,
 
   // YUV422, GRAYSCALE, RGB565, JPEG
   .pixel_format = PIXFORMAT_JPEG,
  
   // QQVGA-UXGA Do not use sizes above QVGA when not JPEG
   .frame_size   = FRAMESIZE_QVGA,

   // 0-63 lower number means higher quality
   .jpeg_quality = 10,
   
   // If more than one, i2s runs in continuous mode. Use only with JPEG
   .fb_count = 1
};

typedef struct {
    httpd_req_t *req;
    size_t       len;
} jpg_chunking_t;

static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len)
{
    jpg_chunking_t *j = (jpg_chunking_t *) arg;
    if (!index) {
        j->len = 0;
    }

    if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
        return 0;
    }

    j->len += len;
    return len;
}

esp_err_t jpg_httpd_handler(httpd_req_t *req)
{
    int64_t time = esp_timer_get_time();
    size_t fb_len = 0;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "camera capture failed");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    esp_err_t err = httpd_resp_set_type(req, "image/jpeg");

    if (err == ESP_OK)
        err = httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");

    if (err == ESP_OK) {
        if (fb->format == PIXFORMAT_JPEG) {
            fb_len = fb->len;
            err = httpd_resp_send(req, (const char *)fb->buf, fb->len);
        } else {
            jpg_chunking_t jchunk = { req, 0 };
            err = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
            httpd_resp_send_chunk(req, NULL, 0);
            fb_len = jchunk.len;
        }
    }

    esp_camera_fb_return(fb);
        
    time = (esp_timer_get_time() - time) / 1000; // μs to ms

    ESP_LOGI(TAG, "JPEG: %uKb %d%dms", 
            (err == ESP_OK) ? (uint32_t)(fb_len / 1024) : (uint32_t)0,
            // Sort of a way to print int64_t:
            (int)(time >> 32), // [63..32] bits of int64_t
            (int)(time));      // [31.. 0] bits of int64_t

    return err;
}

httpd_uri_t uri_handler_jpg = {
    .uri = "/jpg",
    .method = HTTP_GET,
    .handler = jpg_httpd_handler
};

httpd_handle_t webserver_start(void) 
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "starting server in port: %d", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_handler_jpg);
        return server;
    }

    ESP_LOGE(TAG, "error starting server");

    return NULL;
}

void webserver_stop(httpd_handle_t server)
{
    httpd_stop(server);
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    httpd_handle_t *server = (httpd_handle_t *)ctx;
  
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START: {
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            ESP_ERROR_CHECK(esp_wifi_connect()); 
        } break;

        case SYSTEM_EVENT_STA_GOT_IP: {
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            ESP_LOGI(TAG, "Got IP: '%s'",
               ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
  
            // Start web server
            if (*server == NULL) {
                *server = webserver_start();
            }
        } break;

        case SYSTEM_EVENT_STA_DISCONNECTED: {
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            ESP_ERROR_CHECK(esp_wifi_connect());
  
            // Stop web server
            if (*server) {
                webserver_stop(*server);
                *server = NULL;
            }
        } break;
                                            
        default:
            break;
    }

    return ESP_OK;
}

void wifi_init(void *arg)
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, arg));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EC_CONFIG_SSID,
            .password = EC_CONFIG_PASS,
        },
    };
    
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main()
{
    httpd_handle_t server = NULL;
    ESP_ERROR_CHECK(nvs_flash_init());
    ec_camera_init(&camera_config);
    wifi_init(&server);
}
