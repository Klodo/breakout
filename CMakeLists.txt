cmake_minimum_required(VERSION 3.5)
project(breakout)

option(DEBUG "compile in debug mode" FALSE)

if (DEBUG)
    message("-- Building DEBUG profile")
    add_compile_options(-g)
    add_compile_options(-O0)
else (DEBUG)
    message("-- Building RELEASE profile")
    add_compile_options(-O3)
endif (DEBUG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    src/
    libs/
    libs/gl3w
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
)

set(libs
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    GL
    dl
)

file(GLOB breakout_src "src/*.cpp")
file(GLOB imgui_src "libs/*.cpp")

add_executable(breakout
    ${breakout_src}
    ${imgui_src}
    libs/gl3w/GL/gl3w.c
)

target_link_libraries(breakout ${libs})

set_target_properties(breakout PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    CXX_STANDARD 14
)
