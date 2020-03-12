cmake_minimum_required(VERSION 3.15)
project(pract)

set(CMAKE_CXX_STANDARD 20)

add_executable(pract main.cpp include/base/Class.h  include/base/Factory.h  include/base/compile_time/logical.h include/base/ClassRegistry.h)