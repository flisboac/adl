#include <adl/oct_var.hpp>
#include "catch.hpp"
#include <type_traits>
#include <utility>

namespace adl { namespace test {

template <typename T>
class base_oct_var_test {
public:
	virtual ~base_oct_var_test() {}

    virtual inline void test_all() {

        assumptions();
        constructor();
        assignment();
        valid();
        equality();
        comparison();
        index();
        increment();
    }

	virtual inline void assumptions() {

		REQUIRE( (std::is_default_constructible<T>::value) );
		REQUIRE( (std::is_constructible<T, int>::value) );
		REQUIRE( (std::is_copy_constructible<T>::value) );
		REQUIRE( (std::is_move_constructible<T>::value) );
		REQUIRE( (std::is_copy_assignable<T>::value) );
		REQUIRE( (std::is_move_assignable<T>::value) );

		// These are actually checked at compile-time, but no one needs to know :)
		static_assert( (T(), true), "var should be constexpr default-constructible." );
		static_assert( (T(1), true), "var should be constexpr int-constructible." );
		static_assert( ((int) T(), true), "var should be constexpr-explicitly int-convertible." );
		static_assert( ((bool) T(), true), "var should be constexpr-explicitly bool-convertible." );
		static_assert( ((size_t) T(), true), "var should be constexpr-explicitly size_t-convertible." );
		static_assert( (T().to_int(), true), "var::to_int() should be constexpr." );
		static_assert( (T().valid(), true), "var::valid() should be constexpr." );
		static_assert( (T().index(), true), "var::index() should be constexpr." );
	}

    virtual inline void constructor() {
		using namespace adl;

		SECTION( "default-constructed variable should be zero when converted to int" ) {
			const T ov;

			REQUIRE( ((int) ov == 0 && ov.to_int() == 0) );
		}

		SECTION( "default-constructed variable should be equal to its type-based invalid value" ) {
			const T ov;

			REQUIRE( (ov == T::invalid()) );
		}

		SECTION( "int-initialization must be consistent" ) {
            const int iv1 = 1;
			const T ov1(iv1);

			REQUIRE( (iv1 == (int) ov1 && iv1 == ov1.to_int()) );
		}
    }

    virtual inline void assignment() {

		SECTION( "int-assignment must be consistent" ) {
            const int iv2 = 2;
            T ov2;
            ov2 = iv2;

			REQUIRE( (iv2 == (int) ov2 && iv2 == ov2.to_int()) );
        }
    }


    virtual inline void valid() {

		SECTION( "default construction results in an invalid variable" ) {
			const T ov;

			REQUIRE( (!ov.valid() && !ov && !((bool) ov)) ); // method, operator and explicit cast notations
		}

        SECTION( "zero-initialized variable must be invalid" ) {
            const T ov2(0);

            REQUIRE( (!ov2.valid() && !ov2 && !((bool) ov2)) );
        }

		SECTION( "the type-based invalid value must be invalid" ) {
			const T ov;

			REQUIRE( (!T::invalid().valid()) );
		}
    }

    virtual inline void equality() {

		SECTION( "copy-initialization must be consistent" ) {
			const T ov1(1);
            const T ov2(ov1);

			REQUIRE( ((int) ov1 == (int) ov2 && !((int) ov1 != (int) ov2)) );
			REQUIRE( (ov1 == ov2 && !(ov1 != ov2)) );
		}

		SECTION( "copy-assignment must be consistent" ) {
			const T ov1(1);
            T ov2;
            ov2 = ov1;

			REQUIRE( ((int) ov1 == (int) ov2 && !((int) ov1 != (int) ov2)) );
			REQUIRE( (ov1 == ov2 && !(ov1 != ov2)) );
		}

		SECTION( "move-initialization must be consistent" ) {
            const int iv = 1;
			const T ov1(std::move(T(iv)));

			REQUIRE( ((int) ov1 == iv) );
		}

		SECTION( "move-assignment must be consistent" ) {
            const int iv = 1;
			T ov1;
            ov1 = std::move(T(iv));

			REQUIRE( ((int) ov1 == iv) );
		}

		SECTION( "generic equality consistence test here!" ) {
			const T ov1, ov2(1), ov3(1);

			REQUIRE( (ov1 == ov1 && !(ov1 != ov1)) );
			REQUIRE( (ov2 == ov2 && !(ov2 != ov2)) );
			REQUIRE( (ov2 == ov3 && !(ov2 != ov3)) );
			REQUIRE( (!(ov1 == ov2) && ov1 != ov2) );
		}

		SECTION( "zero-assignment must make the variable invalid" ) {
            T ov2;
            ov2 = 0;

			REQUIRE( (0 == ov2.to_int() && ov2 == T::invalid()) );
        }
    }


