cmake_minimum_required(VERSION 3.25)

project(cpp_beat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LINKER_FLAGS "-m64")

file(GLOB BEATSRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(cpp_beat ${BEATSRC})
target_include_directories(cpp_beat PRIVATE ${PROJECT_SOURCE_DIR}/include)

