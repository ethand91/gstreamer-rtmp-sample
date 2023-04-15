cmake_minimum_required(VERSION 3.10)
project(rtmpcam)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(${GST_INCLUDE_DIRS})

add_executable(rtmpcam main.cpp)
target_link_libraries(rtmpcam ${GST_LIBRARIES})
