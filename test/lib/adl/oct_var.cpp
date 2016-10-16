#include <adl/oct_var.hpp>
#include "catch.hpp"
#include <type_traits>


template <typename T>
class base_oct_test {
public:
	virtual ~base_oct_test() {}

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

	virtual inline void sections() const {
		using namespace adl;

		SECTION( "default construction results in an invalid variable" ) {
			const T ov;
			REQUIRE( (!ov.valid() && !ov) );
		}

		SECTION( "default-constructed variable should be zero when converted to int" ) {
			const T ov;
			REQUIRE( (ov == 0 && ov.to_int() == 0) );
		}

		SECTION( "equality consistence" ) {
			const T ov1, ov2(1), ov3(1);

			REQUIRE( (ov1 == ov1 && !(ov1 != ov1)) );
			REQUIRE( (ov2 == ov2 && !(ov2 != ov2)) );
			REQUIRE( (ov2 == ov3 && !(ov2 != ov3)) );
			REQUIRE( (!(ov1 == ov2) && ov1 != ov2) );
		}

		SECTION( "default-constructed variable should be equal to its type-based invalid value" ) {
			const T ov;

			REQUIRE( (ov == oct_var::invalid()) );
		}

		SECTION( "int-construction consistent with its original int value" ) {
			int iv = 1;
			T ov(iv);
			int riv = ov.to_int();

			REQUIRE( (iv == riv) );
			REQUIRE( (ov.valid()) );
			REQUIRE( (!ov.negated()) );

			iv = -2;
			ov = iv;
			riv = (int) ov;
			REQUIRE( (iv == riv) );
			REQUIRE( (ov.valid()) );
			REQUIRE( (ov.negated()) );
		}


		SECTION( "normalize consistence" ) {
			const T ov1(1), ov2(-1);

			REQUIRE( (ov1.normalize() == ov1) );
			REQUIRE( (ov1.normalize() == ov2.normalize() ));
		}

		SECTION( "index consistence" ) {
			const T ov2(1), ov3(-2);

			REQUIRE( (ov2.index() == 0) );
			REQUIRE( (ov3.index() == 1) );
		}

		SECTION( "comparison consistence" ) {
			const T ov1, ov2(1), ov3(-2);

			REQUIRE( (ov1.index() != 0) );
			REQUIRE( (ov2.index() == 0) );
			REQUIRE( (ov3.index() == 1) );
		}

		SECTION( "unary + and -" ) {
			const T ov1(1), ov2(-2);

			REQUIRE( (1 == (int) +ov1 && !(+ov1).negated()) );
			REQUIRE( (-1 == (int) -ov1 && (-ov1).negated()) );
			REQUIRE( (-2 == (int) +ov2 && (+ov2).negated()) );
			REQUIRE( (2 == (int) -ov2 && !(-ov2).negated()) );
		}

		SECTION( "increment" ) {
			T ov1(1), ov2(-2);

			REQUIRE( (1 == (int) ov1++ && 2 == (int) ov1) );
			REQUIRE( (++ov1 == ov1 && 3 == (int) ov1) );
			REQUIRE( (-2 == (int) ov2++ && -3 == (int) ov2) );
			REQUIRE( (++ov2 == ov2 && -4 == (int) ov2) );
		}

		SECTION( "decrement" ) {
			T ov1(10), ov2(-20);

			REQUIRE( (10 == (int) ov1-- && 9 == (int) ov1) );
			REQUIRE( (--ov1 == ov1 && 8 == (int) ov1) );
			REQUIRE( (-20 == (int) ov2-- && -19 == (int) ov2) );
			REQUIRE( (--ov2 == ov2 && -18 == (int) ov2) );
		}

		SECTION( "comparison" ) {
			const T ov1(20), ov2(20), ov3(-20), ov4(10), ov5(-30);

			REQUIRE( (!(ov1 < ov2) && !(ov2 < ov1) && ov1 == ov2 && ov1.compare(ov2) == 0) );
			REQUIRE( (  ov2 < ov3  && !(ov3 < ov2) && ov2 != ov3 && ov2.compare(ov3) < 0) );
			REQUIRE( (!(ov3 < ov4) &&   ov4 < ov3  && ov3 != ov4 && ov3.compare(ov4) > 0) );
			REQUIRE( (  ov3 < ov5  && !(ov5 < ov3) && ov3 != ov5 && ov3.compare(ov5) < 0) );
		}

		SECTION( "index" ) {
			const T ov1(1), ov2(-2);

			REQUIRE( (ov1.index() == 0) );
			REQUIRE( (ov2.index() == 1) );
			REQUIRE( ((size_t) ov1 == ov1.index()) );
			REQUIRE( ((size_t) ov2 == ov2.index()) );
		}
	}
};


