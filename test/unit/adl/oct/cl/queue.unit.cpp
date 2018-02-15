// $flavio.lisboa @ 2018-01-26.
//
/*
 * @file queue.unit.cpp
 */

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/cl/queue.hpp"

using namespace adl;
using namespace adl::oct;

TEST_CASE("unit:adl/oct/cl/queue.hpp", "") {
    auto ctx = oct::cl::context::make();
    //auto queue = ctx->make_queue<oct::cl::queue_cl1>();
    //REQUIRE( (queue->profiling_enabled()) );
}
