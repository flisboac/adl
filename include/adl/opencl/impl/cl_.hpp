// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file cl.hpp
 */

#ifndef adl__opencl__impl__cl___hpp__
#define adl__opencl__impl__cl___hpp__

#ifndef CL_USE_DEPRECATED_OPENCL_1_2_APIS
#   define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif

#include "adl.cfg.hpp"

#include "CL/cl.h"

#if !defined(CL_HPP_TARGET_OPENCL_VERSION)
#   define CL_HPP_TARGET_OPENCL_VERSION 200
#else
    #if CL_HPP_TARGET_OPENCL_VERSION < 200
    #   error "Invalid OpenCL C++ header's target version; it must support at least OpenCL 2.0."
    #endif
#endif

#if defined(CL_HPP_NO_STD_STRING) || defined(CL_HPP_NO_STD_VECTOR) || defined(CL_HPP_NO_STD_ARRAY) || defined(CL_HPP_NO_STD_UNIQUE_PTR)
#   error "ADL is not compatible with non-standard string/vector/array/smart-pointer classes in the OpenCL C++ bindings."
#endif

#if adl_CONFIG_USE_OPENCL_CPP
#include "CL/cl2.hpp"
#endif

#if adl_CONFIG_USE_OPENCL_ERROR_CLASS
#   if !defined(CL_HPP_ENABLE_EXCEPTIONS)
#       error "OpenCL C++ bindinds' exceptions are not enabled. Be sure to activate it if you want ADL to use its Error class."
#   endif
#   define adl_OPENCL_ERROR ::cl::Error
#   define adl_MAKE_OPENCL_ERROR_(error_code__) ::cl::Error(error_code__.value(), error_code__.message())
#else
#   define adl_OPENCL_ERROR ::adl::cl_error
#   define adl_MAKE_OPENCL_ERROR_(error_code__) ::adl::opencl::error(error_code__)
#endif

#define adl_OPENCL_FN_BODY_V_(method, ...) \
    std::error_code code; \
    method(code, __VA_ARGS__); \
    if (code) { \
        throw adl_MAKE_OPENCL_ERROR_(code); \
    }

#define adl_OPENCL_EXPAND_(x) x

#define adl_OPENCL_FN_BODY_(method, ...)                    \
    std::error_code code;                                   \
    auto ret = method(code, __VA_ARGS__);                   \
    if (code)                                               \
    {                                                       \
        throw adl_MAKE_OPENCL_ERROR_(code);                 \
    }                                                       \
    return ret

#define adl_OPENCL_FN_BODY0_(method, ...)   \
    std::error_code code;                   \
    auto ret = method(code);                \
    if (code)                               \
    {                                       \
        throw adl_MAKE_OPENCL_ERROR_(code); \
    }                                       \
    return ret

#include "CL/cl.h"
#include "CL/cl2.hpp"

#endif // adl__opencl__impl__cl___hpp__
