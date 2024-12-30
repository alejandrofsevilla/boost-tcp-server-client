cmake_minimum_required(VERSION 3.22.0)

project(boost-tcp-server-client LANGUAGES CXX)

add_subdirectory(tests)

add_library(${PROJECT_NAME} SHARED
  TcpClient.hpp
  TcpClient.cpp
  TcpConnection.hpp
  TcpConnection.cpp
  TcpServer.hpp
  TcpServer.cpp
  )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)

target_include_directories(${PROJECT_NAME} PUBLIC .)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()

