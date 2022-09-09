cmake_minimum_required(VERSION 3.21)
project(NetworkSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(COMPILER_FLAGS -Ofast)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(COMPILER_FLAGS -Ofast)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(COMPILER_FLAGS /O2 /Ot)
endif()

add_subdirectory(Model)
add_subdirectory(Algorithms)

add_executable(NetworkSolver main.cpp)
target_link_libraries(NetworkSolver PRIVATE Algorithms Model)
target_compile_options(NetworkSolver
                       PUBLIC "$<$<CONFIG:RELEASE>:${COMPILER_FLAGS}>")
target_compile_options(NetworkSolver
                       PUBLIC "$<$<CONFIG:DEBUG>:-g>")
