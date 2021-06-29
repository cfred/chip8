cmake_minimum_required(VERSION 3.5)

project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(SRC_FILES src/main.cpp src/chip8.cpp src/display.cpp src/keyboard.cpp)

add_executable(chip8 ${SRC_FILES})
FILE(COPY src/roms DESTINATION "${CMAKE_BINARY_DIR}")
target_link_libraries(chip8 -lSDL2)
