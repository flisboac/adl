// $flisboac 2017-02-11
/**
 * @file macros.hpp
 */
#pragma once
#ifndef adl_catch__macros__hpp__
#define adl_catch__macros__hpp__

#include "catch.hpp"

#define REQUIRE_SECTION(test__, name__) SECTION( (name__) ) { REQUIRE( (test__) ); }
#define REQUIRE_SECTION_NOTHROW(test__, name__) SECTION( (name__) ) { REQUIRE_NOTHROW( (test__) ); }
#define REQUIRE_SECTION_THROWS_AS(test__, exception__, name__) SECTION( (name__) ) { REQUIRE_THROWS_AS( (test__), exception__ ); }

#endif //adl_catch__macros__hpp__
