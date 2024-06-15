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
    imgui_util    deck elements resource_factory
)
# #find_package(OpenCV REQUIRED)
# #include_directories(${OpenCV_INCLUDE_DIRS})

# #####################################
# #              hand_analyzer_gui
# #####################################
# add_executable(hand_analyzer
#     hand_analyzer.cpp
#     hand_analyzer_gui.cpp
# )
# target_include_directories(hand_analyzer PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)

# target_link_libraries(sim_logplay imgui_util)
# target_link_libraries(sim_logplay fmt::fmt stdc++fs)
# target_link_libraries(sim_logplay tiny_file_dialogs)


# # ## need to link libz explicitly at the end to prevent
# # ## "error adding symbols: DSO missing from command line"
# # target_link_libraries(sim_logplay z)  
# # target_compile_definitions(sim_logplay
# #   PRIVATE 
# #   TEST_SOURCE_DIRECTORY="${TEST_SOURCE_DIRECTORY}"
# #   SIM_RESOURCE_DIR="${SIM_RESOURCE_DIR}"
# #   ${SCH_FEATURE_LIST}
# # )