cmake_minimum_required(VERSION 3.22.0)

project(boost-tcp-server-client LANGUAGES CXX)

add_subdirectory(tests)

add_library(boost-tcp-server-client SHARED
  TcpClient.hpp
  TcpClient.cpp
  TcpConnection.hpp
  TcpConnection.cpp
  TcpServer.hpp
  TcpServer.cpp
  )

target_compile_features(boost-tcp-server-client PRIVATE cxx_std_17)

target_compile_options(boost-tcp-server-client PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
  target_include_directories(boost-tcp-server-client PUBLIC ${Boost_INCLUDE_DIRS})
endif()

