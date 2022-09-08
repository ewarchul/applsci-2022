cmake_minimum_required(VERSION 3.21)
project(Model CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(
  Model SHARED
  Country.cpp
  Country.h
  MyVertex.cpp
  MyVertex.h
  MyArc.cpp
  MyArc.h
  MyRealisation.cpp
  MyRealisation.h
  Network.cpp
  Network.h
  Writer.cpp
  Writer.h
  Path.h
  Path.cpp
  Car.cpp
  Car.h
  Bee.cpp
  Bee.h
  DFS.cpp
  DFS.h)

target_compile_options(Model PRIVATE ${COMPILER_FLAGS})
target_include_directories(Model PUBLIC .)
