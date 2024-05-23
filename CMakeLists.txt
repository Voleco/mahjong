cmake_minimum_required(VERSION 3.6)

project(mahjong_training)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Find packages go here.

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

add_subdirectory(game)

add_subdirectory(test_program)


add_subdirectory(gui)