idf_component_register(SRCS "telegram_bot.c"
                    PRIV_REQUIRES esp_http_client mbedtls
                    INCLUDE_DIRS "include")
