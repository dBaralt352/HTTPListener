cmake_minimum_required(VERSION 3.28)
project(HTTPListener C)

set(CMAKE_C_STANDARD 23)

add_library(HTTPListener STATIC HTTPListener.c)
