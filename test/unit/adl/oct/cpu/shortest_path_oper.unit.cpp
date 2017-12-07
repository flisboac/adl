// $flavio.lisboa @ 2017-11-23.
//

#include "adl_catch.hpp"
#include "adl/oct/cpu/shortest_path_oper.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"
#include "adl/oct/cpu/seq_context.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

TEST_CASE("unit:adl/oct/cpu/shortest_path_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    auto xi = 1_ov;
    auto xj = 2_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    oct_system<double> system = {
        xi <= 3.0,
        xj <= 2.0,
        xi + xj <= 6.0,
        -xi - xj <= 5.0,
        -xj <= 3.0
    };

    auto context = cpu::seq_context::make();
    auto dbm = context.make_dbm<cpu::dense_dbm>(system.to_counterpart(), dbm_major::col);
    auto oper = context.make_oper<cpu::shortest_path_oper>(dbm);
    oper.get();

    REQUIRE( (dbm.at( xdi, -xdi) ==  6.0) );
    REQUIRE( (dbm.at( xdi,  xdj) == 11.0) );
    REQUIRE( (dbm.at( xdi, -xdi) ==  5.0) );
    REQUIRE( (dbm.at(-xdi,  xdi) ==  6.0) );
    REQUIRE( (dbm.at(-xdi,  xdj) ==  5.0) );
    REQUIRE( (dbm.at(-xdi, -xdj) ==  5.0) );
    REQUIRE( (dbm.at( xdj,  xdi) ==  5.0) );
    REQUIRE( (dbm.at( xdj, -xdi) ==  5.0) );
    REQUIRE( (dbm.at( xdj, -xdj) ==  4.0) );
    REQUIRE( (dbm.at(-xdj,  xdi) ==  5.0) );
    REQUIRE( (dbm.at(-xdj, -xdi) == 11.0) );
    REQUIRE( (dbm.at(-xdj,  xdj) == 16.0) );
}
