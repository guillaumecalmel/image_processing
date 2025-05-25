cmake_minimum_required(VERSION 3.30)
project(image_processing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(image_processing
        bmp24.c
        bmp24.h
        bmp8.c
        bmp8.h
        main.c
        cmake-build-debug/flowers_color.bmp
)
