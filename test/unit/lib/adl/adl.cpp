#include "catch.hpp"

#include "adl/adl.hpp"

TEST_CASE( "abs function", "[oct][oct/var][helper]" ) {
    using namespace adl;

    REQUIRE( modulus(0) == 0 );
    REQUIRE( modulus(-7) == 7 );
    REQUIRE( modulus(7) == 7 );
}


TEST_CASE( "is_neg function", "[oct][oct/var][helper]" ) {
    using namespace adl;

    REQUIRE( is_neg(0) == 0 );
    REQUIRE( is_neg(-7) == 1 );
    REQUIRE( is_neg(7) == 0 );
}
