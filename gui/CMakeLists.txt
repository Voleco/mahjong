cmake_minimum_required(VERSION 3.6)

project(gui_interface)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(ImGui-SFML REQUIRED)

add_library(elements elements.cpp elements.hpp)
target_link_libraries(elements 
    imgui_util cards
)

add_library(resource_factory resource_factory.cpp resource_factory.hpp)
target_link_libraries(resource_factory 
    imgui_util cards elements
)

add_executable(test_gui test_gui.cpp)

target_link_libraries(test_gui
    imgui_util   
    deck elements hand_evaluator 
    policy
    resource_factory
)

