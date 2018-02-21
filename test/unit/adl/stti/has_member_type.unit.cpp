// $flisboac 2018-02-15

#include <string>

#include "./stti.unit.hpp"
#include "adl/stti/has_member_type.hpp"

using namespace std::literals;
using namespace adl;

namespace {
    using tparam = int;
    constexpr static const int tvalue = 0;

    adl_CREATE_HAS_MEMBER_TYPE(name);
}

TEST_CASE("unit:adl/stti/has_member_type.hpp", "") {
    // TODO Proper implementation

    SECTION("Test presence") {
        REQUIRE( ("name"s == has_member_type_name<stti_test::type>::name) );

        REQUIRE( (!has_member_type_name<stti_test::empty>::detected) );
        REQUIRE( ( has_member_type_name<stti_test::type>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::template_type>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::templatepack_type>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::templatenum_type>::detected) );

        REQUIRE( (!has_member_type_name<stti_test::static_data>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_const_data>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_template_data>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_template_const_data>::detected) );

        REQUIRE( (!has_member_type_name<stti_test::static_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_overloaded_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_template_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_template_overloaded_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::static_template_overloaded2_function>::detected) );

        REQUIRE( (!has_member_type_name<stti_test::data>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::const_data>::detected) );

        REQUIRE( (!has_member_type_name<stti_test::function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::overloaded_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::template_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::template_overloaded_function>::detected) );
        REQUIRE( (!has_member_type_name<stti_test::template_overloaded2_function>::detected) );
    }
}
