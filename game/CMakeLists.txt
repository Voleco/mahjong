cmake_minimum_required(VERSION 3.6)

project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(cards cards.cpp cards.h)
target_include_directories(cards PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(deck deck.cpp deck.hpp)
target_include_directories(deck PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(game_server game_server.cpp game_server.h)
target_include_directories(game_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


add_library(hand_evaluator hand_evaluator.cpp hand_evaluator.h)
target_include_directories(hand_evaluator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)