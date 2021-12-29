idf_component_register(SRCS "main.c"
			     "https_ota_api/https_ota_api.c"
			     "wifi_provisioning_api/wifi_provisioning_api.c"
                 INCLUDE_DIRS "")
				 

target_add_binary_data(${COMPONENT_TARGET} "certs/cert.pem" TEXT)