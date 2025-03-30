cmake_minimum_required(VERSION 3.29)
project(strumok)

set(CMAKE_CXX_STANDARD 20)

add_executable(strumok main.cpp
        strumok/strumok.cpp
        strumok/strumok.h
        strumok/strumok_tables/strumok_tables.cpp
        strumok/strumok_tables/strumok_tables.h
)
##################################################
cmake_minimum_required(VERSION 3.29)
project(gd_attack)

set(CMAKE_CXX_STANDARD 20)

add_executable(gd_attack gd_attack.cpp
        strumok/strumok.cpp
        strumok/strumok.h
        strumok/strumok_tables/strumok_tables.cpp
        strumok/strumok_tables/strumok_tables.h
)
##################################################
cmake_minimum_required(VERSION 3.29)
project(generate_relations)

set(CMAKE_CXX_STANDARD 20)

add_executable(generate_relations generate_relations.cpp)