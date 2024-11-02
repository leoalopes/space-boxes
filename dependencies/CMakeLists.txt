cmake_minimum_required(VERSION 3.24)

add_library(glad SHARED glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_library(stb SHARED stb/stb.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb/include)
