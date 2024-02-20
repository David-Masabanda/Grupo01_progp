cmake_minimum_required(VERSION 3.26)
project(Grupo1_pp)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

#Habilita el soporte para OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


add_executable(Grupo1_pp main.cpp)
target_link_libraries(Grupo1_pp fmt::fmt)

add_executable(practica practica.cpp)


