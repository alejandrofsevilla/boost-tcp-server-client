project(boost-tcp-server-client-tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
  Test.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE boost-tcp-server-client)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
endif()
