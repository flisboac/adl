// $flavio.lisboa @ 2017-11-23.
//

#include "adl_catch.hpp"
#include "adl/oct/cpu/close_oper.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"
#include "adl/oct/cpu/seq_context.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

TEST_CASE("unit:adl/oct/cpu/close_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    auto xi = 1_ov;
    auto xj = 2_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    oct_system<double> system = {
        xi <= 3.0,
        xj <= 2.0,
        xi + xj <= 6.0,
        -xi - xj <= 5.0,
        -xi <= 3.0
    };
    auto diff_system = system.to_counterpart();

    REQUIRE( (diff_system.get( xdi, -xdi).c() ==  6.0) );
    REQUIRE( (diff_system.get( xdj, -xdj).c() ==  4.0) );
    REQUIRE( (diff_system.get( xdi, -xdj).c() ==  6.0) );
    REQUIRE( (diff_system.get( xdj, -xdi).c() ==  6.0) );
    REQUIRE( (diff_system.get(-xdi,  xdj).c() ==  5.0) );
    REQUIRE( (diff_system.get(-xdj,  xdi).c() ==  5.0) );
    REQUIRE( (diff_system.get(-xdi,  xdi).c() ==  6.0) );

    auto context = cpu::seq_context::make();
    auto dbm = context.make_dbm<cpu::dense_dbm>(diff_system, dbm_major::col);

    REQUIRE( (dbm.is_top( xdi,  xdi)) );
    REQUIRE( (dbm.at    ( xdi, -xdi) == 6.0) );
    REQUIRE( (dbm.is_top( xdi,  xdj)) );
    REQUIRE( (dbm.at    ( xdi, -xdj) == 6.0) );
    REQUIRE( (dbm.at    (-xdi,  xdi) == 6.0) );
    REQUIRE( (dbm.is_top(-xdi, -xdi)) );
    REQUIRE( (dbm.at    (-xdi,  xdj) == 5.0) );
    REQUIRE( (dbm.is_top(-xdi, -xdj)) );
    REQUIRE( (dbm.is_top( xdj,  xdi)) );
    REQUIRE( (dbm.at    ( xdj, -xdi) == 6.0) );
    REQUIRE( (dbm.is_top( xdj,  xdj)) );
    REQUIRE( (dbm.at    ( xdj, -xdj) == 4.0) );
    REQUIRE( (dbm.at    (-xdj,  xdi) == 5.0) );
    REQUIRE( (dbm.is_top(-xdj, -xdi)) );
    REQUIRE( (dbm.is_top(-xdj,  xdj)) );
    REQUIRE( (dbm.is_top(-xdj, -xdj)) );


    auto oper = context.make_oper<cpu::close_oper>(dbm);
    oper.get();

    for (auto k = dbm.first_var(); k < dbm.end_var(); k++) {
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto v_ik = dbm.at(i, k);

            for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                auto v_ij = dbm.at(i, j);
                auto v_kj = dbm.at(k, j);
                INFO( "k = " << k.id() << ", i = " << i.id() << ", j = " << j.id() );
                INFO( "v_ij = " << v_ij << ", v_ik = " << v_ik << ", v_kj = " << v_kj);
                REQUIRE( (v_ij <= v_ik + v_kj) );
            }
        }
    }
}
