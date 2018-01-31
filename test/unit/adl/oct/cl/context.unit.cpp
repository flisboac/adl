// $flavio.lisboa @ 2018-01-26.
//
/*
 * @file context.unit.cpp
 */

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/cl/context.hpp"

using namespace adl;
using namespace adl::oct;

static ::cl_context create_cl_context_() {
    cl_platform_id platform_id = nullptr;
    cl_device_id device_id = nullptr;
    cl_context cl_ctx = nullptr;

    int ret = clGetPlatformIDs(1, &platform_id, nullptr);

    if (CL_SUCCESS == ret) {
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, nullptr);
    }

    if (CL_SUCCESS == ret) {
        cl_ctx = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);
    }

    if (CL_SUCCESS != ret) {
        FAIL("Failed to create OpenCL context!");
    }

    return cl_ctx;
}

TEST_CASE("unit:adl/oct/cl/context.hpp, Creation", "") {
    auto ctx = oct::cl::context::make();
}

TEST_CASE("unit:adl/oct/cl/context.hpp, Deletion", "") {
    auto cl_ctx = create_cl_context_();
    int refcount_before, refcount_after;
    ::clGetContextInfo(cl_ctx, CL_CONTEXT_REFERENCE_COUNT, 1, &refcount_before, nullptr);

    { auto ctx = oct::cl::context::make(cl_ctx); }

    ::clGetContextInfo(cl_ctx, CL_CONTEXT_REFERENCE_COUNT, 1, &refcount_after, nullptr);
    REQUIRE( (refcount_before == refcount_after) );
}
