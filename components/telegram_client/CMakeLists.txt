idf_component_register(SRCS "telegram_client.c"
                    INCLUDE_DIRS "include")

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wconversion -Wshadow -Werror -Werror=unused-variable
)
