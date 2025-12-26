cmake_minimum_required(VERSION 4.1)
project(tmp)

set(CMAKE_CXX_STANDARD 26)

# Sprint 8
add_executable(task_3 task3/main.cpp task3/log_duration.h task3/my_assert.h task3/stack_vector.h)
add_executable(task_2 task_2_main.cpp)
add_executable(task_1 task_1_main.cpp)
