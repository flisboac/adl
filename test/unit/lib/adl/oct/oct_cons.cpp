#include "./oct_cons_base_.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

template <typename T = float>
class oct_cons_test : public base_cons_test_base<oct_cons<T>> {
public:
    using superclass = base_cons_test_base<oct_cons<T>>;
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

        SECTION( "constraints with two variables equal to each other must be calculated appropriately" ) {
            var_type x1(1);
            cons_type c1( x1,  x1, T(10));  //  x1 <= 5
            cons_type c2(-x1, -x1, T(10));  // -x1 >= 5
            cons_type c3(-x1,  x1, T(-10)); // ERROR b/c 0 <= -10 does not hold
            cons_type c4( x1, -x1, T(10));  // INVALID b/c it is difficult to ensure previous item if allowed

            REQUIRE( ( c1.valid()) );
            REQUIRE( ( c1.xi() ==  x1 && c1.c() == 5) );
            REQUIRE( ( c2.valid()) );
            REQUIRE( ( c2.xi() == -x1 && c2.c() == 5) );
            REQUIRE( ( !c3.valid()) );
            REQUIRE( ( !c4.valid()) );
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
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(xi, -xj, c);
            oct_cons<T> oc = dcc;

            INFO( "xi: " << ((int) oc.xi()) << ", xj: " << ((int) oc.xj()) << ", c: " << oc.c() );
            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi + xj") {
            const octdiff_cons<T> dci(xi_p, xj_n, c);
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(xi, xj, c);
            oct_cons<T> oc = dcc;

            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: -xi - xj") {
            const octdiff_cons<T> dci(xi_n, xj_p, c);
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(-xi, -xj, c);
            oct_cons<T> oc = dcc;

            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: -xi + xj = xj + xi") {
            const octdiff_cons<T> dci(xi_n, xj_n, c);
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(-xi, xj, c);
            oct_cons<T> oc = dcc;

            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi") {
            const octdiff_cons<T> dci(xi_p, xi_n, c_2);
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(xi, c);
            oct_cons<T> oc = dcc;

            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
        }

        SECTION( "from octdiff_cons to oct_cons: xi") {
            const octdiff_cons<T> dci(xi_n, xi_p, c_2);
            const auto dcc = dci.conjunction();
            const auto dcj = dcc.second();
            const oct_cons<T> occ(-xi, c);
            oct_cons<T> oc = dcc;

            REQUIRE( (oc.valid()) );
            REQUIRE( (oc == occ && oc.c() == occ.c()) );
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
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xj_p) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_n && dcj.xj() == xi_n) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: x1 + x2" ) {
            const oct_cons<T> oc(xi, xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xj_n) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_p && dcj.xj() == xi_n) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1 - x2" ) {
            const oct_cons<T> oc(-xi, -xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xj_p) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_n && dcj.xj() == xi_p) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1 + x2 = x2 - x1" ) {
            const oct_cons<T> oc(-xi, xj, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xj_n) );
            REQUIRE( (dci.c() == c) );
            REQUIRE( (dcj.xi() == xj_p && dcj.xj() == xi_p) );
            REQUIRE( (dcj.c() == c) );
        }

        SECTION( "from oct_cons to octdiff_cons: x1" ) {
            const oct_cons<T> oc(xi, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_p && dci.xj() == xi_n) );
            REQUIRE( (dci.c() == c_2) );
            REQUIRE( (!dcj.valid()) );
        }

        SECTION( "from oct_cons to octdiff_cons: -x1" ) {
            const oct_cons<T> oc(-xi, c);
            const auto dc = oc.to_octdiff();
            const octdiff_cons<T> dci = dc.first(), dcj = dc.second();

            REQUIRE( (dci.xi() == xi_n && dci.xj() == xi_p) );
            REQUIRE( (dci.c() == c_2) );
            REQUIRE( (!dcj.valid()) );
        }
    }
};


TEST_CASE( "oct_cons", "[oct][oct/cons]" ) {
    using namespace adl;

    oct_cons_test<> oct_cons_test;
    oct_cons_test.test_all();
}

}}}
