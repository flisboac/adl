#ifndef ADL__TEST__OCT__OCT_VAR_BASE_HPP__
#define ADL__TEST__OCT__OCT_VAR_BASE_HPP__

#include <type_traits>
#include <utility>

#include "catch.hpp"

#include "adl/oct/oct_var.hpp"
#include "adl/oct/octdiff_var.hpp"

namespace adl { namespace test { namespace oct {
using namespace ::adl::oct;

template <typename T>
class base_oct_var_test {
public:
    virtual ~base_oct_var_test() {}

    virtual inline void test_all() {

        assumptions();
        constructor();
        assignment();
        valid();
        equality();
        comparison();
        index();
        increment();
    }

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

    virtual inline void constructor() {

        SECTION( "default-constructed variable should be zero when converted to int" ) {
            const T ov;

            REQUIRE( ((int) ov == 0 && ov.to_int() == 0) );
        }

        SECTION( "default-constructed variable should be equal to its type-based invalid value" ) {
            const T ov;

            REQUIRE( (ov == T::invalid()) );
        }

        SECTION( "int-initialization must be consistent" ) {
            const int iv1 = 1;
            const T ov1(iv1);

            REQUIRE( (iv1 == (int) ov1 && iv1 == ov1.to_int()) );
        }
    }

    virtual inline void assignment() {

        SECTION( "int-assignment must be consistent" ) {
            const int iv2 = 2;
            T ov2;
            ov2 = iv2;

            REQUIRE( (iv2 == (int) ov2 && iv2 == ov2.to_int()) );
        }
    }


    virtual inline void valid() {

        SECTION( "default construction results in an invalid variable" ) {
            const T ov;

            REQUIRE( (!ov.valid() && !ov && !((bool) ov)) ); // method, operator and explicit cast notations
        }

        SECTION( "zero-initialized variable must be invalid" ) {
            const T ov2(0);

            REQUIRE( (!ov2.valid() && !ov2 && !((bool) ov2)) );
        }

        SECTION( "the type-based invalid value must be invalid" ) {
            const T ov;

            REQUIRE( (!T::invalid().valid()) );
        }
    }

    virtual inline void equality() {

        SECTION( "copy-initialization must be consistent" ) {
            const T ov1(1);
            const T ov2(ov1);

            REQUIRE( ((int) ov1 == (int) ov2 && !((int) ov1 != (int) ov2)) );
            REQUIRE( (ov1 == ov2 && !(ov1 != ov2)) );
        }

        SECTION( "copy-assignment must be consistent" ) {
            const T ov1(1);
            T ov2;
            ov2 = ov1;

            REQUIRE( ((int) ov1 == (int) ov2 && !((int) ov1 != (int) ov2)) );
            REQUIRE( (ov1 == ov2 && !(ov1 != ov2)) );
        }

        SECTION( "move-initialization must be consistent" ) {
            const int iv = 1;
            const T ov1(std::move(T(iv)));

            REQUIRE( ((int) ov1 == iv) );
        }

        SECTION( "move-assignment must be consistent" ) {
            const int iv = 1;
            T ov1;
            ov1 = std::move(T(iv));

            REQUIRE( ((int) ov1 == iv) );
        }

        SECTION( "generic equality consistence test here!" ) {
            const T ov1, ov2(1), ov3(1);

            REQUIRE( (ov1 == ov1 && !(ov1 != ov1)) );
            REQUIRE( (ov2 == ov2 && !(ov2 != ov2)) );
            REQUIRE( (ov2 == ov3 && !(ov2 != ov3)) );
            REQUIRE( (!(ov1 == ov2) && ov1 != ov2) );
        }

        SECTION( "zero-assignment must make the variable invalid" ) {
            T ov2;
            ov2 = 0;

            REQUIRE( (0 == ov2.to_int() && ov2 == T::invalid()) );
        }
    }


    virtual inline void comparison() {

        SECTION( "generic positive-int comparison consistency check" ) {
            const T ov1(20), ov2(20), ov3(30), ov4(10);

            REQUIRE( (!(ov1 < ov2) && !(ov2 < ov1) && ov1 == ov2 && ov1.compare(ov2) == 0) );
            REQUIRE( (  ov2 < ov3  && !(ov3 < ov2) && ov2 != ov3 && ov2.compare(ov3) < 0) );
            REQUIRE( (!(ov3 < ov4) &&   ov4 < ov3  && ov3 != ov4 && ov3.compare(ov4) > 0) );
        }
    }


    virtual inline void index() {

        SECTION( "index must equal value minus 1" ) {
            const T ov2(1), ov3(2);

            REQUIRE( (ov2.index() == 0) );
            REQUIRE( (ov3.index() == 1) );
            REQUIRE( ((size_t) ov2 == ov2.index()) );
            REQUIRE( ((size_t) ov3 == ov3.index()) );
        }

        SECTION( "index of an invalid variable must not be zero" ) {
            const T ov1;

            REQUIRE( (ov1.index() != 0) );
        }
    }

    virtual inline void increment() {

        SECTION( "increment of a var must follow the normal int increment rules" ) {
            T ov1(1);

            REQUIRE( (1 == (int) ov1++ && 2 == (int) ov1) );
            REQUIRE( (++ov1 == ov1 && 3 == (int) ov1) );
        }

        SECTION( "increment of zero (invalid variable) should yield an invalid variable" ) {
            oct_var ov1, ov2;

            REQUIRE( (0 == (int) ov1++ && 0 == (int) ov1 && !ov1.valid()) );
            REQUIRE( (0 == (int) --ov2 && !ov2.valid()) );
        }

        SECTION( "decrement of a var must follow the normal int decrement rules" ) {
            T ov1(10);
            int iv1;

            INFO( "Initial value of ov1: " << ov1.to_int() );
            iv1 = (int) ov1--;
            INFO( "Value of ov1 returned by post-decrement: " << iv1 );
            INFO( "Value of ov1 after post-decrement: " << ov1.to_int() );
            REQUIRE( (10 == iv1) );
            REQUIRE( (9 == (int) ov1) );
            REQUIRE( (--ov1 == ov1 && 8 == (int) ov1) );
        }

        SECTION( "decrement of zero (invalid variable) should yield an invalid variable" ) {
            oct_var ov1, ov2;

            REQUIRE( (0 == (int) ov1-- && 0 == (int) ov1 && !ov1.valid()) );
            REQUIRE( (0 == (int) --ov2 && !ov2.valid()) );
        }
    }

    virtual inline void sections() const {

    }
};


template <typename T>
class oct_var_test : public base_oct_var_test<T> {};

}}}

#endif /* ADL__TEST__OCT__OCT_CONS_BASE_HPP__ */
