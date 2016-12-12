#include "catch.hpp"

#include "./oct_vexpr_base_.hpp"

#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

class oct_vexpr_test : public oct_vexpr_base_test_<oct_vexpr> {
public:
    using superclass = oct_vexpr_base_test_<oct_vexpr>;

    inline void test_all() {
        superclass::test_all();

        doubled();
    }

    inline void assumptions() {
        superclass::assumptions();

        REQUIRE( (std::is_constructible<oct_var>::value) );

        static_assert( (oct_vexpr(var_type()), true), "should be constexpr 1-var-constructible." );
    }

    inline void valid() {
        superclass::valid();

        SECTION( "Duplicate variables should make a vexpr invalid if they have different signs" ) {
            const var_type x1(10);
            const var_type x2(20);
            const oct_vexpr e1( x1, -x1);
            const oct_vexpr e2(-x1,  x1);
            const oct_vexpr e3( x1,  x1);
            const oct_vexpr e4(-x1, -x1);
            const oct_vexpr e5( x1,  x2);

            REQUIRE( (!e1.valid()) );
            REQUIRE( (!e2.valid()) );
            REQUIRE( ( e3.valid()) );
            REQUIRE( ( e4.valid()) );
            REQUIRE( ( e5.valid()) );
        }
    }

    inline void operators() {
        superclass::operators();

        SECTION( "operator-() should simplify the case xi - (-xj)" ) {
            oct_var xi(1), xj(-2);
            oct_vexpr e = xi - xj;

            REQUIRE( (e.valid()) );

            REQUIRE( (e.xi() == xi) );
            REQUIRE( (e.xj() == -xj) );
        }

        SECTION( "operator+() should maintain an eventual negative sign for the second term" ) {
            oct_var xi(1), xj(-2), xk(3);
            oct_vexpr e1 = xi + xj;
            oct_vexpr e2 = xi + xk;

            REQUIRE( (e1.valid()) );
            REQUIRE( (e2.valid()) );

            REQUIRE( (e1.xj().negated()) );
            REQUIRE( (e1.xi() == xi) );
            REQUIRE( (e1.xj() == xj) );

            REQUIRE( (!e2.xj().negated()) );
            REQUIRE( (e2.xi() == xi) );
            REQUIRE( (e2.xj() == xk) );
        }

        SECTION( "operator+() and operator-() should always keep the sign of the first operand" ) {
            constexpr oct_var xi(1), xj(-2);
            constexpr oct_vexpr e1 = xi + xj;
            constexpr oct_vexpr e2 = xj + xi;
            constexpr oct_vexpr e3 = xi - xj;
            constexpr oct_vexpr e4 = xj - xi;

            REQUIRE( (e1.valid()) );
            REQUIRE( (e2.valid()) );
            REQUIRE( (e3.valid()) );
            REQUIRE( (e4.valid()) );

            REQUIRE( (!e1.xi().negated()) );
            REQUIRE( ( e2.xi().negated()) );
            REQUIRE( (!e3.xi().negated()) );
            REQUIRE( ( e4.xi().negated()) );
        }
    }

    // ---


    inline void doubled() {

        SECTION( "doubled variables should be identified" ) {
            const var_type xi(10);
            const var_type xj(20);
            const oct_vexpr e1( xi,  xi);
            const oct_vexpr e2(-xi, -xi);
            const oct_vexpr e3( xi,  xj);

            REQUIRE( ( e1.doubled()) );
            REQUIRE( ( e2.doubled()) );
            REQUIRE( (!e3.doubled()) );
        }
    }
};


TEST_CASE( "oct_vexpr", "[oct][oct/vexpr]" ) {
    using namespace adl::test::oct;
    oct_vexpr_test test;
    test.test_all();
}

}}}
