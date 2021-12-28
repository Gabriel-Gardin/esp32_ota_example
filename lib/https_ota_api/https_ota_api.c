#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "string.h"
#include "https_ota_api.h"

#include "nvs.h"
#include "nvs_flash.h"

//esp_https_ota_config_t

/*Exemplo simples de ataualização OTA usando uma comunicação HTTPS e autenticação via Token Usando o django rest-framework.
O certificado a ser usado é o certificado da autoridade certificadora. 
Note que o certificado e o token tem um prazo de valdiade e tem que ser atualizados periodicamente para garantir o funcionamento do equipamento.
*/

extern const uint8_t server_cert_pem_start[] asm("_binary_cert_pem_start");  //Certificado da autoridade de TLS usado pelo seu site
extern const uint8_t server_cert_pem_end[] asm("_binary_cert_pem_end");

static const char *TAG = "HTTPS_OTA_API";

static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
{
    esp_err_t err = ESP_OK;
    /* Uncomment to add custom headers to HTTP request */
    err = esp_http_client_set_header(http_client, "Authorization", "Token d119e8f35ca6bd0ab7e2e8ace3681323c4786e5a"); //Token d119e8f35ca6bd0ab7e2e8ace3681323c4786e5a

    return err;
}
//Basic Z2FyZGluOjEyM2FzZDQ1Ng==

void ota_task(void *pvParameter)
{
    ESP_LOGI(TAG, "Inicializando atualização OTA");

    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = CONFIG_FIRMWARE_UPGRADE_URL,
        .cert_pem = (char *)server_cert_pem_start,
        .timeout_ms = OTA_RECV_TIMEOUT,
        .keep_alive_enable = true,
        .skip_cert_common_name_check = false,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao inicializar o procedimento OTA");
        vTaskDelete(NULL);
    }

    for(;;) {
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
        ESP_LOGD(TAG, "Bytes lidos da nova imagem: %d", esp_https_ota_get_image_len_read(https_ota_handle));
    }

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Erro ao carregar a imagem completa");
    } else {
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            esp_restart();
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted");
            }
            ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
            vTaskDelete(NULL);
        }
    }
}

void start_ota_update()
{
    xTaskCreate(&ota_task, "advanced_ota_example_task", 1024 * 8, NULL, 5, NULL);
}