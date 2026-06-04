cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(glprojects)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


add_executable(glprojects
    src/main.cpp
    src/shader.cpp
    src/textures.cpp
    src/glad.c
    include/texture.h
    include/shader.h)


target_link_libraries(glprojects PRIVATE glfw glm::glm)
target_include_directories(glprojects PRIVATE include)

# Copy the shaders folder next to the compiled executable
add_custom_command(TARGET glprojects POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:glprojects>/assets")
