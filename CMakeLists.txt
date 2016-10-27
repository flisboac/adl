cmake_minimum_required(VERSION 3.3)

project(adl)

# Definitions

set(HEADERLIB_NAME    ${PROJECT_NAME}-headers)
set(SHAREDLIB_NAME    ${PROJECT_NAME})
set(STATICLIB_NAME    ${PROJECT_NAME}-static)
set(EXE_NAME          ${PROJECT_NAME}-solver)
set(TESTS_EXEC_NAME   ${PROJECT_NAME}-tests)

set(INCLUDE_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXT_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/ext/include)

set(SRC_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_SRC_DIR       ${SRC_DIR}/lib)
set(EXE_SRC_DIR       ${SRC_DIR}/exec)
set(TEST_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(UNIT_TEST_DIR     ${TEST_DIR}/unit)
set(LIB_UNIT_TEST_DIR ${UNIT_TEST_DIR}/lib)

if(${CMAKE_C_COMPILER_ID} EQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif()

# Files

file(GLOB LIB_HEADERS
    ${INCLUDE_DIR}/*.hpp
    ${INCLUDE_DIR}/**/*.hpp
)
file(GLOB LIB_SOURCES
    ${LIB_SRC_DIR}/*.cpp
    ${LIB_SRC_DIR}/**/*.cpp
)
file(GLOB EXEC_SOURCES
    ${EXE_SRC_DIR}/*.cpp
    ${EXE_SRC_DIR}/**/*.cpp
)
file(GLOB LIB_TEST_SOURCES
    ${LIB_UNIT_TEST_DIR}/*.cpp
    ${LIB_UNIT_TEST_DIR}/**/*.cpp
)

set(SOURCE_FILES
    ${LIB_HEADERS}
    ${LIB_SOURCES}
    ${EXEC_SOURCES}
    ${LIB_TEST_SOURCES}
)


# Include header files

include_directories(${EXT_INCLUDE_DIR})
include_directories(${INCLUDE_DIR})


# Create libraries

#add_library(${STATICLIB_NAME} STATIC ${LIB_SOURCES})
add_library(${HEADERLIB_NAME} INTERFACE)


# Install libraries and headers

#install(TARGETS ${STATICLIB_NAME} DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY ${INCLUDE_DIR} DESTINATION include)


# Testing

add_executable(${TESTS_EXEC_NAME} ${LIB_TEST_SOURCES})
target_link_libraries(${TESTS_EXEC_NAME} ${HEADERLIB_NAME}) # Add ${STATICLIB_NAME} later
enable_testing()
add_test(NAME ${TESTS_EXEC_NAME} COMMAND ${TESTS_EXEC_NAME})

