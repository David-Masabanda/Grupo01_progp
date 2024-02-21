cmake_minimum_required(VERSION 3.26)
project(Grupo1_pp)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt)
find_package(SFML)

#Habilita el soporte para OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


add_executable(Ejercicio1 Ejercicio1.cpp)

target_link_libraries(Ejercicio1 fmt::fmt sfml::sfml)




add_executable(Ejercicio2 Ejercicio2.cpp)

target_link_libraries(Ejercicio2 fmt::fmt)



