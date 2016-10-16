cmake_minimum_required(VERSION 3.3)

project(ukoct)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Files
file(GLOB LIB_HEADERS include/*.hpp include/**/*.hpp)
file(GLOB LIB_SOURCES src/lib/*.cpp src/lib/**/*.cpp)
file(GLOB EXEC_SOURCES src/exec/*.cpp src/exec/**/*.cpp)
file(GLOB LIB_TEST_SOURCES test/lib/*.cpp test/lib/**/*.cpp)

# Include header files
include_directories(ext/include)
include_directories(include)

# Create libraries
#add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
add_library(${PROJECT_NAME} INTERFACE)

# Install libraries and headers
#install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})
install(FILES ${LIB_HEADERS} DESTINATION include/${PROJECT_NAME})

# Testing
add_executable(ukoct_test ${LIB_TEST_SOURCES})
target_link_libraries(ukoct_test ${PROJECT_NAME})
enable_testing()
add_test(NAME ukoct_test COMMAND ukoct_test)
