cmake_minimum_required(VERSION 3.26)
project(Grupo1_pp)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt)
find_package(SFML)

#Habilita el soporte para OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


add_executable(Grupo1_pp main.cpp)
target_link_libraries(Grupo1_pp fmt::fmt sfml::sfml)

add_executable(practica practica.cpp)


