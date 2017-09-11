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
    auto xk = "x3000"_ov;
    auto dxi = xi.to_counterpart();
    auto dxj = xj.to_counterpart();
    auto dxk = xk.to_counterpart();

    oct_system<double> system = {
        xi + xj <= 5.3,    // xi+ - xj- <=  5.3 && xj+ - xi- <= 5.3
        xi - xj <= 10.2,   // xi+ - xj+ <=  10.2 && xj- - xi- <= 10.2
        -xi - xj <= 10.4,  // xi- - xj+ <=  10.4 && xj- - xi+ <= 10.4
        xi <= 20.2,        // xi+ - xi- <=  40.4
        xi >= 10.1,        // xi- - xi+ <=  20.2
        xj >= 30.0,        // xj- - xj+ <= -60.0
        -xk <= 200.0       // xk- - xk+ <= 400.0
    };

    INFO( "system = " << system.to_string() );

    // CLion doesn't like SFINAE
    REQUIRE( (system.vars().count_all(xi) == 5) );
    REQUIRE( (system.vars().count(-xi) == 2) );
    REQUIRE( (system.vars().count(xi) == 3) );

    REQUIRE( (system.vars().at(-xj).count() == 4) );
    REQUIRE( (system.vars().at(xj).negative_count() == 3) );
    REQUIRE( (system.vars().at(-xj).positive_count() == 1) );

    REQUIRE( (system.vars().at(xk).count() == 1) );
    REQUIRE( (system.vars().at(-xk).negative_count() == 1) );
    REQUIRE( (system.vars().at(xk).positive_count() == 0) );

    REQUIRE( (system[xi + xj] == 5.3) );
    REQUIRE( (system[xi - xj] == 10.2) );
    REQUIRE( (system[-xi - xj] == 10.4) );
    REQUIRE( (system[xi] == 20.2) );
    REQUIRE( (system[-xi] == -10.1) );
    REQUIRE_THROWS ( (system[xj]) );
    REQUIRE( (system[-xj] == -30.0) );
    REQUIRE_THROWS ( (system[xk]) );
    REQUIRE( (system[-xk] == 200.0) );

    auto diff_system = system.to_counterpart();
    INFO( "diff_system = " << diff_system.to_string() );

    REQUIRE( (diff_system.vars().count_all(xi) == 10) );
    REQUIRE( (diff_system.vars().count(-xi) == 5) );
    REQUIRE( (diff_system.vars().count(xi) == 5) );

    REQUIRE( (diff_system.vars().at(-xj).count() == 8) );
    REQUIRE( (diff_system.vars().at(xj).negative_count() == 4) );
    REQUIRE( (diff_system.vars().at(-xj).positive_count() == 4) );

    REQUIRE( (diff_system.vars().at(xk).count() == 2) );
    REQUIRE( (diff_system.vars().at(-xk).negative_count() == 1) );
    REQUIRE( (diff_system.vars().at(xk).positive_count() == 1) );

    // xi + xj <= 5.3   -> xi+ - xj- <=  5.3 && xj+ - xi- <= 5.3
    auto split_c1 = (xi + xj <= 5.3).split();
    REQUIRE( (diff_system[split_c1.di()] == split_c1.di().c()) );
    REQUIRE( (diff_system[split_c1.dj()] == split_c1.dj().c()) );

    // xi - xj <= 10.2 -> xi+ - xj+ <=  10.2 && xj- - xi- <= 10.2
    REQUIRE( (diff_system[octdiff_vexpr( dxi,  dxj)] == 10.2) );
    REQUIRE( (diff_system[octdiff_vexpr(-dxj, -dxi)] == 10.2) );

    // -xi - xj <= 10.4 -> xi- - xj+ <=  10.4 && xj- - xi+ <= 10.4
    //REQUIRE( (diff_system[-dxi - dxj] == 10.2) );
    //REQUIRE( (diff_system[-dxj - dxi] == 10.2) );

    // xi <= 20.2,        -> xi+ - xi- <=  40.4
    // xi >= 10.1,        -> xi- - xi+ <=  20.2
    // xj >= 30.0,        -> xj- - xj+ <= -60.0
    // -xk <= 200.0       -> xk- - xk+ <= 400.0

    return system;
}

TEST_CASE("unit:adl/oct/system") {

    auto system = make_system();
    INFO(system.to_string());
    auto diff_system = system.to_counterpart();
    INFO("diff_system = " << diff_system.to_string());
}
