idf_component_register(SRCS "io_handler.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_driver_gpio)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable
)
