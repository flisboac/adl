// $flisboac 2018-01-14

#include <cmath>

#include "adl_catch.hpp"
#include "adl/oct/cpu/closure_oper.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"
#include "adl/oct/cpu/seq_context.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

template <typename FloatType>
static void do_test() {

    using limits = constant_limits<FloatType>;

    auto xi = "x1"_ov;
    auto xj = "x2"_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    auto context = cpu::seq_context::make();
    auto dbm = context.make_dbm<cpu::dense_dbm, FloatType>(xj);

    dbm.assign({
        xi <= 3.0,
        xj <= 2.0,
        xi + xj <= 6.0,
        -xi - xj <= 5.0,
        -xi <= 3.0
    });

    auto queue = context.make_queue();
    auto closure = queue.make_oper<cpu::closure_oper>(dbm);
    auto satisfiable = closure.get();

    REQUIRE( (satisfiable) );
    // dbm.at(xi, -xi) = 3; // IT WORKS! \o/

    for (auto k = dbm.first_var(); k <= dbm.last_var(); k++) {
        REQUIRE( (dbm.at(k, k) == 0) ); // closure
        REQUIRE( (limits::is_pair(dbm.at(k, -k))) ); // tight closure

        for (auto i = dbm.first_var(); i <= dbm.last_var(); i++) {
            REQUIRE( (dbm.at(k, i) <= (dbm.at(k, -k) / 2) + (dbm.at(-i, i) / 2)) ); // strong closure

            for (auto j = dbm.first_var(); j <= dbm.last_var(); j++) {
                REQUIRE( (dbm.at(i, j) <= dbm.at(i, k) + dbm.at(k, j)) ); // closure
            }
        }
    }
}

TEST_CASE("unit:adl/oct/cpu/closure_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    do_test<int>();
    do_test<float>();
    do_test<float_int>();
}

