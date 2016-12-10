#include "catch.hpp"

#include "./oct_vexpr_base_.hpp"

#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

class oct_vexpr_test : public oct_vexpr_base_test_<oct_vexpr> {
public:
    using superclass = oct_vexpr_base_test_<oct_vexpr>;

    inline void test_all() {
        superclass::test_all();
    }

    inline void assumptions() {
        superclass::assumptions();

        REQUIRE( (std::is_constructible<oct_var>::value) );

        static_assert( (oct_vexpr(var_type()), true), "should be constexpr 1-var-constructible." );
    }

    inline void valid() {
        superclass::valid();

        SECTION( "Duplicate variables should be identified correctly" ) {
            const oct_var x1(10);
            const oct_vexpr o1(x1, -x1);
            const oct_vexpr o2(-x1, x1);
            const oct_vexpr o3(x1, x1);
            const oct_vexpr o4(-x1, -x1);

            REQUIRE( (!o1.doubled() && !o2.doubled()) );
            REQUIRE( (!o1.valid()   && !o2.valid()) );

            REQUIRE( ( o3.doubled() &&  o4.valid()) );
            REQUIRE( ( o3.valid()   &&  o4.valid()) );
        }
    }
};


}}}
