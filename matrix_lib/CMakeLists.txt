cmake_minimum_required(VERSION 3.0.0)
project(matrix_lib)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fPIC -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0")

if(TEST_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

if(DEBUG_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

file(GLOB_RECURSE SOURCES       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS       ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# без   PARENT_SCOPE для тестов
# с     PARENT_SCOPE для главного CMakeLists.txt
set(MATRIX_LIB_LIBRARIES       ${PROJECT_NAME}                 )
set(MATRIX_LIB_LIBRARIES       ${PROJECT_NAME}     PARENT_SCOPE)
set(MATRIX_LIB_INCLUDE_DIRS    ${INCLUDE_DIRS}     PARENT_SCOPE)
set(MATRIX_LIB_INCLUDE_DIRS    ${INCLUDE_DIRS}                 )

if (TEST_OPT)
    enable_testing()
    add_subdirectory(tests)
endif()
