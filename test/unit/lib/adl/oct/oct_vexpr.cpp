#include "catch.hpp"

#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr.hpp"
#include "adl/oct/oct_cons.hpp"

// TODO unit tests for {oct,octdiff}_vexpr

static void test() {
    using namespace adl::oct;

    oct_var x1(1);
    oct_var x2(2);

    auto c1 = x1 <= 0.1;
    auto c2 = x1 + x2 <= 10;
    auto c3 = x1 - x2 <= 10;
    auto c4 = -x1 - x2 <= 10;

}
