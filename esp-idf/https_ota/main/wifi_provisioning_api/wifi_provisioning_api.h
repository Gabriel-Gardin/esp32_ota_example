#ifndef WIFI_PROVIONING_H
#define WIFI_PROVIONING_H

#include "esp_err.h"
/**
* @brief Se as credenciais foram encontradas no NVS, inicializa o Wi-Fi, caso contrário inicaliza o Access point.
*/
esp_err_t start_wifi_or_provisioning();

#define CONFIG_EXAMPLE_PROV_MGR_MAX_RETRY_CNT 5

#define PROOF_OF_POSSESION "abcd1234"

#endif