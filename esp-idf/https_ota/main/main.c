#include <stdio.h>
#include <string.h>
#include "wifi_provisioning_api/wifi_provisioning_api.h"
#include <nvs_flash.h>
#include "esp_err.h"
#include <esp_log.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "https_ota_api/https_ota_api.h"
#include "freertos/task.h"

static const char TAG[] = "Main app";


/*Simples exemplo de provisionamento de credenciais Wi-Fi e atualização OTA via https*/



void main_task(void *PvParmeters)
{
    for(;;)
    {
        ESP_LOGI(TAG, "Firmware antigasso");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main()
{
        /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated
         * and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());

        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    xTaskCreatePinnedToCore(main_task, "main_task", 8192, NULL, 1, NULL, 1);

    if(start_wifi_or_provisioning() != ESP_OK){
        ESP_LOGE(TAG, "Erro ao inicializar o wifi provisioning");
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
    start_ota_update();
}