    virtual inline void comparison() {

		SECTION( "generic positive-int comparison consistency check" ) {
			const T ov1(20), ov2(20), ov3(30), ov4(10);

			REQUIRE( (!(ov1 < ov2) && !(ov2 < ov1) && ov1 == ov2 && ov1.compare(ov2) == 0) );
			REQUIRE( (  ov2 < ov3  && !(ov3 < ov2) && ov2 != ov3 && ov2.compare(ov3) < 0) );
			REQUIRE( (!(ov3 < ov4) &&   ov4 < ov3  && ov3 != ov4 && ov3.compare(ov4) > 0) );
		}
    }


    virtual inline void index() {

		SECTION( "index must equal value minus 1" ) {
			const T ov2(1), ov3(2);

			REQUIRE( (ov2.index() == 0) );
			REQUIRE( (ov3.index() == 1) );
			REQUIRE( ((size_t) ov2 == ov2.index()) );
			REQUIRE( ((size_t) ov3 == ov3.index()) );
		}

		SECTION( "index of an invalid variable must not be zero" ) {
			const T ov1;

			REQUIRE( (ov1.index() != 0) );
		}
    }

    virtual inline void increment() {

		SECTION( "increment of a var must follow the normal int increment rules" ) {
			T ov1(1);

			REQUIRE( (1 == (int) ov1++ && 2 == (int) ov1) );
			REQUIRE( (++ov1 == ov1 && 3 == (int) ov1) );
		}

		SECTION( "increment of zero (invalid variable) should yield an invalid variable" ) {
			oct_var ov1, ov2;

			REQUIRE( (0 == (int) ov1++ && 0 == (int) ov1 && !ov1.valid()) );
			REQUIRE( (0 == (int) --ov2 && !ov2.valid()) );
		}

		SECTION( "decrement of a var must follow the normal int decrement rules" ) {
			T ov1(10);

			REQUIRE( (10 == (int) ov1-- && 9 == (int) ov1) );
			REQUIRE( (--ov1 == ov1 && 8 == (int) ov1) );
		}

		SECTION( "decrement of zero (invalid variable) should yield an invalid variable" ) {
			oct_var ov1, ov2;

			REQUIRE( (0 == (int) ov1-- && 0 == (int) ov1 && !ov1.valid()) );
			REQUIRE( (0 == (int) --ov2 && !ov2.valid()) );
		}
    }

	virtual inline void sections() const {

	}
};


template <typename T>
class oct_var_test : public base_oct_var_test<T> {};

template <>
class oct_var_test<adl::oct_var> : public base_oct_var_test<adl::oct_var> {
public:
    using superclass = base_oct_var_test<adl::oct_var>;

    inline void test_all() {
        using namespace adl;
        superclass::test_all();

        negated();
        unary_plus_minus();
        normalize();
        same_var();
        octdiff_var_cast();
    }

    inline void constructor() {
        using namespace adl;
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
        using namespace adl;
        superclass::assignment();

		SECTION( "oct_var: negative int-assignment must be consistent" ) {
            const int iv2 = -2;
            oct_var ov2;
            ov2 = iv2;

			REQUIRE( (iv2 == (int) ov2 && iv2 == ov2.to_int()) );
        }
    }

