cmake_minimum_required(VERSION 3.26)
project(gate_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(gate_server main.cpp
        json/jsoncpp.cpp
)

target_include_directories(gate_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/json)

if(MSVC)
    target_compile_options(gate_server PRIVATE /W4)
else()
    target_compile_options(gate_server PRIVATE -Wall -Wextra -pedantic)
endif()
