# author: snikons

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpendantic
  -Wunnused -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align
  -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return
  -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
  -Wformat=2 -Winit-self -std=c18 -O3")

#
# https://developer.gnome.org/glib/stable/glib-compiling.html
# glib: pkg-config --cflags glib-2.0
#
include_directories(
  /usr/include/glib-2.0
  /usr/lib64/glib-2.0/include

  /usr/include
)

add_executable(
  packet-sniffer
  src/main.c
)

#
# https://developer.gnome.org/glib/stable/glib-compiling.html
# glib: pkg-config --libs glib-2.0
#
target_link_libraries(
  packet-sniffer
  glib-2.0
)