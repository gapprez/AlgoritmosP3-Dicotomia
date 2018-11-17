cmake_minimum_required(VERSION 3.12)
project(AlgoritmosP3 C)

set(CMAKE_C_STANDARD 99)

add_executable(AlgoritmosP3 main.c dicotomia.c)
target_link_libraries(AlgoritmosP3 m)