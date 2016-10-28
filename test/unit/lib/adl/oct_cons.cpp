#include "adl/oct_var.hpp"
#include "adl/oct_cons.hpp"
#include "catch.hpp"
#include <type_traits>

namespace adl { namespace test {

template <class C>
class base_cons_test_base {
public:
	typedef C cons_type;
    using value_type = typename cons_type::value_type;
    using var_type = typename cons_type::var_type;

	virtual ~base_cons_test_base() {}


    virtual inline void test_all() {

        assumptions();
        constructor();
        valid();
        equality();
        getters_setters();
    }

	virtual inline void assumptions() {

		REQUIRE( (std::is_default_constructible<cons_type>::value) );
		REQUIRE( (std::is_copy_constructible<cons_type>::value) );
		REQUIRE( (std::is_move_constructible<cons_type>::value) );
		REQUIRE( (std::is_copy_assignable<cons_type>::value) );
		REQUIRE( (std::is_move_assignable<cons_type>::value) );
		REQUIRE( (std::is_constructible<cons_type, var_type, var_type, int>::value) );

		// These are actually checked at compile-time, but no one needs to know :)
		static_assert( (cons_type(), true), "cons should be constexpr default-constructible." );
		static_assert( ((bool) cons_type(), true), "cons should be constexpr-explicitly bool-convertible." );
		static_assert( (!cons_type(), true), "cons::operator! should be constexpr." );
		static_assert( (cons_type().valid(), true), "cons::valid() should be constexpr." );
	}

    virtual inline void constructor() {

		SECTION( "default-constructed constraint fields should be invalid" ) {
			cons_type c1;

			REQUIRE( (!c1.xi().valid()) );
			REQUIRE( (!c1.xj().valid()) );
		}
    }

    virtual inline void valid() {

		SECTION( "default-constructed constraint should be invalid" ) {
			cons_type c1;

			REQUIRE( (!c1.valid() && !((bool) c1) && !c1) );
		}

		SECTION( "the type-based invalid value must be invalid" ) {

			REQUIRE( (!cons_type::invalid().valid()) );
		}

		SECTION( "non-default-constructed, two-variable constraint should be valid" ) {
			cons_type c1(var_type(1), var_type(2), 3);

			REQUIRE( (c1.valid() && ((bool) c1) && !!c1) );
		}

		SECTION( "constraints with two variables equal to each other must be invalid" ) {
			cons_type c1(var_type(1), var_type(1), 3);

			REQUIRE( (!c1.valid() && !((bool) c1) && !c1) );
		}
    }

    inline void assert_equal(const cons_type& c1, const cons_type& c2) {

        REQUIRE( (c1 == c2 && !(c1 != c2)) );
        REQUIRE( (c1.xi() == (int) c2.xi() && c1.xj() == c2.xj()) );
        REQUIRE( (c1.c() == c2.c()) );
    }

    virtual inline void equality() {

		SECTION( "copy-initialization must be consistent" ) {
			const cons_type c1(var_type(1), var_type(2), 3);
            const cons_type c2(c1);

			assert_equal(c1, c2);
		}

		SECTION( "copy-assignment must be consistent" ) {
			const cons_type c1(var_type(1), var_type(2), 3);
            cons_type c2;
            c2 = c1;

			assert_equal(c1, c2);
		}

		SECTION( "move-initialization must be consistent" ) {
            var_type xi(1), xj(2);
            value_type c(3);
			const cons_type c1(xi, xj, c);
			const cons_type c2(std::move(cons_type(c1)));

			REQUIRE( (c2.xi() == xi) );
			REQUIRE( (c2.xj() == xj) );
			REQUIRE( (c2.c() == c) );
		}

		SECTION( "move-assignment must be consistent" ) {
            var_type xi(1), xj(2);
            value_type c(3);
			const cons_type c1(xi, xj, c);
			cons_type c2;
            c2 = std::move(cons_type(c1));

			REQUIRE( (c2.xi() == xi) );
			REQUIRE( (c2.xj() == xj) );
			REQUIRE( (c2.c() == c) );
		}

		SECTION( "generic equality consistence test here!" ) {
			const cons_type c1(var_type(1), var_type(2), 3);
			const cons_type c2(var_type(1), var_type(2), 4);
			const cons_type c3(var_type(2), var_type(3), 3);
			const cons_type c4(var_type(2), var_type(4), 3);

            REQUIRE( (  c1 == c1  && !(c1 != c1)) );
            REQUIRE( (  c2 == c1  && !(c2 != c1)) );
            REQUIRE( (!(c3 == c2) &&  (c3 != c2)) );
            REQUIRE( (!(c4 == c3) &&  (c4 != c3)) );
		}
    }

