add_executable(boost-tcp-server-client-tests
  TestHelper.hpp
  Test.cpp)

target_link_libraries(boost-tcp-server-client-tests PRIVATE boost-tcp-server-client)

target_compile_features(boost-tcp-server-client-tests PRIVATE cxx_std_17)

target_include_directories(boost-tcp-server-client-tests PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(boost-tcp-server-client-tests PRIVATE GTest::gtest GTest::gtest_main)
endif()
