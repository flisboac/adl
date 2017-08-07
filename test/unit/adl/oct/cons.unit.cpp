// $flisboac 2017-06-15
//#include <iostream>

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/cons.hpp"

TEST_CASE("unit:adl/oct/cons") {
    using namespace adl::literals; // For "x1"_ol
    using namespace adl::operators; // For common operators (not part of a DSL, e.g. x1 <= x2)
    using namespace adl::dsl; // for x1 - x2
    using namespace adl::oct; // For type definitions

    auto x1 = 1_ov;
    auto x2 = "x2"_ov;
    auto x3 = octdiff_var(3);
    auto vexpr2 = make_sub_vexpr(x3, x2);
    auto comp = x1 == x2;
    auto vexpr = x3 + x1;
    auto cons = make_cons(vexpr, 10.0);
    //std::cout << x3 << " (oct: " << ((oct_var) x3) << ")" << std::endl << vexpr << std::endl << cons << std::endl << cons.split() << std::endl;

    // TODO Tests (remember to remove all the commented ios stuff)
}
