// $flisboac 2017-06-15
//#include <iostream>

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/cons.hpp"

#define SHOW_CONS_INFO_(cons) INFO( #cons \
        << " valid: " << cons.valid() \
        << ", xi: " << cons.xi().to_string() \
        << ", xj: " << cons.xj().to_string() \
        << ", c: " << cons.c() )

TEST_CASE("unit:adl/oct/cons") {
    using namespace adl::literals; // For "x1"_ol
    using namespace adl::operators; // For common operators (not part of a DSL, e.g. x1 <= x2)
    using namespace adl::dsl; // for x1 - x2
    using namespace adl::oct; // For type definitions

    auto xi = "x1"_ov;
    auto xj = "x2"_ov;
    auto dxi = xi.to_counterpart();
    auto dxj = xj.to_counterpart();
    auto c = 10.1;
    auto c2 = c * 2;

    SECTION("Correctness of variable conversion") {
        REQUIRE( ( xi.id() == 1 &&  (-xi).id() == -1) );
        REQUIRE( ( xj.id() == 2 &&  (-xj).id() == -2) );
        REQUIRE( (dxi.id() == 1 && (-dxi).id() == 2) );
        REQUIRE( (dxj.id() == 3 && (-dxj).id() == 4) );
    }

    SECTION("Binary constraints: xi + xj") {
        auto cons = xi + xj <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == ( dxi) && di.xj() == (-dxj) && di.c() == c) );
        SHOW_CONS_INFO_(dj); REQUIRE( (dj.valid() && dj.xi() == ( dxj) && dj.xj() == (-dxi) && dj.c() == c) );
    }

    SECTION("Binary constraints: xi - xj") {
        auto cons = xi - xj <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == ( dxi) && di.xj() == ( dxj) && di.c() == c) );
        SHOW_CONS_INFO_(dj); REQUIRE( (dj.valid() && dj.xi() == (-dxj) && dj.xj() == (-dxi) && dj.c() == c) );
    }

    SECTION("Binary constraints: -xi - xj") {
        auto cons = -xi - xj <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == (-dxi) && di.xj() == ( dxj) && di.c() == c) );
        SHOW_CONS_INFO_(dj); REQUIRE( (dj.valid() && dj.xi() == (-dxj) && dj.xj() == ( dxi) && dj.c() == c) );
    }

    SECTION("Binary constraints: -xi + xj = xj - xi") {
        auto cons = -xi + xj <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == (-dxi) && di.xj() == (-dxj) && di.c() == c) );
        SHOW_CONS_INFO_(dj); REQUIRE( (dj.valid() && dj.xi() == ( dxj) && dj.xj() == ( dxi) && dj.c() == c) );
    }

    SECTION("Binary constraints: xi <= c") {
        auto cons = xi <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == ( dxi) && di.xj() == (-dxi) && di.c() == c2) );
        SHOW_CONS_INFO_(dj); REQUIRE( (!dj.valid()) );
    }

    SECTION("Binary constraints: -xi <= c") {
        auto cons = -xi <= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == (-dxi) && di.xj() == ( dxi) && di.c() == c2) );
        SHOW_CONS_INFO_(dj); REQUIRE( (!dj.valid()) );
    }

    SECTION("Binary constraints: -xi >= c -> xi <= -c") {
        auto cons = -xi >= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == ( dxi) && di.xj() == (-dxi) && di.c() == -c2) );
        SHOW_CONS_INFO_(dj); REQUIRE( (!dj.valid()) );
    }

    SECTION("Binary constraints: xi >= c -> -xi <= -c") {
        auto cons = xi >= c;
        auto split = cons.split();
        auto di = split.di(), dj = split.dj();
        SHOW_CONS_INFO_(di); REQUIRE( (di.valid() && di.xi() == (-dxi) && di.xj() == ( dxi) && di.c() == -c2) );
        SHOW_CONS_INFO_(dj); REQUIRE( (!dj.valid()) );
    }

}
