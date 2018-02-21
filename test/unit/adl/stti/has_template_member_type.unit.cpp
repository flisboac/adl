// $flisboac 2018-02-15

#include "./stti.unit.hpp"
#include "adl/stti/has_template_member_type.hpp"

using namespace std::literals;
using namespace adl;

namespace {
    using tparam = int;
    constexpr static const int tvalue = 0;

    adl_CREATE_HAS_TEMPLATE_MEMBER_TYPE(name, ((typename), (int)));
}

TEST_CASE("unit:adl/stti/has_template_member_type_name.hpp", "") {
    // TODO Proper implementation

    SECTION("Test presence") {
        REQUIRE( ("name"s == has_template_member_type_name<stti_test::type, tparam, tvalue>::name) );

        REQUIRE( (!has_template_member_type_name<stti_test::empty, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::type, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::template_type, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::templatepack_type, tparam, tvalue>::detected) );
        REQUIRE( ( has_template_member_type_name<stti_test::templatenum_type, tparam, tvalue>::detected) );

        REQUIRE( (!has_template_member_type_name<stti_test::static_data, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_const_data, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_template_data, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_template_const_data, tparam, tvalue>::detected) );

        REQUIRE( (!has_template_member_type_name<stti_test::static_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_overloaded_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_template_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_template_overloaded_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::static_template_overloaded2_function, tparam, tvalue>::detected) );

        REQUIRE( (!has_template_member_type_name<stti_test::data, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::const_data, tparam, tvalue>::detected) );

        REQUIRE( (!has_template_member_type_name<stti_test::function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::overloaded_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::template_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::template_overloaded_function, tparam, tvalue>::detected) );
        REQUIRE( (!has_template_member_type_name<stti_test::template_overloaded2_function, tparam, tvalue>::detected) );
    }
}
