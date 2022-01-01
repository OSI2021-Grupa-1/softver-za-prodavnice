cmake_minimum_required(VERSION 3.19)

add_executable(tests tests.cpp)
target_compile_features(tests PRIVATE cxx_std_20)

add_test(Test tests)
