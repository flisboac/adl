#include "./oct_var_base_.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

template <>
class oct_var_test<oct_var> : public base_oct_var_test<oct_var> {
public:
    using superclass = base_oct_var_test<oct_var>;

    inline void test_all() {
        superclass::test_all();

        negated();
        unary_plus_minus();
        normalize();
        same_var();
        octdiff_var_cast();
    }

    inline void constructor() {
        superclass::constructor();

		SECTION( "oct_var: negative int-construction consistent with its original int value" ) {
			const int iv = -11;
			oct_var ov(iv);
			int riv = ov.to_int();

			REQUIRE( (iv == riv) );
			REQUIRE( (ov.valid()) );
		}
    }

    virtual inline void assignment() {
        superclass::assignment();

		SECTION( "oct_var: negative int-assignment must be consistent" ) {
            const int iv2 = -2;
            oct_var ov2;
            ov2 = iv2;

			REQUIRE( (iv2 == (int) ov2 && iv2 == ov2.to_int()) );
        }
    }

    virtual inline void valid() {
        superclass::valid();

        SECTION( "oct_var: negated variables are valid" ) {
            const oct_var ov(-101);

            REQUIRE( (ov.valid()) );
        }
    }

    virtual inline void equality() {
        superclass::equality();

        SECTION( "oct_var: equality consistence for negated values" ) {
            const oct_var ov1(-1), ov2(-12), ov3(-12);

			REQUIRE( (ov1 == ov1 && !(ov1 != ov1)) );
			REQUIRE( (ov2 == ov2 && !(ov2 != ov2)) );
			REQUIRE( (ov2 == ov3 && !(ov2 != ov3)) );
			REQUIRE( (!(ov1 == ov2) && ov1 != ov2) );
        }
    }

    inline void comparison() {
        superclass::comparison();

		SECTION( "oct_var: negated variables must be compared greater than their absolute value-equivalent" ) {
			const oct_var ov1(20), ov2(20), ov3(-20), ov4(10), ov5(-30);

			REQUIRE( (!(ov1 < ov2) && !(ov2 < ov1) && ov1 == ov2 && ov1.compare(ov2) == 0) );
			REQUIRE( (  ov2 < ov3  && !(ov3 < ov2) && ov2 != ov3 && ov2.compare(ov3) < 0) );
			REQUIRE( (!(ov3 < ov4) &&   ov4 < ov3  && ov3 != ov4 && ov3.compare(ov4) > 0) );
			REQUIRE( (  ov3 < ov5  && !(ov5 < ov3) && ov3 != ov5 && ov3.compare(ov5) < 0) );
		}
    }

    inline void index() {
        superclass::index();

		SECTION( "index must be normalized value - 1" ) {
			const oct_var ov2(-1), ov3(-2);

			REQUIRE( (ov2.index() == 0) );
			REQUIRE( (ov3.index() == 1) );
		}
    }

    inline void increment() {
        superclass::increment();

		SECTION( "oct_var: increment of a negated var must follow positive int increment rules while still negated" ) {
			oct_var ov2(-2);

			REQUIRE( (-2 == (int) ov2++ && -3 == (int) ov2) );
			REQUIRE( (++ov2 == ov2 && -4 == (int) ov2) );
		}

		SECTION( "oct_var: decrement of a negated var must follow positive int decrement rules while still negated" ) {
			oct_var ov2(-20);

			REQUIRE( (-20 == (int) ov2-- && -19 == (int) ov2) );
			REQUIRE( (--ov2 == ov2 && -18 == (int) ov2) );
		}
    }

    // ---

    inline void negated() {

        SECTION( "oct_var: negated() must be consistent" ) {
            const oct_var ov1(-1), ov2(2);

			REQUIRE( (ov1.negated()) );
            REQUIRE( (!ov2.negated()) );
        }
    }

    inline void unary_plus_minus() {
        using namespace adl;

		SECTION( "unary + and - consistency" ) {
			const oct_var ov1(1), ov2(-2);

			REQUIRE( (1 == (int) +ov1 && !(+ov1).negated()) );
			REQUIRE( (-1 == (int) -ov1 && (-ov1).negated()) );
			REQUIRE( (-2 == (int) +ov2 && (+ov2).negated()) );
			REQUIRE( (2 == (int) -ov2 && !(-ov2).negated()) );
		}
    }

