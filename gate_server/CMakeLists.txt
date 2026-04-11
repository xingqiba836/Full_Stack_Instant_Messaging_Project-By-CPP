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

set(VCPKG_ROOT_DIR "$ENV{VCPKG_ROOT}")
set(VCPKG_TRIPLET_DIR "${VCPKG_ROOT_DIR}/installed/x64-mingw-dynamic")
if(EXISTS "${VCPKG_TRIPLET_DIR}/share/hiredis/hiredisConfig.cmake")
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_TRIPLET_DIR}")
    find_package(hiredis CONFIG REQUIRED)
endif()

# 添加可执行文件
add_executable(gate_server
        main.cpp
        CServer.cpp
        AsioIOServicePool.cpp
        HttpConnection.cpp
        LogicSystem.cpp
        ConfigMgr.cpp
        VerifyEmailServiceClient.cpp
        RedisMgr.cpp
        urlcode.cpp
        json/jsoncpp.cpp
)

# 包含目录
target_include_directories(gate_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/json
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

if(TARGET hiredis::hiredis)
    target_link_libraries(gate_server PRIVATE hiredis::hiredis)
elseif(EXISTS "${VCPKG_TRIPLET_DIR}/include/hiredis/hiredis.h")
    target_include_directories(gate_server PRIVATE "${VCPKG_TRIPLET_DIR}/include")
    target_link_directories(gate_server PRIVATE "${VCPKG_TRIPLET_DIR}/lib")
    target_link_libraries(gate_server PRIVATE hiredis)
else()
    message(FATAL_ERROR "hiredis not found. Please install hiredis and/or set VCPKG_ROOT.")
endif()

if(WIN32)
    target_link_libraries(gate_server PRIVATE
            ws2_32
            mswsock
    )
endif()

# MinGW 动态链接时，运行时 DLL 需在 PATH 或与 exe 同目录，否则启动失败 0xC0000135 (STATUS_DLL_NOT_FOUND)
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    get_filename_component(_MINGW_RUNTIME_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
    add_custom_command(
        TARGET gate_server
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_MINGW_RUNTIME_BIN}/libgcc_s_seh-1.dll"
            $<TARGET_FILE_DIR:gate_server>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_MINGW_RUNTIME_BIN}/libstdc++-6.dll"
            $<TARGET_FILE_DIR:gate_server>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_MINGW_RUNTIME_BIN}/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:gate_server>
        COMMENT "Copy MinGW runtime DLLs beside gate_server.exe"
    )
    if(NOT EXISTS "${_MINGW_RUNTIME_BIN}/libgcc_s_seh-1.dll")
        message(WARNING "MinGW runtime DLLs not found under ${_MINGW_RUNTIME_BIN}; run may fail with 0xC0000135.")
    endif()
endif()

if(EXISTS "${VCPKG_TRIPLET_DIR}/bin/libhiredis.dll")
    add_custom_command(
        TARGET gate_server
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_TRIPLET_DIR}/bin/libhiredis.dll"
            $<TARGET_FILE_DIR:gate_server>
        COMMENT "Copy libhiredis.dll beside gate_server.exe"
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