    virtual inline void valid() {
        using namespace adl;
        superclass::valid();

        SECTION( "oct_var: negated variables are valid" ) {
            const oct_var ov(-101);

            REQUIRE( (ov.valid()) );
        }
    }

    virtual inline void equality() {
        using namespace adl;
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
        using namespace adl;
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
        using namespace adl;
        superclass::index();

		SECTION( "index must be normalized value - 1" ) {
			const oct_var ov2(-1), ov3(-2);

			REQUIRE( (ov2.index() == 0) );
			REQUIRE( (ov3.index() == 1) );
		}
    }

    inline void increment() {
        using namespace adl;
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
        using namespace adl;

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
        using namespace adl;

		SECTION( "normalize consistence" ) {
			const oct_var ov1(1), ov2(-1);

			REQUIRE( (ov1.normalize() == ov1) );
			REQUIRE( (ov1.normalize() == ov2.normalize() ));
		}
    }

    inline void same_var() {
        using namespace adl;

		SECTION( "same-var comparison" ) {
			const oct_var ov1(2), ov2(1), ov3(-1), ov4(-1);

			REQUIRE( (!ov1.same_var(ov2) && !ov2.same_var(ov1)) );
			REQUIRE( (ov2.same_var(ov3) && ov3.same_var(ov2)) );
			REQUIRE( (ov3.same_var(ov4) && ov4.same_var(ov3)) );
		}
    }

    inline void octdiff_var_cast() {
        using namespace adl;

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

template <>
class oct_var_test<adl::octdiff_var> : public base_oct_var_test<adl::octdiff_var> {
public:
    using superclass = base_oct_var_test<adl::octdiff_var>;

    inline void test_all() {
        using namespace adl;
        superclass::test_all();

        same_oct_var();
        positive_negative();
        swap();
        oct_var_cast();
    }

    inline void constructor() {
        using namespace adl;
        superclass::constructor();

        SECTION( "octdiff_var: negative int construction is not accepted (undefined behaviour)" ) {
			const octdiff_var odv1(-1);

            REQUIRE( (odv1.to_int() != -1) );
        }
    }

    inline void assignment() {
        using namespace adl;
        superclass::assignment();

        SECTION( "octdiff_var: negative int assignment is not accepted (undefined behaviour)" ) {
            octdiff_var odv2;
            odv2 = -2;

            REQUIRE( (odv2.to_int() != -2) );
        }
    }

    // ---

    inline void same_oct_var() {
        using namespace adl;

		SECTION( "octdiff_var: same-oct-var comparison" ) {
			const octdiff_var ov1(1), ov2(2), ov3(3);

			REQUIRE( (ov1.same_oct_var(ov2)) );
			REQUIRE( (!ov2.same_oct_var(ov3)) );
		}
    }

    inline void positive_negative() {
        using namespace adl;

		SECTION( "octdiff_var: positive and negative" ) {
			const octdiff_var odv1(1), odv2(2);

			REQUIRE( ( odv1.positive()) );
			REQUIRE( (!odv1.negative()) );
			REQUIRE( ( odv2.negative()) );
			REQUIRE( (!odv2.positive()) );
		}
    }

    inline void swap() {
        using namespace adl;

		SECTION( "octdiff_var: swap" ) {
			const octdiff_var odv1(7), odv2(8);

			REQUIRE( (8 == (int) odv1.swap()) );
			REQUIRE( (7 == (int) odv2.swap()) );
		}
    }

    inline void oct_var_cast() {
        using namespace adl;

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

TEST_CASE( "octdiff_var and oct_var", "[oct][oct/var]" ) {
	using namespace adl;

	oct_var_test<oct_var> ov_test;
	oct_var_test<octdiff_var> od_test;

	ov_test.test_all();
	od_test.test_all();


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

}}