    inline void normalize() {

		SECTION( "normalize consistence" ) {
			const oct_var ov1(1), ov2(-1);

			REQUIRE( (ov1.normalize() == ov1) );
			REQUIRE( (ov1.normalize() == ov2.normalize() ));
		}
    }

    inline void same_var() {

		SECTION( "same-var comparison" ) {
			const oct_var ov1(2), ov2(1), ov3(-1), ov4(-1);

			REQUIRE( (!ov1.same_var(ov2) && !ov2.same_var(ov1)) );
			REQUIRE( (ov2.same_var(ov3) && ov3.same_var(ov2)) );
			REQUIRE( (ov3.same_var(ov4) && ov4.same_var(ov3)) );
		}
    }

    inline void octdiff_var_cast() {

        SECTION( "valid conversions from oct_var to octdiff_var" ) {
            const oct_var ov1(1), ov2(-1), ov3(3), ov4(-4);
            const octdiff_var odv1 = ov1, odv2 = ov2, odv3 = ov3, odv4 = ov4;

            REQUIRE( (1 == ov1.to_diff().to_int()) );
            REQUIRE( (2 == ov2.to_diff().to_int()) );
            REQUIRE( (5 == ov3.to_diff().to_int()) );
            REQUIRE( (8 == ov4.to_diff().to_int()) );

            REQUIRE( (odv1.to_oct() == ov1 && ov1.to_diff() == odv1) );
            REQUIRE( (odv2.to_oct() == ov2 && ov2.to_diff() == odv2) );
            REQUIRE( (odv3.to_oct() == ov3 && ov3.to_diff() == odv3) );
            REQUIRE( (odv4.to_oct() == ov4 && ov4.to_diff() == odv4) );

            REQUIRE( (odv1.valid()) );
            REQUIRE( (odv2.valid()) );
            REQUIRE( (odv3.valid()) );
            REQUIRE( (odv4.valid()) );

            REQUIRE( (1 == (int) odv1) );
            REQUIRE( (2 == (int) odv2) );
            REQUIRE( (5 == (int) odv3) );
            REQUIRE( (8 == (int) odv4) );
        }

        SECTION( "invalid oct_var should convert into an invalid octdiff_var" ) {
            const oct_var ov = oct_var::invalid();
            const octdiff_var dv = ov;

            REQUIRE( (!dv.valid()) );
        }
    }
};

TEST_CASE( "oct_var", "[oct][oct/var]" ) {

	oct_var_test<oct_var> ov_test;
	ov_test.test_all();

	SECTION( "comparisons between oct_var and octdiff_var" ) {
		const octdiff_var odv1(1), odv2(2), odv3(3), odv4(4);
		const oct_var ov1(1), ov2(-1), ov3(2);

		REQUIRE( (odv1 == ov1  && odv1 != ov2  && odv1 != ov3 ) );
		REQUIRE( (ov1  == odv1 && ov2  != odv1 && ov3  != odv1) );

		REQUIRE( (odv2 == ov2  && odv2 != ov1  && odv2 != ov3 ) );
		REQUIRE( (ov2  == odv2 && ov1  != odv2 && ov3  != odv2) );

		REQUIRE( (odv3 == ov3.to_diff()) );
        REQUIRE( ((-ov3).to_diff() == odv4) );
	}
}

TEST_CASE( "octdiff_var and oct_var interoperability", "[oct][oct/var]" ) {

	SECTION( "comparisons between oct_var and octdiff_var" ) {
		const octdiff_var odv1(1), odv2(2), odv3(3), odv4(4);
		const oct_var ov1(1), ov2(-1), ov3(2);

		REQUIRE( (odv1 == ov1  && odv1 != ov2  && odv1 != ov3 ) );
		REQUIRE( (ov1  == odv1 && ov2  != odv1 && ov3  != odv1) );

		REQUIRE( (odv2 == ov2  && odv2 != ov1  && odv2 != ov3 ) );
		REQUIRE( (ov2  == odv2 && ov1  != odv2 && ov3  != odv2) );

		REQUIRE( (odv3 == ov3.to_diff()) );
        REQUIRE( ((-ov3).to_diff() == odv4) );
	}
}

}}}
