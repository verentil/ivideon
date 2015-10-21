cmake_minimum_required(VERSION 2.8)

project(ivideon)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB led.cpp led_user.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS thread date_time program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -std=c++11 -Wall")

add_library(own_lib STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main own_lib ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
