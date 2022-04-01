CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(test_libevent)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE DEBUG)

INCLUDE_DIRECTORIES(/usr/local/include)
ADD_EXECUTABLE(hello_libevent hello_libevent.cpp)

LINK_DIRECTORIES(/usr/local/lib)
TARGET_LINK_LIBRARIES(hello_libevent libevent.a)