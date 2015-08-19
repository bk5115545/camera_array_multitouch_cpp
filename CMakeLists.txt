cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(camera_array_multitouch_cpp)

SET(CMAKE_CXX_FLAGS "-std=c++14")
set(Boost_USE_MULTITHREADED      ON)


FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
message(STATUS "--- OpenCV2: ${OpenCV_FOUND}")
message(STATUS "--- Boost: ${Boost_FOUND}")

add_subdirectory(src)
