#ifndef HTTPS_OTA_API_H
#define HTTPS_OTA_API_H 

#define CONFIG_FIRMWARE_UPGRADE_URL "https://gabriel3sat.pythonanywhere.com/api/device/100000002/"
#define OTA_RECV_TIMEOUT 2000

void start_ota_update();

#endif