idf_component_register(SRCS "wifi_sta.c"
                    PRIV_REQUIRES esp_wifi nvs_flash
                    INCLUDE_DIRS "include")
