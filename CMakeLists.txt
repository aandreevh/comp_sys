cmake_minimum_required(VERSION 3.15)
project(pract)

set(CMAKE_CXX_STANDARD 20)

add_executable(pract main.cpp include/base/class/Class.h include/base/Factory.h include/base/CompileTime.h include/base/class/ClassRegistry.h include/base/stream/Stream.h)