idf_component_register(SRCS "wifi_handler.c"
                    INCLUDE_DIRS "include")

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable
)
