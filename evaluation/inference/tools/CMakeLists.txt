cmake_minimum_required(VERSION 3.0)
project(crop CXX)

#set(OpenCV_DIR "/home/huaijing/3rd/opencv-3.2.0_source/build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(crop crop.cpp)
target_link_libraries(crop ${OpenCV_LIBRARIES})
