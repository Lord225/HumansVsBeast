cmake_minimum_required(VERSION 3.17)
project(HumansVsBeast C)

set(CMAKE_C_STANDARD 11)

add_compile_options(
#        "-std=c11"
#        "-ggdb3"
#        "-xc"
        "-fdiagnostics-color"
        "-pedantic"
#        "-Werror"
        "-Wall"
        "-Wextra"
#        "-Wextra"
#        "-fmax-errors=5"
#        "-Werror=vla"
#        "-Wno-error=unused-parameter"
#        "-Wno-error=implicit-fallthrough"
#        "-Wno-error=parentheses"
#        "-Wno-parentheses"
#        "-D_GNU_SOURCE"
)

set(CURSES_NEED_NCURSES TRUE)

set(CURSES_NEED_WIDE TRUE) # test

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)

find_package(Threads REQUIRED)

include_directories(HumanVsBeast ${CURSES_INCLUDE_DIRS})

add_executable(HumansVsBeast main.c map.c map.h player.c player.h game.c game.h)

target_link_libraries(HumansVsBeast ${CURSES_LIBRARIES} Threads::Threads)