# This cmake file is just used to compile the examples.
cmake_minimum_required(VERSION 3.10)

project(
  Basic_Example
  VERSION 1.0
  LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")


add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples)