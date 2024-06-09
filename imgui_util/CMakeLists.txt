cmake_minimum_required(VERSION 3.10)

project(imgui_util)


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-result")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")

find_package(SFML REQUIRED graphics system window)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-1.90/)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(imgui-sfml-2.6)

set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot-0.16/)
add_library(implot 
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)
target_link_libraries(implot PUBLIC ImGui-SFML sfml-graphics)

add_library(imgui_util INTERFACE)
target_include_directories(imgui_util INTERFACE
    ${IMGUI_DIR}
    ${IMPLOT_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml-2.6
)
target_link_libraries(imgui_util INTERFACE 
    ImGui-SFML implot sfml-system sfml-window sfml-graphics OpenGL)

add_executable(imgui_util_demo
    imgui_util_demo.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
)
target_link_libraries(imgui_util_demo imgui_util)