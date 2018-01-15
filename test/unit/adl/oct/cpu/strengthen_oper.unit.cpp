// $flisboac 2018-01-14

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

TEST_CASE("unit:adl/oct/cpu/strengthen_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    auto xi = 1_ov;
    auto xj = 2_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    // TODO Implementation
}
