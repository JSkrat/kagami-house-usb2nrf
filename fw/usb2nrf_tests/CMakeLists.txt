cmake_minimum_required(VERSION 3.5)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(usb2nrf_tests LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set (CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")

add_definitions(-D"UNIT_TESTING=1")

add_executable(usb2nrf_tests RFParser.c tests.c functions.c units_structure.cpp main.cpp )
include_directories(../../../googletest/googletest/include)

find_library(TEST_LIBRARY gtestd ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib)
find_library(TEST_MAIN_LIBRARY gtest_maind ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib)
target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_LIBRARY})
target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_MAIN_LIBRARY})
