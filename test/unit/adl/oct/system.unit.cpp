// $flisboac 2017-06-26
#include <iostream>

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"





adl::oct::oct_system<double> make_system() {
    using namespace adl::oct;
    using namespace adl::literals;
    using namespace adl::operators;
    using namespace adl::dsl;

    auto xi = "x1"_ov;
    auto xj = "x2"_ov;
    auto cons = -xi - xj <= 10.1;
    basic_oct_vexpr<oct_lvar> vexpr = cons;
    auto split = cons.split();
    auto converted_cons = split.to_oct();
    INFO(split.to_string() << " -> {" << cons.to_string() << "} == {" << converted_cons.to_string() << "}");
    oct_system<double> system;
    // CLion doesn't like SFINAE
    system.insert(xi - xj <= 10.2);
    system.insert(xi <= 20.2);
    if (xi <= xj) {
        system.insert(xj >= 30);
    }
    INFO("count(xj) = " << system.vars().get(xj).count());
    INFO("x1 - x2 <= " << system[vexpr]);
    return system;
}

TEST_CASE("unit:adl/oct/system") {

    auto system = make_system();
    INFO(system.to_string());
    auto diff_system = system.to_counterpart();
    INFO("diff_system = " << diff_system.to_string());
}
