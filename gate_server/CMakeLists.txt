cmake_minimum_required(VERSION 3.26)
project(gate_server)

set(CMAKE_CXX_STANDARD 20)

# Windows 平台定义
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-D_WIN32_LEAN_AND_MEAN)
endif()

# Boost 路径配置（避免硬编码绝对路径）
# - 默认从系统路径/包管理器（如 vcpkg）查找
# - 可通过环境变量 BOOST_ROOT 或 -DBOOST_ROOT=<path> 指定
set(BOOST_ROOT "" CACHE PATH "Path to local Boost installation (optional)")
if(NOT BOOST_ROOT AND DEFINED ENV{BOOST_ROOT})
    set(BOOST_ROOT "$ENV{BOOST_ROOT}")
endif()
if(BOOST_ROOT)
    message(STATUS "Using BOOST_ROOT: ${BOOST_ROOT}")
endif()

# 设置 CMake 策略
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 OLD)
endif()

# 查找 Boost
find_package(Boost REQUIRED)

# 添加可执行文件
add_executable(gate_server
        main.cpp
        CServer.cpp
        AsioIOServicePool.cpp
        HttpConnection.cpp
        LogicSystem.cpp
        ConfigMgr.cpp
        VerifyEmailServiceClient.cpp
        urlcode.cpp
        json/jsoncpp.cpp
)

# 包含目录
target_include_directories(gate_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/json
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

if(WIN32)
    target_link_libraries(gate_server
            ws2_32
            mswsock
    )
endif()

# 编译选项
if(MSVC)
    target_compile_options(gate_server PRIVATE /W4)
else()
    target_compile_options(gate_server PRIVATE -Wall -Wextra -pedantic)
endif()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        ${CMAKE_CURRENT_BINARY_DIR}/config.ini
        COPYONLY
)