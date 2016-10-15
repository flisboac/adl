#include "catch.hpp"
#include "ukoct.hpp"
#include <type_traits>

TEST_CASE( "ukoct::abs function", "[oct][oct/var][helper]" ) {
	using namespace ukoct;

	REQUIRE( ::ukoct::abs(0) == 0 );
	REQUIRE( ::ukoct::abs(-7) == 7 );
	REQUIRE( ::ukoct::abs(7) == 7 );
}


TEST_CASE( "octdiff_var and oct_var", "[oct][oct/var]" ) {
	using namespace ukoct;

	REQUIRE( (std::is_default_constructible<oct_var>::value) );
	REQUIRE( (std::is_default_constructible<octdiff_var>::value) );
	REQUIRE( (std::is_constructible<oct_var, int>::value) );
	REQUIRE( (std::is_constructible<octdiff_var, int>::value) );
	REQUIRE( (std::is_copy_constructible<oct_var>::value) );
	REQUIRE( (std::is_copy_constructible<octdiff_var>::value) );
	REQUIRE( (std::is_move_constructible<oct_var>::value) );
	REQUIRE( (std::is_move_constructible<octdiff_var>::value) );
	REQUIRE( (std::is_copy_assignable<oct_var>::value) );
	REQUIRE( (std::is_copy_assignable<octdiff_var>::value) );
	REQUIRE( (std::is_move_assignable<oct_var>::value) );
	REQUIRE( (std::is_move_assignable<octdiff_var>::value) );
	REQUIRE( (std::is_convertible<oct_var, int>::value) );
	REQUIRE( (std::is_convertible<octdiff_var, int>::value) );
	REQUIRE( (std::is_convertible<int, oct_var>::value) );
	REQUIRE( (std::is_convertible<int, octdiff_var>::value) );
	static_assert( (oct_var(), true), "oct_var should be constexpr default-constructible." );
	static_assert( (octdiff_var(), true), "octdiff_var should be constexpr default-constructible." );
	static_assert( (oct_var(1), true), "oct_var should be constexpr int-constructible." );
	static_assert( (octdiff_var(1), true), "octdiff_var should be constexpr int-constructible." );
	// how to test constexpr-ness for (copy|move)-constructible and (copy|move)-assignable?

	SECTION( "default construction results in an invalid variable" ) {
		const oct_var ov;
		const octdiff_var odv;
		bool bv = ov;
		bool bdv = odv;
		REQUIRE( (!bv && bv == ov.valid() && bv == ov) );
		REQUIRE( (!bdv && bdv == odv.valid() && bdv == odv) );
	}

	SECTION( "default-constructed variable should be zero when converted to int" ) {
		const oct_var ov;
		const octdiff_var odv;
		int iv = ov;
		int idv = odv;
		REQUIRE( (iv == 0) );
		REQUIRE( (idv == 0) );
	}

	SECTION( "equality consistence" ) {
		const oct_var ov1, ov2(1), ov3(1);
		const octdiff_var odv1, odv2(1), odv3(1);

		REQUIRE( (ov1 == ov1 && !(ov1 != ov1)) );
		REQUIRE( (ov2 == ov2 && !(ov2 != ov2)) );
		REQUIRE( (ov2 == ov3 && !(ov2 != ov3)) );
		REQUIRE( (!(ov1 == ov2) && ov1 != ov2) );

		REQUIRE( (odv1 == odv1 && !(odv1 != odv1)) );
		REQUIRE( (odv2 == odv2 && !(odv2 != odv2)) );
		REQUIRE( (odv2 == odv3 && !(odv2 != odv3)) );
		REQUIRE( (!(odv1 == odv2) && odv1 != odv2) );
	}

	SECTION( "default-constructed variable should be equal to its type-based invalid value" ) {
		const oct_var ov;
		const octdiff_var odv;
		REQUIRE( (ov == oct_var::invalid()) );
		REQUIRE( (odv == octdiff_var::invalid()) );
	}

	SECTION( "int-construction consistent with its original int value" ) {
		const int iv = 1;
		const int idv = -2;
		oct_var ov(iv);
		octdiff_var odv(idv);
		int riv = ov;
		int ridv = odv;
		REQUIRE( (iv == riv && ov.valid() && !ov.negated()) );
		REQUIRE( (idv == ridv && odv.valid() && odv.negated()) );
		ov = idv;
		odv = iv;
		riv = ov;
		ridv = odv;
		REQUIRE( (idv == riv && ov.valid() && ov.negated()) );
		REQUIRE( (iv == ridv && ov.valid() && !odv.negated()) );
	}

	SECTION( "normalize consistence" ) {
		const oct_var ov1(1), ov2(-1);
		const octdiff_var odv1(1), odv2(-1);

		REQUIRE( (ov1.normalize() == ov1 && ov1.normalize() == ov2.normalize()) );
		REQUIRE( (odv1.normalize() == odv1 && odv1.normalize() == odv2.normalize()) );
	}

	SECTION( "index consistence" ) {
		const oct_var ov1, ov2(1), ov3(-2);
		const octdiff_var odv1, odv2(1), odv3(-2);

		REQUIRE( (ov1.index() != 0 && ov2.index() == 0 && ov3.index() == 1) );
		REQUIRE( (odv1.index() != 0 && odv2.index() == 0 && odv3.index() == 1) );
	}

	SECTION( "comparison consistence" ) {
		const oct_var ov1, ov2(1), ov3(-2);
		const octdiff_var odv1, odv2(1), odv3(-2);

		REQUIRE( (ov1.index() != 0 && ov2.index() == 0 && ov3.index() == 1) );
		REQUIRE( (odv1.index() != 0 && odv2.index() == 0 && odv3.index() == 1) );
	}

	SECTION( "unary + and -" ) {
		const oct_var ov1(1), ov2(-2);
		const octdiff_var odv1(1), odv2(-2);

		REQUIRE( (1 == +ov1 && !(+ov1).negated()) );
		REQUIRE( (-1 == -ov1 && (-ov1).negated()) );
		REQUIRE( (-2 == +ov2 && (+ov2).negated()) );
		REQUIRE( (2 == -ov2 && !(-ov2).negated()) );

		REQUIRE( (1 == +odv1 && !(+odv1).negated()) );
		REQUIRE( (-1 == -odv1 && (-odv1).negated()) );
		REQUIRE( (-2 == +odv2 && (+odv2).negated()) );
		REQUIRE( (2 == -odv2 && !(-odv2).negated()) );
	}

	SECTION( "increment" ) {
		oct_var ov1(1), ov2(-2);
		octdiff_var odv1(1), odv2(-2);

		REQUIRE( (1 == ov1++ && 2 == ov1) );
		REQUIRE( (++ov1 == ov1 && 3 == ov1) );
		REQUIRE( (-2 == ov2++ && -3 == ov2) );
		REQUIRE( (++ov2 == ov2 && -4 == ov2) );

		REQUIRE( (1 == odv1++ && 2 == odv1) );
		REQUIRE( (++odv1 == odv1 && 3 == odv1) );
		REQUIRE( (-2 == odv2++ && -3 == odv2) );
		REQUIRE( (++odv2 == odv2 && -4 == odv2) );
	}

	SECTION( "decrement" ) {
		oct_var ov1(10), ov2(-20);
		octdiff_var odv1(10), odv2(-20);

		REQUIRE( (10 == ov1-- && 9 == ov1) );
		REQUIRE( (--ov1 == ov1 && 8 == ov1) );
		REQUIRE( (-20 == ov2-- && -19 == ov2) );
		REQUIRE( (--ov2 == ov2 && -18 == ov2) );

		REQUIRE( (10 == odv1-- && 9 == odv1) );
		REQUIRE( (--odv1 == odv1 && 8 == odv1) );
		REQUIRE( (-20 == odv2-- && -19 == odv2) );
		REQUIRE( (--odv2 == odv2 && -18 == odv2) );
	}

	SECTION( "comparison" ) {
		const oct_var ov1(20), ov2(20), ov3(-20), ov4(10), ov5(-30);
		const octdiff_var odv1(20), odv2(20), odv3(-20), odv4(10), odv5(-30);

		REQUIRE( (!(ov1 < ov2) && !(ov2 < ov1) && ov1 == ov2 && ov1.compare(ov2) == 0) );
		REQUIRE( (  ov2 < ov3  && !(ov3 < ov2) && ov2 != ov3 && ov2.compare(ov3) < 0) );
		REQUIRE( (!(ov3 < ov4) &&   ov4 < ov3  && ov3 != ov4 && ov3.compare(ov4) > 0) );
		REQUIRE( (  ov3 < ov5  && !(ov5 < ov3) && ov3 != ov5 && ov3.compare(ov5) < 0) );

		REQUIRE( (!(odv1 < odv2) && !(odv2 < odv1) && odv1 == odv2 && odv1.compare(odv2) == 0) );
		REQUIRE( (  odv2 < odv3  && !(odv3 < odv2) && odv2 != odv3 && odv2.compare(odv3) < 0) );
		REQUIRE( (!(odv3 < odv4) &&   odv4 < odv3  && odv3 != odv4 && odv3.compare(odv4) > 0) );
		REQUIRE( (  odv3 < odv5  && !(odv5 < odv3) && odv3 != odv5 && odv3.compare(odv5) < 0) );
	}

	SECTION( "index" ) {
		const oct_var ov1(1), ov2(-2);
		const octdiff_var odv1(1), odv2(-2);

		REQUIRE( (ov1.index() == 0) );
		REQUIRE( (ov2.index() == 1) );

		REQUIRE( (odv1.index() == 0) );
		REQUIRE( (odv2.index() == 1) );
	}

	SECTION( "octdiff_var: positive and negative" ) {
		const octdiff_var odv1(-1), odv2(2);

		REQUIRE( ( odv1.positive()) );
		REQUIRE( (!odv1.negative()) );
		REQUIRE( ( odv2.negative()) );
		REQUIRE( (!odv2.positive()) );
	}

	SECTION( "octdiff_var: swap" ) {
		const octdiff_var odv1(-7), odv2(8);

		REQUIRE( (-8 == odv1.swap()) );
		REQUIRE( ( 7 == odv2.swap()) );
	}

	SECTION( "conversion from octdiff_var to oct_var" ) {
		const octdiff_var odv1(1), odv2(2), odv3(-5), odv4(8);
		const oct_var ov1 = odv1, ov2 = odv2, ov3 = odv3, ov4 = odv4;

		REQUIRE( (odv1.to_oct() == ov1) );
		REQUIRE( (ov2.to_diff() == odv2) );

		REQUIRE( (1 == ov1) );
		REQUIRE( (-1 == ov2) );
		REQUIRE( (3 == ov3) );
		REQUIRE( (-4 == ov4) );
	}

	SECTION( "conversion from oct_var to octdiff_var" ) {
		const oct_var ov1(1), ov2(-1), ov3(3), ov4(-4);
		const octdiff_var odv1 = ov1, odv2 = ov2, odv3 = ov3, odv4 = ov4;

		REQUIRE( (odv1.to_oct() == ov1) );
		REQUIRE( (ov2.to_diff() == odv2) );

		REQUIRE( (1 == odv1) );
		REQUIRE( (2 == odv2) );
		REQUIRE( (5 == odv3) );
		REQUIRE( (-8 == -odv4) );
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

TEST_CASE( "oct_cons and octdiff_cons", "[oct][oct/cons]" ) {

}
