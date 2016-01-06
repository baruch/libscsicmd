cmake_minimum_required(VERSION 2.8)
project(libscsicmd)

export(PACKAGE libscsicmd)

include_directories("include")
add_compile_options(-Wall -Wextra -Wshadow -Wmissing-prototypes -Winit-self -g)
add_definitions(-D_GNU_SOURCE -D_FORTIFY_SOURCE=2)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Werror -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")

add_subdirectory(src)
add_subdirectory(test)
