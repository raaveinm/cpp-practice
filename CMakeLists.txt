cmake_minimum_required(VERSION 4.1)
project(tmp)

set(CMAKE_CXX_STANDARD 26)

# Sprint 9
add_executable(s9_e4_2 s9_e4_2_main.cpp)
add_executable(s9_e4 s9_e4_main.cpp)
add_executable(s9_e3 s9_e3_main.cpp)
add_executable(s9_e2 s9_e2_main.cpp)
add_executable(s9_e1 s9_e1_main.cpp assets/log_duration.h)

# Sprint 8
add_executable(task_6 task_6_main.cpp)
add_executable(task_5 task_5_main.cpp)
add_executable(SimpleVector simplevector/main.cpp simplevector/simple_vector.h simplevector/tests.h simplevector/array_ptr.h)
add_executable(task_3 task3/main.cpp task3/log_duration.h task3/my_assert.h task3/stack_vector.h)
add_executable(task_2 task_2_main.cpp)
add_executable(task_1 task_1_main.cpp)