    virtual inline void invalidate() {

        SECTION( "invalidate() must invalidate the constraint" ) {
            cons_type c1(var_type(1), var_type(2), 3);
            c1.invalidate();

            REQUIRE( (!c1.valid()) );
        }
    }

    virtual inline void getters_setters() {

		SECTION( "getters and setters" ) {
			cons_type c1(var_type(1), var_type(2), 3);

			REQUIRE( (var_type(1) == c1.xi()) );
			REQUIRE( (var_type(2) == c1.xj()) );
			REQUIRE( (3 == c1.c()) );

			REQUIRE( (&(c1.xi(var_type(2))) == &c1 && var_type(2) == c1.xj()) );
			REQUIRE( (&(c1.xj(var_type(3))) == &c1 && var_type(3) == c1.xj()) );
			REQUIRE( (&(c1.c(4)) == &c1 && 4 == (int) c1.c()) );
		}
    }
};


template <template <typename> class C, typename T = float>
class base_cons_test : public base_cons_test_base<C<T>> {};


template <typename T = float>
class oct_cons_test : public base_cons_test_base<adl::oct_cons<T>> {
public:
    using superclass = base_cons_test_base<adl::oct_cons<T>>;
    using var_type = typename superclass::var_type;
    using cons_type = typename superclass::cons_type;

    inline void test_all() {
		superclass::test_all();

        single_var();
        to_octdiff();
        from_octdiff();
    }

	inline void assumptions() {
		using namespace adl;
		superclass::assumptions();

		REQUIRE( (std::is_constructible<cons_type, var_type, int>::value) );
	}

    virtual inline void constructor() {
        superclass::constructor();
    }

	inline void valid() {
		superclass::valid();

        SECTION( "oct_cons: single-var constraint with a valid variable must be valid" ) {
            const cons_type c1(var_type(1), 3);

            REQUIRE( (c1.valid()) );
        }

        SECTION( "oct_cons: constraints with same variables must be invalid" ) {
            const cons_type c1(var_type(1), var_type(-1), 3);

            REQUIRE( (!c1.valid()) );
        }
    }

    // ---

    inline void single_var() {

        SECTION( "oct_cons: single-var constraints must be indicated as such" ) {
            const cons_type c1(var_type(1), 3);
            const cons_type c2(var_type(1), var_type(2), 3);

            REQUIRE( (c1.single_var()) );
            REQUIRE( (!c2.single_var()) );
        }
    }

