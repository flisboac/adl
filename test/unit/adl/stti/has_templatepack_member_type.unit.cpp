// $flavio.lisboa @ 2018-02-20.
//
/*
 * @file has_templatepack_member_type.unit.cpp
 */

#include "./stti.unit.hpp"
#include "adl/stti/has_templatepack_member_type.hpp"

using namespace std::literals;
using namespace adl;

namespace {
    using tparam = int;
    using tparam2 = double;
    constexpr static const int tvalue = 0;

    adl_CREATE_HAS_TEMPLATEPACK_MEMBER_TYPE(name);
}

TEST_CASE("unit:adl/stti/has_templatepack_member_type.hpp", "") {
    // TODO Proper implementation

    SECTION("Test presence") {
        REQUIRE( ("name"s == has_templatepack_member_type_name<stti_test::type, tparam>::name) );

        REQUIRE( (!has_templatepack_member_type_name<stti_test::empty, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::type, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::template_type, tparam, tparam2>::detected) );
        REQUIRE( ( has_templatepack_member_type_name<stti_test::template_type, tparam>::detected) );
        REQUIRE( ( has_templatepack_member_type_name<stti_test::templatepack_type, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::templatenum_type, tparam, tparam2>::detected) );

        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_data, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_const_data, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_template_data, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_template_const_data, tparam, tparam2>::detected) );

        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_overloaded_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_template_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_template_overloaded_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::static_template_overloaded2_function, tparam, tparam2>::detected) );

        REQUIRE( (!has_templatepack_member_type_name<stti_test::data, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::const_data, tparam, tparam2>::detected) );

        REQUIRE( (!has_templatepack_member_type_name<stti_test::function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::overloaded_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::template_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::template_overloaded_function, tparam, tparam2>::detected) );
        REQUIRE( (!has_templatepack_member_type_name<stti_test::template_overloaded2_function, tparam, tparam2>::detected) );
    }
}


