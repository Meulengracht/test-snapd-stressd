cmake_minimum_required(VERSION 3.8)
project(log-stressd C)

add_executable(logger main.c)
target_link_libraries(logger systemd)

install(
  TARGETS logger 
  RUNTIME DESTINATION bin
)
