cmake_minimum_required(VERSION 3.22)

project(2024-lab1-myFTP)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(../include)

add_executable(ftp_server
    ${CMAKE_SOURCE_DIR}/src/ftp_server.cpp
    ${CMAKE_SOURCE_DIR}/src/ftp_server_funcs.cpp
    ${CMAKE_SOURCE_DIR}/src/ftp_server_utils.cpp)
add_executable(ftp_client 
    ${CMAKE_SOURCE_DIR}/src/ftp_client.cpp
    ${CMAKE_SOURCE_DIR}/src/ftp_client_funcs.cpp
    ${CMAKE_SOURCE_DIR}/src/ftp_client_utils.cpp)