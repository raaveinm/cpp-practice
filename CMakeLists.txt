cmake_minimum_required(VERSION 4.1)
project(tmp)

set(CMAKE_CXX_STANDARD 26)

#CMAKE_CXX_STANDARD
#CMAKE_BUILD_TYPE

set(HELLO_WORLD Hello " " World) # HELLO_WORLD = Hello, " ", World
message(STATUS "Value of HELLO_WORLD: " ${HELLO_WORLD})

set(SOURCE_DIR "src" CACHE STRING "Path of source files")
message(STATUS "SOURCE_DIR value: ${SOURCE_DIR}")

set(SOURCE_DIR "src2" CACHE STRING "Path of source files")
message(STATUS "SOURCE_DIR value after another set: ${SOURCE_DIR}")

set(SOURCE_DIR "no_cache")
message(STATUS "SOURCE_DIR value after another set: ${SOURCE_DIR}")

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
    message("RRR")
else()
    message("rrr")
    set(SYSTEM_LIBS)
endif()

#target_link_libraries(negate ${SYSTEM_LIBS})

#Sprint 15
add_executable(s15_e1 s15_e1/main.cpp s15_e1/img_lib.cpp s15_e1/img_lib.h s15_e1/ppm_image.cpp s15_e1/ppm_image.h)

#Sprint 14
add_executable(s14_e3_3 variadic_templates/s14_e3_3_main.cpp variadic_templates/optional.h)
add_executable(s14_e3_2_main variadic_templates/s14_e3_2_main.cpp)
add_executable(s14_e3 variadic_templates/s14_e3_main.cpp)
add_executable(self_harmed_vector s14_e2/main.cpp s14_e2/vector.h)
add_executable(s14_e1 s14_e1/main.cpp s14_e1/optional.h)

#Sprint 13
add_executable(s13_e10 s13_e10/ParseCitySubjson.cpp s13_e10/CheckDateTimeValidity.cpp s13_e10/LoadPerson.cpp)
add_executable(s13_e9 s13_e9_main.cpp)
add_executable(s13_e8 s13_e8_main.cpp)
add_executable(s13_e7 s13_e7_main.cpp)
add_executable(s13_e6 s13_e6/main.cpp s13_e6/budget_manager.h s13_e6/budget_manager.cpp s13_e6/date.h s13_e6/parser.h s13_e6/parser.cpp)
add_executable(s13_e5 s13_e5_main.cpp)
add_executable(s13_e4 s13_e4_main.cpp)
add_executable(s13_e3 s13_e3_main.cpp)
add_executable(s13_e2 s13_e2_main.cpp)
add_executable(s13_e1 s13_e1_main.cpp)

#Sprint 12
add_executable(s12_e4 s12_e4_main.cpp)
add_executable(s12_e3 s12_e3_main.cpp)
add_executable(s12_e2 s12_e2_main.cpp)
add_executable(s12_e1 s12_e1_main.cpp)

#Sprint 11
add_executable(s11_e7 s11_e7_animals.cpp)
add_executable(s11_e6 s11_e6/main.cpp s11_e6/json.h s11_e6/json.cpp)
add_executable(s11_e5 s11_e5/main.cpp s11_e5/svg.h s11_e5/svg.cpp)
add_executable(s11_e4 s11_e4/simulation.cpp s11_e4/people.h s11_e4/supervisor.h)
add_executable(s11_e3 s11_e3_main.cpp)
add_executable(s11_e2 s11e2/main.cpp s11e2/people.h s11e2/people.cpp)
add_executable(s11_e1 s11_e1_main.cpp)

# Sprint 10
add_executable(s10_e7 s10_e7_main.cpp)
add_executable(s10_e6 s10_e6_main.cpp)
add_executable(s10_e5 s10_e5_main.cpp)
add_executable(s10_e4 s10_e4/main.cpp s10_e4/cow.h s10_e4/drawer.h s10_e4/geom.h s10_e4/image.h s10_e4/tile.h)
add_executable(s10_e3 s10_e3/main.cpp s10_e3/canvas.h s10_e3/common.h s10_e3/shapes.h s10_e3/texture.h)
add_executable(s10_e2 s10_e2/main.cpp s10_e2/bimap.cpp s10_e2/bimap.h)
add_executable(s10_e1_1 s10_e1_1_main.cpp)
add_executable(s10_e1 s10_e1_main.cpp)

# Sprint 9
add_executable(s9_e7 s9_e7/main.cpp s9_e7/libstat.h s9_e7/libstat.cpp s9_e7/libstat_test.cpp)
add_executable(s9_e6 s9_e621/main.cpp s9_e621/ini.h s9_e621/ini.cpp)
add_executable(s9_e5_2 s9_e5/s9_e5_2_main.cpp s9_e5/xml.h s9_e5/xml.cpp)
add_executable(s9_e5 s9_e5/s9_e5_main.cpp s9_e5/json.h s9_e5/json.cpp)
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

add_executable(sheeesh trash.cpp)