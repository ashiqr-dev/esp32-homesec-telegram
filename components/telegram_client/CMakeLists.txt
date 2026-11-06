idf_component_register(SRCS "telegram_client.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp-tls esp_http_client)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable
)
