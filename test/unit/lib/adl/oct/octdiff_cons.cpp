#include "./oct_cons_base_.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

template <typename T = float>
class octdiff_cons_test : public base_cons_test_base<octdiff_cons<T>> {
public:
    using superclass = base_cons_test_base<octdiff_cons<T>>;
    using var_type = typename superclass::var_type;
    using cons_type = typename superclass::cons_type;

    inline void test_all() {
        superclass::test_all();

        pair();
        swapped_variables();
        single_oct_var();
        complement();
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

        SECTION( "constraints with two variables equal to each other must be invalid" ) {
            cons_type c1(var_type(1), var_type(1), 3);

            REQUIRE( (!c1.valid() && !((bool) c1) && !c1) );
        }
    }

    // ---

    inline void pair() {
        using pair = typename octdiff_cons<T>::pair;

        SECTION( "pair comparisons" ) {
            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr octdiff_var x2_pos(11), x2_neg(12);
            constexpr auto x1p_x2n = make_cons(x1_pos, x2_neg, c);
            constexpr auto x2p_x1n = make_cons(x2_pos, x1_neg, c);
            constexpr auto x1p_x1n = make_cons(x1_pos, x1_neg, c);
            constexpr pair p1(x1p_x2n, x2p_x1n), p2(x1p_x1n);

            REQUIRE( (p1.valid()) );
            REQUIRE( (p2.valid()) );
            REQUIRE( (p1.commute().valid()) );
            REQUIRE( (p2.commute().valid()) );

            REQUIRE( (p1 == p1 && !(p1 != p1)) );
            REQUIRE( (p1.commute() == p1) );

            REQUIRE( (p2 == p2 && !(p2 != p2)) );
            REQUIRE( (p2.commute() == p2) );

            REQUIRE( (p1 != p2) );
        }

        SECTION( "pair constructed with two valid constraints should also be valid" ) {
            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr octdiff_var x2_pos(11), x2_neg(12);
            constexpr auto e1 = make_cons(x1_pos, x2_neg, c);
            constexpr auto e2 = make_cons(x2_pos, x1_neg, c);
            constexpr pair p1(e1, e2), p2(e2, e1);

            REQUIRE( (p1.valid()) );
            REQUIRE( (p2.valid()) );

            REQUIRE( (!p1.single_cons()) );
            REQUIRE( (p1.first() == e1) );
            REQUIRE( (p1.second() == e2) );

            REQUIRE( (p1.commute() == p2) );
            REQUIRE( (p1.commute() == p1) ); // (a && b) is logically equivalent to (b && a), set-wise (e.g. disregarding short-circuiting)
        }

        SECTION( "pair should identify invalid single-cons case" ) {
            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr auto e1 = make_cons(x1_pos, x1_pos, c);
            constexpr auto e2 = octdiff_cons<T>::invalid();
            constexpr pair p1(e1, e2);

            REQUIRE( (!p1.valid()) );
            REQUIRE( (!p1.commute().valid()) );
        }

        SECTION( "pair with two equal constraints should be invalid" ) {
            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr auto e1 = make_cons(x1_pos, x1_pos, c);
            constexpr pair p1(e1, e1);

            REQUIRE( (!p1.valid()) );
        }

        SECTION( "single-cons pair constructed with two-cons constructor should disregard where the valid cons is provided" ) {
            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr auto e1 = make_cons(x1_pos, x1_neg, c);
            constexpr auto e2 = octdiff_cons<T>::invalid();
            constexpr pair p1(e1, e2), p2(e2, e1);

            REQUIRE( (p1.valid()) );
            REQUIRE( (p2.valid()) );
            REQUIRE( (p1.single_cons()) );
            REQUIRE( (p2.single_cons()) );

            REQUIRE( (p1.first() == e1 && !p1.second().valid()) );
            REQUIRE( (p2.first() == e1 && !p2.second().valid()) );

            REQUIRE( (p1.commute() == p1) );
            REQUIRE( (p2.commute() == p2) );
        }
    }

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

