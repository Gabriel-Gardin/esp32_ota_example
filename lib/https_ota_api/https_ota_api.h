#ifndef HTTPS_OTA_API_H
#define HTTPS_OTA_API_H 

#define CONFIG_FIRMWARE_UPGRADE_URL "http://otaesp32test.s3.us-west-2.amazonaws.com/firmwares/firmware.bin"
#define OTA_RECV_TIMEOUT 2000

void start_ota_update();

#endif