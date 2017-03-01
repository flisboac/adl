
# Folder layout:
# /                            (Project's root folder)
# +- CMakeLists.txt
# +- include/                  (include folder for exported/public modules)
#    +- <module|component>?    (can be a header or folder)
# +- src/
#    +- include/               (include folder for static/private modules)
#       +- <module|component>? (can be a header or folder)
#    +- <module|component>?    (can be a source file or folder)

# Optional test folders:
# /
# +- ext/                      (For any external, local dependency.)
#    +- include/               (For header-only dependencies and simple drop-in)
# +- test/                     (For unit tests only! Specs and regressions are put somewhere else. Module layout)
#    +- include/
#       +- <module|component>? (include folder used for test execution, be it unit, spec or fix)
#    +- <module|component>?
# +- spec/                     (only if there's something akin to use cases supporting the tests. Use-case oriented layout)
#    +- include/
#    +- <use_case>?
# +- fix/                      (should be created as soon as a fix (and consequent regression test) is created. Per-fix oriented layout)
#    +- include/
#    +- <bug_tracker_artifact>?

# Project phases:
# - build
# - test
#   - test_unit
#   - test_spec
#   - test_fix

#CMake Target variable names:
# <target>                 (the main target name)
#    <target>_dir_root
#    <target>_dir_include
#    <target>_dir_source
#    <target>_deps         (a list with all dependencies for this target)
#    <target>_objs         (a target name for a OBJECTS target, for binaries only)
#    <target>_sharedlib    (a target name for a SHARED target, if any distinction must be made by name)
#    <target>_staticlib    (a target name for a STATIC target, if any distinction must be made by name)

# CMake Target Libraries:
# - ${PROJECT_NAME}-api            : include/
# - ${PROJECT_NAME}-iapi           : src/include/   ; ${PROJECT_NAME}-api
# - ${PROJECT_NAME}-ext-api        : include/
# - ${PROJECT_NAME}-ext-iapi       : src/include/   ; ${PROJECT_NAME}-api
# - ${PROJECT_NAME}-test-iapi      : test/include/  ; ${PROJECT_NAME}-iapi
# - ${PROJECT_NAME}-test_spec-iapi : spec/include/  ; ${PROJECT_NAME}-test-api
# - ${PROJECT_NAME}-test_fix-iapi  : fix/include/   ; ${PROJECT_NAME}-test-api

# CMake Targets:
# - ${PROJECT_NAME}                : src/**/*.cpp              ; ${PROJECT_NAME}-api
# - ${PROJECT_NAME}-api              (only if needed, or for header-only projects)
# - ${PROJECT_NAME}-test           : {test,spec,fix}/**/*.cpp  ; ${PROJECT_NAME}-test-api, ${PROJECT_NAME}-test_spec-api, ${PROJECT_NAME}-test_fix-api
# - ${PROJECT_NAME}-test_unit      : test/**/*.cpp             ; ${PROJECT_NAME}-test-api
# - ${PROJECT_NAME}-test_spec      : spec/**/*.cpp             ; ${PROJECT_NAME}-test_spec-api
# - ${PROJECT_NAME}-test_fix       : spec/**/*.cpp             ; ${PROJECT_NAME}-test_fix-api

cmake_minimum_required(VERSION 3.6)

project(adl
        VERSION 0.1.0
        LANGUAGES CXX)

# For C++14 support
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message( FATAL_ERROR "In-source builds not allowed. To build, create a folder and run CMake from there." )
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
#target_sources(${api} INTERFACE
    # e.g. "${api_dir_root}/adl.hpp"
#    )


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
)
target_link_libraries(${test} ${test_iapi})
enable_testing()
add_test(NAME ${test} COMMAND "./${test}")


#
# [ CMAKE DEBUGGING ]
#


#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
