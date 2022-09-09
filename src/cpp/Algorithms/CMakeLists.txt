cmake_minimum_required(VERSION 3.21)
project(Algorithms)
set(CMAKE_CXX_STANDARD 17)


include_directories(../Model)

add_library(
  Algorithms SHARED
  BeeColony.cpp
  EvolutionaryAlgorithm.cpp
  BeeColony.h
  EvolutionaryAlgorithm.h
)

target_compile_options(Algorithms PRIVATE ${COMPILER_FLAGS})
target_include_directories(Algorithms PUBLIC "../${CMAKE_ROOT_DIR}/Model")

