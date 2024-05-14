cmake_minimum_required(VERSION 3.6)
project(gui_interface)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(ImGui-SFML REQUIRED)

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