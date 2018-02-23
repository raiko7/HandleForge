cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(testHandleForge testHandleForge.cpp)

install(TARGETS testHandleForge DESTINATION bin)
