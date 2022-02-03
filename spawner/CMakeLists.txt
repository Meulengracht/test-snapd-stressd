cmake_minimum_required(VERSION 3.16)
project(spawn-stressd C)

add_executable(spawner main.c)
target_link_libraries(spawner pthread)

install(TARGETS spawner)
