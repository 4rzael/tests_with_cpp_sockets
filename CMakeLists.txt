cmake_minimum_required(VERSION 3.5)
project(tests_with_cpp_sockets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/ASocket.hpp
    include/InputSocket.hh
    include/OutputSocket.hh
    include/SocketErrors.hpp
    include/StandardSerializer.hh
    src/INETSocket.cpp)

set(CLIENT_SOURCES
    src/examples/client.cpp)

set(SERVER_SOURCES
    src/examples/server.cpp)

include_directories(include/)

add_executable(client.exe ${CLIENT_SOURCES} ${SOURCE_FILES})
target_link_libraries(client.exe -lws2_32 -static -static-libgcc)

add_executable(server.exe ${SERVER_SOURCES} ${SOURCE_FILES})
target_link_libraries(server.exe -lws2_32 -static -static-libgcc)
