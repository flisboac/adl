// $flisboac 2018-01-14

#include "adl_catch.hpp"
#include "adl/oct/cpu/strengthen_oper.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"
#include "adl/oct/cpu/seq_context.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

TEST_CASE("unit:adl/oct/cpu/strengthen_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    auto xi = 1_ov;
    auto xj = 2_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    auto context = cpu::seq_context::make();
    auto dbm = context.make_dbm<cpu::dense_dbm, double>(-xdj);

    dbm.at( xdi,  xdi) = 11;
    dbm.at( xdi, -xdi) = 6;
    dbm.at( xdi,  xdj) = 11;
    dbm.at( xdi, -xdj) = 6;
    dbm.at(-xdi,  xdi) = 6;
    dbm.at(-xdi, -xdi) = 11;
    dbm.at(-xdi,  xdj) = 5;
    dbm.at(-xdi, -xdj) = 9;
    dbm.at( xdj,  xdi) = 9;
    dbm.at( xdj, -xdi) = 6;
    dbm.at( xdj,  xdj) = 11;
    dbm.at( xdj, -xdj) = 4;
    dbm.at(-xdj,  xdi) = 5;
    dbm.at(-xdj, -xdi) = 11;
    dbm.at(-xdj,  xdj) = 16;
    dbm.at(-xdj, -xdj) = 11;

    auto oper = context.make_oper<cpu::strengthen_oper>(dbm);
    oper.get();

    for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
        for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
            auto const v_ij = dbm.at(i, j);
            auto const v_iI = dbm.at(i, -i);
            auto const v_Jj = dbm.at(-j, j);
            auto const v_alt = (v_iI + v_Jj) / 2;
            INFO( "i = " << i.id() << ", j = " << j.id() );
            INFO( "v_ij = " << v_ij << ", v_iI = " << v_iI << ", v_Jj = " << v_Jj );
            REQUIRE( (v_ij <= v_alt) );
        }
    }

    // FIXME Maybe not needed...
    INFO( (dbm.at( xdi,  xdi)) ); REQUIRE( (dbm.at( xdi,  xdi) == 6) );
    INFO( (dbm.at( xdi, -xdi)) ); REQUIRE( (dbm.at( xdi, -xdi) == 6) );
    INFO( (dbm.at( xdi,  xdj)) ); REQUIRE( (dbm.at( xdi,  xdj) == 11) );
    INFO( (dbm.at( xdi, -xdj)) ); REQUIRE( (dbm.at( xdi, -xdj) == 5) );
    INFO( (dbm.at(-xdi,  xdi)) ); REQUIRE( (dbm.at(-xdi,  xdi) == 6) );
    INFO( (dbm.at(-xdi, -xdi)) ); REQUIRE( (dbm.at(-xdi, -xdi) == 6) );
    INFO( (dbm.at(-xdi,  xdj)) ); REQUIRE( (dbm.at(-xdi,  xdj) == 5) );
    INFO( (dbm.at(-xdi, -xdj)) ); REQUIRE( (dbm.at(-xdi, -xdj) == 5) );
    INFO( (dbm.at( xdj,  xdi)) ); REQUIRE( (dbm.at( xdj,  xdi) == 5) );
    INFO( (dbm.at( xdj, -xdi)) ); REQUIRE( (dbm.at( xdj, -xdi) == 5) );
    INFO( (dbm.at( xdj,  xdj)) ); REQUIRE( (dbm.at( xdj,  xdj) == 10) );
    INFO( (dbm.at( xdj, -xdj)) ); REQUIRE( (dbm.at( xdj, -xdj) == 4) );
    INFO( (dbm.at(-xdj,  xdi)) ); REQUIRE( (dbm.at(-xdj,  xdi) == 5) );
    INFO( (dbm.at(-xdj, -xdi)) ); REQUIRE( (dbm.at(-xdj, -xdi) == 11) );
    INFO( (dbm.at(-xdj,  xdj)) ); REQUIRE( (dbm.at(-xdj,  xdj) == 16) );
    INFO( (dbm.at(-xdj, -xdj)) ); REQUIRE( (dbm.at(-xdj, -xdj) == 10) );
}
