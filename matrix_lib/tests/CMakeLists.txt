cmake_minimum_required(VERSION 3.0.0)
project(tests)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )


file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

message("MATRIX_LIB_INCLUDE_DIRS = ${MATRIX_LIB_INCLUDE_DIRS}")
message("MATRIX_LIB_LIBRARIES    = ${MATRIX_LIB_LIBRARIES}")

target_link_libraries(${PROJECT_NAME}       PRIVATE ${MATRIX_LIB_LIBRARIES} GTest::GTest gtest_main)

target_include_directories(${PROJECT_NAME}  PUBLIC  ${MATRIX_LIB_INCLUDE_DIRS})

gtest_discover_tests(${PROJECT_NAME})