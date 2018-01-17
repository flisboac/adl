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

# Clang-check support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Various safety checks
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message( FATAL_ERROR "In-source builds not allowed. To build, create a folder and run CMake from there." )
endif()

# Build flags
add_definitions(-Dadl_BUILDING)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-Dadl_DEBUG)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
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
    "${api_dir_root}/adl/crtp.fwd.hpp"
    "${api_dir_root}/adl/crtp.hpp"
    "${api_dir_root}/adl/error.fwd.hpp"
    "${api_dir_root}/adl/error.hpp"
    "${api_dir_root}/adl/char_helper.hpp"
    "${api_dir_root}/adl/char_helper.fwd.hpp"
    "${api_dir_root}/adl/timer.hpp"
    "${api_dir_root}/adl/timer.fwd.hpp"

    # adl/std
    "${api_dir_root}/adl/std/optional.hpp"
    "${api_dir_root}/adl/std/impl/optional.hpp"
    "${api_dir_root}/adl/std/any.hpp"
    "${api_dir_root}/adl/std/impl/any.hpp"
    "${api_dir_root}/adl/std/string_view.hpp"

    # adl/oct
    "${api_dir_root}/adl/oct.hpp"
    "${api_dir_root}/adl/oct.fwd.hpp"
    "${api_dir_root}/adl/oct/traits.hpp"
    "${api_dir_root}/adl/oct/constant.hpp"

    # adl/oct/var
    "${api_dir_root}/adl/oct/var_id.hpp"
    "${api_dir_root}/adl/oct/var.hpp"
    "${api_dir_root}/adl/oct/var/var_base_.hpp"
    "${api_dir_root}/adl/oct/var/identity_var_base_.hpp"
    "${api_dir_root}/adl/oct/var/literal_var_base_.hpp"
    "${api_dir_root}/adl/oct/var/oct_var.hpp"
    "${api_dir_root}/adl/oct/var/octdiff_var.hpp"
    "${api_dir_root}/adl/oct/var/oct_lvar.hpp"
    "${api_dir_root}/adl/oct/var/octdiff_lvar.hpp"

    # adl/oct/vexpr
    "${api_dir_root}/adl/oct/vexpr.hpp"
    "${api_dir_root}/adl/oct/vexpr/vexpr_base_.hpp"
    "${api_dir_root}/adl/oct/vexpr/basic_oct_vexpr.hpp"
    "${api_dir_root}/adl/oct/vexpr/basic_octdiff_vexpr.hpp"

    # adl/oct/cons
    "${api_dir_root}/adl/oct/cons.hpp"
    "${api_dir_root}/adl/oct/cons/cons_base_.hpp"
    "${api_dir_root}/adl/oct/cons/basic_oct_cons.hpp"
    "${api_dir_root}/adl/oct/cons/basic_octdiff_cons.hpp"
    "${api_dir_root}/adl/oct/cons/basic_octdiff_conjunction.hpp"

    # adl/oct/system
    "${api_dir_root}/adl/oct/system.hpp"
    "${api_dir_root}/adl/oct/system/var_set_.hpp"
    "${api_dir_root}/adl/oct/system/system_base_.hpp"
    "${api_dir_root}/adl/oct/system/oct_system_.hpp"
    "${api_dir_root}/adl/oct/system/octdiff_system_.hpp"

    # adl/oct/oper
    "${api_dir_root}/adl/oct/oper.hpp"

    # adl/oct/dbm
    "${api_dir_root}/adl/oct/dbm.hpp"
    "${api_dir_root}/adl/oct/dbm/traits.hpp"
    "${api_dir_root}/adl/oct/dbm/dbm_base_.hpp"
    "${api_dir_root}/adl/oct/dbm/dense_dbm_base_.hpp"

    # adl/oct/context
    "${api_dir_root}/adl/oct/context.hpp"
    "${api_dir_root}/adl/oct/context/traits.hpp"
    "${api_dir_root}/adl/oct/context/context_base_.hpp"

    #adl/oct/cpu
    "${api_dir_root}/adl/oct/cpu.hpp"
    "${api_dir_root}/adl/oct/cpu/oper_base_.hpp"
    "${api_dir_root}/adl/oct/cpu/seq_context.hpp"
    "${api_dir_root}/adl/oct/cpu/async_context.hpp"
    "${api_dir_root}/adl/oct/cpu/dense_dbm.hpp"

    #adl/oct/cpu/operators
    "${api_dir_root}/adl/oct/cpu/close_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/strong_close_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/strengthen_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/tighten_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/is_coherent_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/is_consistent_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/is_int_consistent_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/closure_oper.hpp"
    "${api_dir_root}/adl/oct/cpu/add_cons_close_oper.hpp"
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
    "${test_iapi_dir_root}/adl/assert.hpp"
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

    # unit:adl
    "${test_dir_unit}/adl/config.unit.cpp"
    "${test_dir_unit}/adl/intl.unit.cpp"
    "${test_dir_unit}/adl/error.unit.cpp"
    "${test_dir_unit}/adl/char_helper.unit.cpp"
    "${test_dir_unit}/adl/std/string_view.unit.cpp"
    "${test_dir_unit}/adl/timer.unit.cpp"

    # unit:adl/oct
    "${test_dir_unit}/adl/oct/limits.unit.cpp"
    "${test_dir_unit}/adl/oct/traits.unit.cpp"
    "${test_dir_unit}/adl/oct/var.unit.cpp"
    "${test_dir_unit}/adl/oct/vexpr.unit.cpp"
    "${test_dir_unit}/adl/oct/cons.unit.cpp"
    "${test_dir_unit}/adl/oct/system.unit.cpp"
    "${test_dir_unit}/adl/oct/oper.unit.cpp"

    # unit:adl/oct/dbm
    "${test_dir_unit}/adl/oct/dbm/dense_dbm.unit.cpp"

    # unit:adl/oct/cpu
    "${test_dir_unit}/adl/oct/cpu/seq_context.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/async_context.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/dense_dbm.unit.cpp"

    # unit:adl/oct/cpu
    "${test_dir_unit}/adl/oct/cpu/close_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/strong_close_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/strengthen_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/tighten_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/is_coherent_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/is_consistent_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/is_int_consistent_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/closure_oper.unit.cpp"
    "${test_dir_unit}/adl/oct/cpu/add_cons_close_oper.unit.cpp"
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
    add_custom_target(${PROJECT_NAME}-doc
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