template <>
class base_oct_test<adl::octdiff_var> : public base_oct_test<adl::oct_var> {
public:

	inline void sections() const {
		using namespace adl;
		base_oct_test<oct_var>::sections();

		SECTION( "octdiff_var: positive and negative" ) {
			const octdiff_var odv1(-1), odv2(2);

			REQUIRE( ( odv1.positive()) );
			REQUIRE( (!odv1.negative()) );
			REQUIRE( ( odv2.negative()) );
			REQUIRE( (!odv2.positive()) );
		}

		SECTION( "octdiff_var: swap" ) {
			const octdiff_var odv1(-7), odv2(8);

			REQUIRE( (-8 == (int) odv1.swap()) );
			REQUIRE( ( 7 == (int) odv2.swap()) );
		}
	}
};

TEST_CASE( "octdiff_var and oct_var", "[oct][oct/var]" ) {
	using namespace adl;

	base_oct_test<oct_var> oct_var_test;
	base_oct_test<octdiff_var> octdiff_var_test;

	oct_var_test.assumptions();
	octdiff_var_test.assumptions();
	oct_var_test.sections();
	octdiff_var_test.sections();


	SECTION( "conversion from octdiff_var to oct_var" ) {
		const octdiff_var odv1(1), odv2(2), odv3(-5), odv4(8);
		const oct_var ov1 = odv1, ov2 = odv2, ov3 = odv3, ov4 = odv4;

		REQUIRE( (odv1.to_oct() == ov1) );
		REQUIRE( (ov2.to_diff() == odv2) );

		REQUIRE( (1 == (int) ov1) );
		REQUIRE( (-1 == (int) ov2) );
		REQUIRE( (3 == (int) ov3) );
		REQUIRE( (-4 == (int) ov4) );
	}

	SECTION( "conversion from oct_var to octdiff_var" ) {
		const oct_var ov1(1), ov2(-1), ov3(3), ov4(-4);
		const octdiff_var odv1 = ov1, odv2 = ov2, odv3 = ov3, odv4 = ov4;

		REQUIRE( (odv1.to_oct() == ov1) );
		REQUIRE( (ov2.to_diff() == odv2) );

		REQUIRE( (1 == (int) odv1) );
		REQUIRE( (2 == (int) odv2) );
		REQUIRE( (5 == (int) odv3) );
		REQUIRE( (-8 == (int) -odv4) );
	}

	SECTION( "comparisons between oct_var and octdiff_var" ) {
		const octdiff_var odv1(1), odv2(2), odv3(-3);
		const oct_var ov1(1), ov2(-1), ov3(2);

		REQUIRE( (odv1 == ov1  && odv1 != ov2  && odv1 != ov3 ) );
		REQUIRE( (ov1  == odv1 && ov2  != odv1 && ov3  != odv1) );

		REQUIRE( (odv2 == ov2  && odv2 != ov1  && odv2 != ov3 ) );
		REQUIRE( (ov2  == odv2 && ov1  != odv2 && ov3  != odv2) );

		REQUIRE( (odv3 == -ov3.to_diff()  && -ov3.to_diff() == odv3) );
	}
}

