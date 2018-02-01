// $flavio.lisboa @ 2018-01-26.
//
/*
 * @file context.unit.cpp
 */

#include <future>

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

static void acquire_context(std::shared_ptr<oct::cl::context> ctx) {}
static void acquire_context(std::shared_ptr<oct::cl::context const> ctx) {}

TEST_CASE("unit:adl/oct/cl/context.hpp, Creation", "") {
    using namespace std::chrono_literals;

    auto ctx = oct::cl::context::make();
    ::cl_uint refcount_before = 0, refcount_after = (::cl_uint) -1;
    ::clGetContextInfo(ctx->underlying_context(), CL_CONTEXT_REFERENCE_COUNT, sizeof(::cl_uint), &refcount_before, nullptr);

    acquire_context(ctx->const_ptr());
    auto wait_future = { // Not that it makes me sleep better at night, but anyways, here goes nothing...
        std::async(std::launch::async, [=] () {
            auto this_ctx = ctx->const_ptr();
            std::this_thread::sleep_for(1ns);
        }),
        std::async(std::launch::async, [=] () {
            auto this_ctx = ctx->ptr();
            std::this_thread::sleep_for(1ns);
        }),
        std::async(std::launch::async, [=] () {
            auto this_ctx = ctx->ptr();
            std::this_thread::sleep_for(1us);
        })
    };
    acquire_context(ctx->ptr());

    for (auto& f : wait_future) f.wait();

    ::clGetContextInfo(ctx->underlying_context(), CL_CONTEXT_REFERENCE_COUNT, sizeof(::cl_uint), &refcount_after, nullptr);
    CAPTURE(refcount_before);
    CAPTURE(refcount_after);
    REQUIRE( (refcount_before == refcount_after) );
    //REQUIRE( (refcount_after == 1) ); // Does it count usage across processes? Can I really take this for granted??
}

TEST_CASE("unit:adl/oct/cl/context.hpp, Deletion", "") {
    auto cl_ctx = create_cl_context_();
    ::cl_uint refcount_before = 0, refcount_after = (::cl_uint) -1;
    ::clGetContextInfo(cl_ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(::cl_uint), &refcount_before, nullptr);

    {
        auto ctx = oct::cl::context::make(cl_ctx);
        acquire_context(ctx->const_ptr());
        acquire_context(ctx->ptr());
    }

    ::clGetContextInfo(cl_ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(::cl_uint), &refcount_after, nullptr);
    CAPTURE(refcount_before);
    CAPTURE(refcount_after);
    REQUIRE( (refcount_before == refcount_after) );
    //REQUIRE( (refcount_after == 1) ); // Does it count usage across processes? Can I really take this for granted??
}
