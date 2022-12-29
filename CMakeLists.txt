cmake_minimum_required(VERSION 3.0.0)
project(Rubbish_Bookstore VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast")
include_directories(

)
include(CTest)
enable_testing()

add_executable(code main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
