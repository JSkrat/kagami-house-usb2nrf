cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)

project(usb2nrf_tests LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D"UNIT_TESTING=1")

add_executable(usb2nrf_tests RFParser.c tests.c ../usb2nrf/functions.c main.cpp )
include_directories(../../../googletest/googletest/include)
include_directories(../usb2nrf)

find_library(TEST_LIBRARY gtestd ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib)
find_library(TEST_MAIN_LIBRARY gtest_maind ../../../googletest/build-googletest-Desktop_Qt_5_12_5_MinGW_64_bit-u041eu0442u043bu0430u0434u043au0430/lib)
target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_LIBRARY})
target_link_libraries(usb2nrf_tests LINK_PUBLIC ${TEST_MAIN_LIBRARY})
