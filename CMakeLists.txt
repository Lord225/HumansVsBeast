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
#        "-fmax-errors=5"
#        "-Werror=vla"
#        "-Wno-error=unused-parameter"
#        "-Wno-error=implicit-fallthrough"
#        "-Wno-error=parentheses"
#        "-Wno-parentheses"
#        "-D_GNU_SOURCE"
)

set(CURSES_NEED_NCURSES TRUE)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)

find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})

add_executable(server server.c map.c map.h player.c player.h game.c game.h utils.c utils.h socket.c socket.h common.h beast.c beast.h beast_behaviour.h beast_behaviour.c dfs.c dfs.h)

target_link_libraries(server ${CURSES_LIBRARIES} Threads::Threads)

add_executable(client client.c utils.c utils.h common.h client_utils.c client_utils.h)

target_link_libraries(client ${CURSES_LIBRARIES} Threads::Threads)

configure_file(default_map.txt map.txt COPYONLY)