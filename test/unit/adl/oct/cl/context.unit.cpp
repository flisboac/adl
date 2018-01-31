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

TEST_CASE("unit:adl/oct/cl/context.hpp", "") {
    oct::cl::context ctx = oct::cl::context::make();
}
