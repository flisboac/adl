#include "adl/oct_var.hpp"
#include "adl/oct_cons.hpp"
#include "catch.hpp"
#include <type_traits>


template <template <typename> class C, typename V>
class base_cons_test_base {
public:
    using var_t = V;
	typedef C<int> IntCons;
	typedef C<float> FloatCons;

	virtual ~base_cons_test_base() {}
	virtual inline void assumptions() {

		REQUIRE( (std::is_default_constructible<IntCons>::value) );
		REQUIRE( (std::is_copy_constructible<IntCons>::value) );
		REQUIRE( (std::is_move_constructible<IntCons>::value) );
		REQUIRE( (std::is_copy_assignable<IntCons>::value) );
		REQUIRE( (std::is_move_assignable<IntCons>::value) );

		// These are actually checked at compile-time, but no one needs to know :)
		static_assert( (IntCons(), true), "cons should be constexpr default-constructible." );
		static_assert( ((bool) IntCons(), true), "cons should be constexpr-explicitly bool-convertible." );
		static_assert( (!IntCons(), true), "cons::operator! should be constexpr." );
		static_assert( (IntCons().valid(), true), "cons::valid() should be constexpr." );
	}

	virtual inline void sections() {
		using namespace adl;
		using namespace adl::literals;

		SECTION( "getters and setters" ) {
			IntCons c1(var_t(1), var_t(2), 3);

			REQUIRE( (var_t(1) == c1.xi()) );
			REQUIRE( (var_t(2) == c1.xj()) );
			REQUIRE( (3 == c1.c()) );

			REQUIRE( (&(c1.xi(var_t(2))) == &c1 && var_t(2) == c1.xj()) );
			REQUIRE( (&(c1.xj(var_t(3))) == &c1 && var_t(3) == c1.xj()) );
			REQUIRE( (&(c1.c(4)) == &c1 && 4 == (int) c1.c()) );
		}
	}
};


template <template <typename> class C, typename T = adl::oct_var>
class base_cons_test : public base_cons_test_base<C, T> {};


template <>
class base_cons_test<adl::oct_cons, adl::oct_var> : public base_cons_test_base<adl::oct_cons, adl::oct_var> {
public:
    using superclass = base_cons_test_base<adl::oct_cons, adl::oct_var>;

	inline void assumptions() {
		using namespace adl;
		superclass::assumptions();

		REQUIRE( (std::is_constructible<IntCons, var_t, int>::value) );
		REQUIRE( (std::is_constructible<IntCons, var_t, var_t, int>::value) );
	}

	inline void sections() {
		using namespace adl;
		using namespace adl::literals;
	}
};


template <>
class base_cons_test<adl::octdiff_cons, adl::octdiff_var> : public base_cons_test_base<adl::octdiff_cons, adl::octdiff_var> {
public:
    using superclass = base_cons_test_base<adl::octdiff_cons, adl::octdiff_var>;

	inline void assumptions() {
		using namespace adl;
		superclass::assumptions();

		REQUIRE( (std::is_constructible<oct_cons<int>, var_t, var_t, int>::value) );
	}

	inline void sections() {
		using namespace adl;

		SECTION( "swapped difference variables' getters" ) {
			var_t x(1), y(2);
			IntCons c1(x, y, 0);

			REQUIRE( (c1.xI() == x.swap()) );
			REQUIRE( (c1.xJ() == y.swap()) );
		}
	}
};


TEST_CASE( "oct_cons", "[oct][oct/cons]" ) {
	using namespace adl;

	base_cons_test<oct_cons> oct_cons_test;
	base_cons_test<octdiff_cons> octdiff_cons_test;

	oct_cons_test.assumptions();
	octdiff_cons_test.assumptions();

	oct_cons_test.sections();
	octdiff_cons_test.sections();
}