    inline void from_octdiff() {
        const oct_var xi(1), xj(2);
        const octdiff_var xi_p(1), xi_n(2), xj_p(3), xj_n(4);
        const T c(1);
        const T c_2(2);

        SECTION( "from octdiff_cons to oct_cons: xi - xj") {
            const octdiff_cons<T> dci(xi_p, xj_p, c);
            const octdiff_cons<T> dcj(xj_n, xi_n, c);
            const oct_cons<T> occ(xi, -xj, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi + xj") {
            const octdiff_cons<T> dci(xi_p, xj_n, c);
            const octdiff_cons<T> dcj(xj_p, xi_n, c);
            const oct_cons<T> occ(xi, xj, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: -xi - xj") {
            const octdiff_cons<T> dci(xi_n, xj_p, c);
            const octdiff_cons<T> dcj(xj_n, xi_p, c);
            const oct_cons<T> occ(-xi, -xj, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: -xi + xj = xj + xi") {
            const octdiff_cons<T> dci(xi_n, xj_n, c);
            const octdiff_cons<T> dcj(xj_p, xi_p, c);
            const oct_cons<T> occ(-xi, xj, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi") {
            const octdiff_cons<T> dci(xi_p, xi_n, c_2);
            const octdiff_cons<T> dcj{};
            const oct_cons<T> occ(xi, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && !oc.valid()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi") {
            const octdiff_cons<T> dci(xi_n, xi_p, c_2);
            const octdiff_cons<T> dcj{};
            const oct_cons<T> occ(-xi, c);
            oct_cons<T> oc;

            REQUIRE( (&(oc.from_octdiff_i(dci)) == &oc && oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );

            REQUIRE( (&(oc.from_octdiff_j(dcj)) == &oc && !oc.valid()) );
        }
    }

    inline void to_octdiff() {
        const oct_var xi(1), xj(2);
        const octdiff_var xi_p(1), xi_n(2), xj_p(3), xj_n(4);
        const T c(1);
        const T c_2(2);

        SECTION( "from oct_cons to octdiff_cons: x1 - x2" ) {
            const oct_cons<T> oc(xi, -xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xj_p) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_n && dcj.xj() == xi_n) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: x1 + x2" ) {
            const oct_cons<T> oc(xi, xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xj_n) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_p && dcj.xj() == xi_n) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1 - x2" ) {
            const oct_cons<T> oc(-xi, -xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xj_p) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_n && dcj.xj() == xi_p) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1 + x2 = x2 - x1" ) {
            const oct_cons<T> oc(-xi, xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xj_n) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_p && dcj.xj() == xi_p) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: x1" ) {
            const oct_cons<T> oc(xi, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xi_n) );
            REQUIRE( (dci.c() == c_2) );
            REQUIRE( (!dcj.valid()) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1" ) {
            const oct_cons<T> oc(-xi, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = oc.to_octdiff_i(), dcj = oc.to_octdiff_j();

            // Answer consistency
            REQUIRE( (dc[0] == dci && dc[0].c() == dci.c()) );
            REQUIRE( (dc[1] == dcj && dc[1].c() == dcj.c()) );

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xi_p) );
            REQUIRE( (dci.c() == c_2) );
            REQUIRE( (!dcj.valid()) );
        }
    }
};


template <typename T = float>
class octdiff_cons_test : public base_cons_test_base<adl::octdiff_cons<T>> {
public:
    using superclass = base_cons_test_base<adl::octdiff_cons<T>>;
    using var_type = typename superclass::var_type;
    using cons_type = typename superclass::cons_type;

    inline void test_all() {
		superclass::test_all();

        swapped_variables();
        single_oct_var();
        from_oct();
        to_oct();
    }

	inline void valid() {
		superclass::valid();

        SECTION( "octdiff_cons: constraints involving the same oct_var must be valid only if the original oct_var's have different sign" ) {
            const cons_type c1(var_type(1), var_type(2), 3);
            const cons_type c2(var_type(1), var_type(1), 3);

            REQUIRE( (c1.valid()) );
            REQUIRE( (!c2.valid()) );
        }
    }

    // ---

	inline void swapped_variables() {

		SECTION( "swapped difference variables' getters" ) {
			var_type x(1), y(2);
			cons_type c1(x, y, 0);

			REQUIRE( (c1.xI() == x.swap()) );
			REQUIRE( (c1.xJ() == y.swap()) );
		}
    }

    inline void single_oct_var() {

        SECTION( "octdiff_cons: single-oct-variable constraints must indicate themselves as such" ) {
            const cons_type c1(var_type(1), var_type(2), 3);
            const cons_type c2(var_type(1), var_type(3), 4);

            REQUIRE( (c1.single_oct_var()) );
            REQUIRE( (!c2.single_oct_var()) );
        }
    }

    inline void from_oct() {
        // TODO
    }

    inline void to_oct() {
        // TODO
    }
};


TEST_CASE( "oct_cons", "[oct][oct/cons]" ) {
	using namespace adl;

	oct_cons_test<> oct_cons_test;
	octdiff_cons_test<> octdiff_cons_test;

	oct_cons_test.test_all();
	octdiff_cons_test.test_all();
}

}}
