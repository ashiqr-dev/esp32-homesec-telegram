idf_component_register(SRCS "io_handler.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES project_utils esp_driver_gpio esp_timer)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable
)
