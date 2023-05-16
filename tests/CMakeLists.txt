add_executable(tcp-server-client-tests
  TestHelper.hpp
  Test.cpp)

target_link_libraries(tcp-server-client-tests PRIVATE tcp-server-client)

target_compile_features(tcp-server-client-tests PRIVATE cxx_std_17)

target_include_directories(tcp-server-client-tests PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(tcp-server-client-tests PRIVATE GTest::gtest GTest::gtest_main)
endif()
