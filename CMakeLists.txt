cmake_minimum_required(VERSION 3.10)
link_directories(lpc/bin)

SET(CMAKE_CXX_FLAGS_CACHE "${CMAKE_CXX_FLAGS}")
SET(GCC_STATIC_COMPILE_FLAGS "-static-libstdc++ -static-libgcc -s ./lpc/bin/libzmq.dll")
SET(LPC_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_STATIC_COMPILE_FLAGS} -D ADD_EXPORTS")

SET(CMAKE_CXX_FLAGS  "${LPC_CXX_FLAGS}")
ADD_LIBRARY(lpclient SHARED lpc/lpclient.cpp)


