// $flisboac 2017-02-11
/**
 * @file macros.hpp
 */
#pragma once
#ifndef adl_catch__macros__hpp__
#define adl_catch__macros__hpp__

#include "catch.hpp"

#define adl_UNIT_TEST_NAME(module_name__, element_type__) "unit:" module_name__ "#" adl_STRFY(element_type__) // element can be a C++ type, as long as it's not parameterized (template)
#define adl_UNIT_TEST_STRNAME(module_name__, element_name__) "unit:" module_name__ "#" element_name__ // Element must be a string.
#define adl_UNIT_TEST_FLAGS(module_name__) "[unit][" module_name__ "]"

#define REQUIRE_TRUE_NOTHROW(test__) REQUIRE_NOTHROW( REQUIRE( (test__) ) )

#define REQUIRE_SECTION(test__, name__) SECTION( (name__) ) { REQUIRE( (test__) ); }
#define REQUIRE_SECTION_NOTHROW(test__, name__) SECTION( (name__) ) { REQUIRE_TRUE_NOTHROW(test__); }
#define REQUIRE_SECTION_THROWS_AS(test__, exception__, name__) SECTION( (name__) ) { REQUIRE_THROWS_AS( REQUIRE( (test__) ), exception__ ); }

#endif //adl_catch__macros__hpp__
