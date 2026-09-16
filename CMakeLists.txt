cmake_minimum_required(VERSION 3.16)

project(async_http_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(async_http_server
    src/server.cpp
    src/Route.cpp
)

target_include_directories(async_http_server PRIVATE includes)

target_compile_options(async_http_server PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
