cmake_minimum_required(VERSION 3.19)


add_executable(tests)

target_sources(tests PRIVATE tests.cpp ../src/util.cpp ../src/config.cpp)
target_include_directories(tests PRIVATE ../include ../src)

target_link_libraries(tests PRIVATE GTest::gtest_main)
target_compile_features(tests PRIVATE cxx_std_20)


add_test(Test tests)
