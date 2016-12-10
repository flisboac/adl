#include "./oct_var_base_.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

template <>
class oct_var_test<octdiff_var> : public base_oct_var_test<octdiff_var> {
public:
    using superclass = base_oct_var_test<octdiff_var>;

    inline void assumptions() {
        superclass::assumptions();

        static_assert( (octdiff_var().to_oct(), true), "to_oct() should be constexpr" );
        static_assert( ((oct_var) octdiff_var(), true), "explicit conversion to oct_var should be constexpr" );
        static_assert( (octdiff_var().same_oct_var(octdiff_var())), "same_oct_var() should be constexpr" );
        static_assert( (octdiff_var().negative(), true), "negative() should be constexpr" );
        static_assert( (octdiff_var().positive(), true), "positive() should be constexpr" );
        static_assert( (octdiff_var().swap(), true), "swap() should be constexpr" );
    }

    inline void test_all() {
        superclass::test_all();

        same_oct_var();
        positive_negative();
        swap();
        oct_var_cast();
    }

    inline void constructor() {
        superclass::constructor();

        SECTION( "octdiff_var: negative int construction is not accepted (undefined behaviour)" ) {
            const octdiff_var odv1(-1);

            REQUIRE( (odv1.to_int() != -1) );
        }
    }

    inline void assignment() {
        superclass::assignment();

        SECTION( "octdiff_var: negative int assignment is not accepted (undefined behaviour)" ) {
            octdiff_var odv2;
            odv2 = -2;

            REQUIRE( (odv2.to_int() != -2) );
        }
    }

    // ---

    inline void same_oct_var() {

        SECTION( "octdiff_var: same-oct-var comparison" ) {
            const octdiff_var ov1(1), ov2(2), ov3(3);

            REQUIRE( (ov1.same_oct_var(ov2)) );
            REQUIRE( (!ov2.same_oct_var(ov3)) );
        }
    }

    inline void positive_negative() {

        SECTION( "octdiff_var: positive and negative" ) {
            const octdiff_var odv1(1), odv2(2);

            REQUIRE( ( odv1.positive()) );
            REQUIRE( (!odv1.negative()) );
            REQUIRE( ( odv2.negative()) );
            REQUIRE( (!odv2.positive()) );
        }
    }

    inline void swap() {

        SECTION( "octdiff_var: swap" ) {
            const octdiff_var odv1(7), odv2(8);

            REQUIRE( (8 == (int) odv1.swap()) );
            REQUIRE( (7 == (int) odv2.swap()) );
        }
    }

    inline void oct_var_cast() {

        SECTION( "explicit cast to oct_var yields the same results as to_oct()" ) {
            const octdiff_var odv(1);

            REQUIRE( (odv.to_oct() == (oct_var) odv) );
        }

        SECTION( "conversion from octdiff_var to oct_var" ) {
            const octdiff_var odv1(1), odv2(2), odv3(5), odv4(8);
            const oct_var ov1 = odv1, ov2 = odv2, ov3 = odv3, ov4 = odv4;

            REQUIRE( ( 1 == odv1.to_oct().to_int()) );
            REQUIRE( (-1 == odv2.to_oct().to_int()) );
            REQUIRE( ( 3 == odv3.to_oct().to_int()) );
            REQUIRE( (-4 == odv4.to_oct().to_int()) );

            REQUIRE( (odv1.to_oct() == ov1 && ov1.to_diff() == odv1) );
            REQUIRE( (odv2.to_oct() == ov2 && ov2.to_diff() == odv2) );
            REQUIRE( (odv3.to_oct() == ov3 && ov3.to_diff() == odv3) );
            REQUIRE( (odv4.to_oct() == ov4 && ov4.to_diff() == odv4) );

            REQUIRE( (ov1.valid()) );
            REQUIRE( (ov2.valid()) );
            REQUIRE( (ov3.valid()) );
            REQUIRE( (ov4.valid()) );

            REQUIRE( ( 1 == (int) ov1) );
            REQUIRE( (-1 == (int) ov2) );
            REQUIRE( ( 3 == (int) ov3) );
            REQUIRE( (-4 == (int) ov4) );
        }
    }
};


TEST_CASE( "octdiff_var", "[oct][oct/var]" ) {

    oct_var_test<octdiff_var> od_test;
    od_test.test_all();
}

}}}
