cmake_minimum_required(VERSION 3.14)
project(debrujin_seq)

set(CMAKE_CXX_STANDARD 14)

add_executable(debrujin_seq main.cpp debruijn.h bitsperlong.h fxttypes.h necklace.h)