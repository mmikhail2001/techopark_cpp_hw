cmake_minimum_required(VERSION 3.0.0)
project(main)

set(CMAKE_CXX_STANDARD 17)

option(TEST_OPT "build test version" OFF)
option(DEBUG_OPT "build debug version" OFF)

add_subdirectory(matrix_lib)

add_executable(${PROJECT_NAME} main.cpp)

if(TEST_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

if(DEBUG_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()


target_include_directories(${PROJECT_NAME} PUBLIC ${MATRIX_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MATRIX_LIB_LIBRARIES})

