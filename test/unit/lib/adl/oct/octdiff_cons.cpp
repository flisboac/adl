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

        SECTION( "constraints with two variables equal to each other must be invalid" ) {
            cons_type c1(var_type(1), var_type(1), 3);

            REQUIRE( (!c1.valid() && !((bool) c1) && !c1) );
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


TEST_CASE( "octdiff_cons", "[oct][oct/cons]" ) {
    octdiff_cons_test<> octdiff_cons_test;
    octdiff_cons_test.test_all();
}

}}}

