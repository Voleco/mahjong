
cmake_minimum_required(VERSION 3.6)

project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sample sample.cpp)

target_link_libraries(sample PUBLIC cards deck game_server hand_evaluator policy)