cmake_minimum_required(VERSION 3.24)

add_executable(shooter src/main.cpp)

target_link_libraries(shooter core)

find_package(glfw3 3.4 REQUIRED)
target_link_libraries(shooter glfw)
