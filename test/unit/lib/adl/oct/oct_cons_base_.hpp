#ifndef ADL__TEST__OCT__OCT_CONS_BASE_HPP__
#define ADL__TEST__OCT__OCT_CONS_BASE_HPP__

#include <type_traits>

#include "catch.hpp"

#include "adl/oct/oct_var.hpp"
#include "adl/oct/octdiff_var.hpp"
#include "adl/oct/oct_cons.hpp"
#include "adl/oct/octdiff_cons.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;


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


}}}

#endif /* ADL__TEST__OCT__OCT_CONS_BASE_HPP__ */
