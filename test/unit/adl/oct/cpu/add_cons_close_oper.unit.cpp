// $flisboac 2018-01-14

#include "adl_catch.hpp"
#include "adl/oct/cpu/closure_oper.hpp"
#include "adl/oct/cpu/add_cons_close_oper.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"
#include "adl/oct/cpu/seq_context.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

template <typename DbmType, typename OperType>
static void add_cons(char const* type_name, DbmType& dbm, OperType&& oper) {
    using limits = typename DbmType::constant_limits;
    using constant_type = typename DbmType::constant_type;

    auto cons_close_sat = oper.get();

    INFO("type_name = " << type_name << ", dbm = " << dbm.to_string());
    REQUIRE((cons_close_sat));

    for (auto k = dbm.first_var(); k <= dbm.last_var(); k++) {
        INFO(limits::raw_value(dbm.at(k, k))); REQUIRE( (dbm.at(k, k) == 0) ); // closure
        INFO(limits::raw_value(dbm.at(k, -k))); REQUIRE( (limits::is_even(dbm.at(k, -k))) ); // tight closure (unary constraints, all even)

        for (auto i = dbm.first_var(); i <= dbm.last_var(); i++) {
            INFO("dbm.at(k, i) = " << limits::raw_value(dbm.at(k, i)));
            INFO("dbm.at(k, -k) = " << limits::raw_value(dbm.at(k, -k)));
            INFO("dbm.at(-i, i) = " << limits::raw_value(dbm.at(-i, i)));
            REQUIRE( (dbm.at(k, i) <= (dbm.at(k, -k) / 2) + (dbm.at(-i, i) / 2)) ); // strong closure

            for (auto j = dbm.first_var(); j <= dbm.last_var(); j++) {
                REQUIRE( (dbm.at(i, j) <= dbm.at(i, k) + dbm.at(k, j)) ); // closure
            }
        }
    }
};

template <typename FloatType>
static void do_test(char const* type_name) {

    using limits = constant_limits<FloatType>;

    auto xi = 1_ov;
    auto xj = 2_ov;
    auto xdi = xi.to_counterpart();
    auto xdj = xj.to_counterpart();

    auto context = cpu::seq_context::make();
    auto dbm = context.make_dbm<cpu::dense_dbm, FloatType>(xj);

    dbm.assign({
        xi <= FloatType(3.0),
        xj <= FloatType(2.0),
        xi + xj <= FloatType(6.0)
    });

    auto queue = context.make_queue();
    auto closure = queue.make_oper<cpu::closure_oper>(dbm);
    REQUIRE( (closure.get()) );

    add_cons( type_name, dbm, queue.make_oper<cpu::add_cons_close_oper>(dbm, -xi <= FloatType(3.0)) );
    add_cons( type_name, dbm, queue.make_oper<cpu::add_cons_close_oper>(dbm, -xi - xj <= FloatType(5.0)) );
}

TEST_CASE("unit:adl/oct/cpu/add_cons_close_oper.hpp", "[unit][oper][adl][adl/oct][adl/oct/cpu]") {

    //do_test<int>("int"); // TBD
    do_test<float>("float");
    do_test<double>("double");
    do_test<long double>("long double");
    do_test<float_int>("float_int");
    do_test<double_int>("double_int");
    do_test<ldouble_int>("ldouble_int");
}
