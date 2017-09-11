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

#define INFO_REQUIRE_MAXSIZE__ 8
#define INFO_REQUIRE_SIZE_N__(x,x8,x7,x6,x5,x4,x3,x2,x1,xn,...) xn
#define INFO_REQUIRE_SIZE__(...) INFO_REQUIRE_SIZE_N__(0,__VA_ARGS__,8,7,6,5,4,3,2,1,0)
#define INFO_REQUIRE_BASE_NAME__ scopedMessage
#define INFO_REQUIRE_UNIQUE_NAME_LINE2( name, line, argn ) name##line##_##argn
#define INFO_REQUIRE_UNIQUE_NAME_LINE( name, line, argn ) INFO_REQUIRE_UNIQUE_NAME_LINE2( name, line, argn )
#define INFO_REQUIRE_CATCH_INFO__( argn, log ) Catch::ScopedMessage INFO_REQUIRE_UNIQUE_NAME_LINE( INFO_REQUIRE_BASE_NAME__, __LINE__, argn ) = Catch::MessageBuilder( "INFO", CATCH_INTERNAL_LINEINFO, Catch::ResultWas::Info ) << log;
#define INFO_REQUIRE_INFO__(argn__, obj__) INFO_REQUIRE_CATCH_INFO__(argn__, #obj__ << " = " << (obj__))
#define INFO_REQUIRE_INFO__1(obj__)      INFO_REQUIRE_INFO__(1, obj__)
#define INFO_REQUIRE_INFO__2(obj__, ...) INFO_REQUIRE_INFO__(2, obj__); INFO_REQUIRE_INFO__1(__VA_ARGS__)
#define INFO_REQUIRE_INFO__3(obj__, ...) INFO_REQUIRE_INFO__(3, obj__); INFO_REQUIRE_INFO__2(__VA_ARGS__)
#define INFO_REQUIRE_INFO__4(obj__, ...) INFO_REQUIRE_INFO__(4, obj__); INFO_REQUIRE_INFO__3(__VA_ARGS__)
#define INFO_REQUIRE_INFO__5(obj__, ...) INFO_REQUIRE_INFO__(5, obj__); INFO_REQUIRE_INFO__4(__VA_ARGS__)
#define INFO_REQUIRE_INFO__6(obj__, ...) INFO_REQUIRE_INFO__(6, obj__); INFO_REQUIRE_INFO__5(__VA_ARGS__)
#define INFO_REQUIRE_INFO__7(obj__, ...) INFO_REQUIRE_INFO__(7, obj__); INFO_REQUIRE_INFO__6(__VA_ARGS__)
#define INFO_REQUIRE_INFO__8(obj__, ...) INFO_REQUIRE_INFO__(8, obj__); INFO_REQUIRE_INFO__7(__VA_ARGS__)
#define INFO_REQUIRE_INFO__N(n__, ...) INFO_REQUIRE_INFO__ ## n__ (__VA_ARGS__)
#define INFO_REQUIRE_INFO_N__(n__, ...) INFO_REQUIRE_INFO__N(n__, __VA_ARGS__)
#define INFO_REQUIRE(test__, ...) INFO_REQUIRE_INFO_N__(INFO_REQUIRE_SIZE__(__VA_ARGS__), __VA_ARGS__); REQUIRE(test__)

#endif //adl_catch__macros__hpp__
