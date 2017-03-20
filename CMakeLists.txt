cmake_minimum_required(VERSION 3.6)

project(adl
        VERSION 0.1.0
        LANGUAGES CXX)
set(GLOBAL PROPERTY SONAME 0)
set(GLOBAL PROPERTY PROJECT_BRIEF "Abstract Domain Library, a C++ library implementing the octagon domain in CPU and GPU.")

# For C++14 support
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message( FATAL_ERROR "In-source builds not allowed. To build, create a folder and run CMake from there." )
endif()

add_definitions(-Dadl_BUILDING)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-Dadl_DEBUG)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    add_definitions(-Dadl_STRIPPED)
endif()

#
# [ TARGETS ]
#


# Third-party header-only dependency target
set(ext_iapi ${PROJECT_NAME}-ext-iapi)
set(ext_iapi_dir_root "${CMAKE_CURRENT_SOURCE_DIR}/ext/include")
add_library(${ext_iapi} INTERFACE)
target_include_directories(${ext_iapi} INTERFACE ${ext_iapi_dir_root})
target_sources(${ext_iapi} INTERFACE
    "${ext_iapi_dir_root}/catch.hpp"
    )


# Public API target
set(api ${PROJECT_NAME}-api)
set(api_dir_root "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(${api} INTERFACE)
target_include_directories(${api} INTERFACE ${api_dir_root})
target_link_libraries(${api} INTERFACE ${ext_iapi})
target_sources(${api} INTERFACE
    # e.g. "${api_dir_root}/adl.hpp"
    # adl
    "${api_dir_root}/adl.hpp"
    "${api_dir_root}/adl.fwd.hpp"
    "${api_dir_root}/adl.cfg.hpp"
    "${api_dir_root}/adl/config.hpp"
    "${api_dir_root}/adl/intl.hpp"
    "${api_dir_root}/adl/error.fwd.hpp"
    "${api_dir_root}/adl/error.hpp"

    # adl/util
    "${api_dir_root}/adl/util.hpp"
    "${api_dir_root}/adl/util.fwd.hpp"

    # adl/oct
    "${api_dir_root}/adl/oct.hpp"
    "${api_dir_root}/adl/oct.fwd.hpp"
    "${api_dir_root}/adl/oct/domain_space.hpp"

    # adl/oct/dsl
    "${api_dir_root}/adl/oct/dsl.hpp"
    )


# Internal API target
set(iapi ${PROJECT_NAME}-iapi)
set(iapi_dir_root "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
add_library(${iapi} INTERFACE)
target_include_directories(${iapi} INTERFACE ${iapi_dir_root})
target_link_libraries(${iapi} INTERFACE ${api})


# Test-only internal API target
set(test_iapi ${PROJECT_NAME}-test-iapi)
set(test_iapi_dir_root "${CMAKE_CURRENT_SOURCE_DIR}/test/include")
add_library(${test_iapi} INTERFACE)
target_include_directories(${test_iapi} INTERFACE ${test_iapi_dir_root})
target_link_libraries(${test_iapi} INTERFACE ${iapi})
target_sources(${test_iapi} INTERFACE
    "${test_iapi_dir_root}/adl_catch.hpp"
    "${test_iapi_dir_root}/adl_catch/macros.hpp"
    )


# Main binary/library/executable target
#set(main ${PROJECT_NAME}) # Main target
#    set(main_dir_root ${CMAKE_CURRENT_SOURCE_DIR}/src)
#    set(main_objs ${PROJECT_NAME}-api-objs)
#    set(main_deps ${api} ${iapi} ${ext_iapi})
#    set(main_sharedlib ${PROJECT_NAME})
#    set(main_staticlib ${PROJECT_NAME}-static)

# Test target
set(test ${PROJECT_NAME}-test)
set(test_dir_root "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(test_dir_unit "${CMAKE_CURRENT_SOURCE_DIR}/test/unit")
add_executable(${test}
    "${test_dir_unit}/main.unit.cpp"

    # adl
    "${test_dir_unit}/adl/config.unit.cpp"
    "${test_dir_unit}/adl/intl.unit.cpp"
    "${test_dir_unit}/adl/error.unit.cpp"
)
target_link_libraries(${test} ${test_iapi})
enable_testing()
add_test(NAME ${test} COMMAND "./${test}")


#
# [ DOCUMENTATION GENERATION ]
#


find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

#
# [ CMAKE DEBUGGING ]
#


#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