    inline void complement() {
        using namespace adl::oct;

        SECTION( "single-oct-var constraints have no complements" ) {
            // Reason: the complement of a single-var octdiff constraint is itself.
            // Also, there is no single-variable octagonal constraint
            // convertible to a pair of different constraints, as far as the
            // literature demonstrates.

            constexpr octdiff_var xi_pos(9), xi_neg(10);
            constexpr auto c1 = make_cons(xi_pos, xi_neg, 10),
                c2 = make_cons(xi_neg, xi_pos, 10),
                cc1 = c1.complement(),
                cc2 = c2.complement();

            REQUIRE( (c1.valid()) );
            REQUIRE( (c2.valid()) );
            REQUIRE( (!cc1.valid()) );
            REQUIRE( (!cc2.valid()) );
        }

        SECTION( "double-var constraints have valid complements if the original constraint is valid" ) {
            using pair = typename octdiff_cons<T>::pair;

            constexpr T c(10);
            constexpr octdiff_var x1_pos(9), x1_neg(10);
            constexpr octdiff_var x2_pos(11), x2_neg(12);
            constexpr auto x1p_x2p = make_cons(x1_pos, x2_pos, c),
                x1p_x2n = make_cons(x1_pos, x2_neg, c),
                x1n_x2p = make_cons(x1_neg, x2_pos, c),
                x1n_x2n = make_cons(x1_neg, x2_neg, c),
                x2p_x1p = make_cons(x2_pos, x1_pos, c),
                x2p_x1n = make_cons(x2_pos, x1_neg, c),
                x2n_x1p = make_cons(x2_neg, x1_pos, c),
                x2n_x1n = make_cons(x2_neg, x1_neg, c);

            REQUIRE( (x1p_x2p.valid()) );
            REQUIRE( (x1p_x2n.valid()) );
            REQUIRE( (x1n_x2n.valid()) );
            REQUIRE( (x1n_x2p.valid()) );
            REQUIRE( (x2p_x1p.valid()) );
            REQUIRE( (x2p_x1n.valid()) );
            REQUIRE( (x2n_x1n.valid()) );
            REQUIRE( (x2n_x1p.valid()) );

            //  x1 + x2 <= c
            //  - 1st: x1p - x2n <= c
            //  - 2nd: x2p - x1n <= c
            //  x1 - x2 <= c
            //  - 1st: x1p - x2p <= c
            //  - 2nd: x2n - x1n <= c
            // -x1 + x2 <= c ===> x2 - x1 <= c
            //  - 1st: x2p - x1p <= c
            //  - 2nd: x1n - x2n <= c
            // -x1 - x2 <= c
            //  - 1st: x1n - x2p <= c
            //  - 2nd: x2n - x1p <= c

            REQUIRE( (x1p_x2p.complement() == x2n_x1n) ); // 1st of  x1 - x2 <= c
            REQUIRE( (x1p_x2n.complement() == x2p_x1n) ); // 1st of  x1 + x2 <= c
            REQUIRE( (x1n_x2p.complement() == x2n_x1p) ); // 1st of -x1 - x2 <= c
            REQUIRE( (x1n_x2n.complement() == x2p_x1p) ); // 2nd of -x1 + x2 <= c

            REQUIRE( (x2p_x1p.complement() == x1n_x2n) ); // 1st of -x1 + x2 <= c
            REQUIRE( (x2p_x1n.complement() == x1p_x2n) ); // 2nd of  x1 + x2 <= c
            REQUIRE( (x2n_x1p.complement() == x1n_x2p) ); // 2nd of -x1 - x2 <= c
            REQUIRE( (x2n_x1n.complement() == x1p_x2p) ); // 2nd of  x1 - x2 <= c

            REQUIRE( (x1p_x2p.conjunction() == pair(x1p_x2p, x2n_x1n)) );
            REQUIRE( (x1p_x2n.conjunction() == pair(x1p_x2n, x2p_x1n)) );
            REQUIRE( (x1n_x2p.conjunction() == pair(x1n_x2p, x2n_x1p)) );
            REQUIRE( (x1n_x2n.conjunction() == pair(x1n_x2n, x2p_x1p)) );

            REQUIRE( (x2p_x1p.conjunction() == pair(x2p_x1p, x1n_x2n)) );
            REQUIRE( (x2p_x1n.conjunction() == pair(x2p_x1n, x1p_x2n)) );
            REQUIRE( (x2n_x1p.conjunction() == pair(x2n_x1p, x1n_x2p)) );
            REQUIRE( (x2n_x1n.conjunction() == pair(x2n_x1n, x1p_x2p)) );

            REQUIRE( (x1p_x2p.conjunction() == pair(x2n_x1n, x1p_x2p)) );
        }
    }

    inline void from_oct() {
        // TODO
    }

    inline void to_oct() {
        // TODO
    }
};


TEST_CASE( "octdiff_cons", "[oct][oct/cons]" ) {
    octdiff_cons_test<> octdiff_cons_test;
    octdiff_cons_test.test_all();
}

}}}

