idf_component_register(SRCS "wifi_handler.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES project_utils esp_wifi nvs_flash)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable -Wno-sign-conversion
)
