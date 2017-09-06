// $flisboac 2017-08-28
#include "adl_catch.hpp"
#include <iostream>
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/cpu/seq_context.hpp"
#include "adl/oct/cpu/async_context.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;


template <typename T>
static void test_dbm(T dbm) {
    INFO(dbm.size());
    REQUIRE( (dbm.size() == 200) );
    INFO(dbm.last_var().to_string());
    REQUIRE( ( dbm.last_var() == "x100"_ov ) );
}

static void test_with_fixed_size(cpu::seq_context & seq_ctx) {
    test_dbm(seq_ctx.make_dbm<cpu::dense_dbm, float>(100_ov));
    auto dbm = seq_ctx.make_dbm<cpu::dense_dbm, float>(100_ov, 10);
    test_dbm(dbm);
    //REQUIRE( (dbm[1_ov] == 10) );
}

static void test_with_octdiff_system(cpu::seq_context & seq_ctx) {
    oct_system<float> system;
    constexpr auto cons = 1_ov - 2_ov <= 10.2f;
    constexpr auto split = cons.split();
    system.insert(cons);
    auto diff_system = system.to_counterpart();
    //test_dbm(seq_ctx.make_dbm<cpu::dense_dbm, float>(diff_system));
    auto dbm = seq_ctx.make_dbm<cpu::dense_dbm, float>(diff_system, 10);
    //test_dbm(dbm);
    WARN(cons.to_vexpr().to_string() << ", " << split.to_string());
    WARN(dbm.at(cons.xi(), cons.xj()));
    WARN(dbm.at(split.di().to_identity_vexpr()));
    WARN(dbm.at(split.dj().to_identity_vexpr()));
    for (int i = 0; i < dbm.size() * dbm.size(); ++i) {
        if (i > 0 && i % dbm.size() == 0) std::cout << std::endl;
        std::cout << *(&dbm.constant_(0) + i) << " ";
    }
    REQUIRE( (dbm.at(cons.xi(), cons.xj()) == 10.2f) );
    REQUIRE( (dbm[1_ov] == 10) );
}

TEST_CASE("unit:adl/oct/cpu/dense_dbm.hpp", "[unit][adl][adl/oct][adl/oct/cpu]") {
    auto seq_ctx = cpu::seq_context::make();
    //test_with_fixed_size(seq_ctx);
    test_with_octdiff_system(seq_ctx);
}
