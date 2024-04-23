cmake_minimum_required(VERSION 3.24)
project(qsch C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

add_executable(qsch main.c Sourcecode/grover_initialization_solver.c Sourcecode/grover_initialization_solver.h Sourcecode/state_generator.c Sourcecode/state_generator.h Sourcecode/helper_functions.c Sourcecode/helper_functions.h)
