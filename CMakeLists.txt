cmake_minimum_required(VERSION 3.5)
project(tests_with_cpp_sockets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    )

set(SERVER_SOURCES
    src/examples/serverTest.cpp
    src/Server.cpp)

include_directories(include/)

add_executable(server ${SERVER_SOURCES} ${SOURCE_FILES})
target_link_libraries(server -lws2_32 -static -static-libgcc)
