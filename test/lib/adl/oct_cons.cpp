#include "adl/oct_cons.hpp"
#include "catch.hpp"
#include <type_traits>


template <template <typename> class C>
class base_cons_test_base {
public:
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
};


template <template <typename> class C>
class base_cons_test : public base_cons_test_base<C> {};


template <>
class base_cons_test<adl::oct_cons> : public base_cons_test_base<adl::oct_cons> {
public:
	inline void assumptions() {
		using namespace adl;
		base_cons_test_base<adl::oct_cons>::assumptions();

		REQUIRE( (std::is_constructible<oct_cons<int>, oct_var, int>::value) );
		REQUIRE( (std::is_constructible<oct_cons<int>, oct_var, oct_var, int>::value) );
	}
};


template <>
class base_cons_test<adl::octdiff_cons> : public base_cons_test_base<adl::octdiff_cons> {
public:
	inline void assumptions() {
		using namespace adl;
		base_cons_test_base<adl::octdiff_cons>::assumptions();

		REQUIRE( (std::is_constructible<oct_cons<int>, octdiff_var, octdiff_var, int>::value) );
	}
};


TEST_CASE( "oct_cons", "[oct][oct/cons]" ) {
	using namespace adl;

	base_cons_test<oct_cons> oct_cons_test;
	base_cons_test<octdiff_cons> octdiff_cons_test;

	oct_cons_test.assumptions();
	octdiff_cons_test.assumptions();
}
