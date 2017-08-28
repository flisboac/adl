// $flisboac 2017-08-28
#include "adl_catch.hpp"

#include "adl/oct/var.hpp"

#include "adl/oct/cpu/seq_context.hpp"
#include "adl/oct/cpu/async_context.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"


TEST_CASE("unit:adl/oct/cpu/dense_dbm.hpp", "[unit][adl][adl/oct][adl/oct/cpu]") {
    using namespace adl::oct::cpu;
    using namespace adl::literals;
    using namespace adl::operators;
    using namespace adl::dsl;
    auto seq_ctx = seq_context::make();
    auto dbm = seq_ctx.make_dbm<dense_dbm, float>(100_ov);
    INFO(dbm.size());
    REQUIRE( (dbm.size() == 200) );
    INFO(dbm.last_var().to_string());
    REQUIRE( ( dbm.last_var().equals(100_ov) ) );
}
