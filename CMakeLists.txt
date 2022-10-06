cmake_minimum_required(VERSION 3.10)  # Min version of CMake

set( CMAKE_INCLUDE_PATH "/usr/include")
set( CMAKE_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu/libncurses.so")

project("AiP_CourseWork" VERSION 0.1.0)  # Set project name

add_library(menu SHARED menu.c)  # Add menu to project

# find_package(curses)
# target_link_libraries("AiP_CourseWork" curses)
# add_custom_target(ncurses)
# # add_library(nsurses ncurses)  # Add ncurses to project

find_package(Curses REQUIRED)
include_directories(/usr/include)
add_executable("AiP_CourseWork" main.c)         # Add main file to project
target_link_libraries("AiP_CourseWork" curses)
target_link_libraries("AiP_CourseWork" panel)

target_link_libraries("AiP_CourseWork" menu)		# Линковка программы с библиотекой