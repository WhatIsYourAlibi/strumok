cmake_minimum_required(VERSION 3.29)
project(strumok)

set(CMAKE_CXX_STANDARD 20)

add_executable(strumok main.cpp
        strumok/strumok.cpp
        strumok/strumok.h
        strumok/strumok_tables/strumok_tables.cpp
        strumok/strumok_tables/strumok_tables.h
)
