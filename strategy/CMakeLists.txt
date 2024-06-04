cmake_minimum_required(VERSION 3.6)

project(strategy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(policy policy.cpp policy.hpp)
target_include_directories(policy PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(policy PUBLIC cards deck game_server hand_evaluator)