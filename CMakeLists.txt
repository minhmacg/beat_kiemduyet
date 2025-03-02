cmake_minimum_required(VERSION 3.25)

project(beatdata VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(beatdata src/main.cpp src/Beat.cpp)

target_include_directories(beatdata PRIVATE ${PROJECT_SOURCE_DIR}/include)
