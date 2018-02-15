// $flisboac 2018-02-12

#include <typeinfo>
#include <string>

#include "./stti.unit.hpp"
#include "adl/stti/has_member_type.hpp"

using namespace std::literals;

#define sfinae_disallowed false
#define sfinae_allowed true

namespace {
    adl_DECL_HAS_MEMBER_TYPE_(name);
    adl_DEFN_HAS_MEMBER_TYPE_(name);

    template <typename T> class initializer_list_literal {
        T size = T();
    public:
        initializer_list_literal(std::initializer_list<T> list) : size(list.size()) {}
        T get_size() const { return size; }
    };

    template <typename T> class custom_constructor_literal {
        T value = T();
    public:
        custom_constructor_literal(T a, T b) : value(b) {}
        T get_value() const { return value; }
    };

    struct initializer_list_type { using name = initializer_list_literal<int>; };
    struct custom_constructor_type { using name = custom_constructor_literal<int>; };
}


#define TEST_MEMBER_DATA_PRESENCE(member_name, expected, enclosing_type, fallback_type) \
    SECTION("STTI, Test member type presence: " #member_name ", for <" #enclosing_type ", " #fallback_type ">"){ \
        auto member_name ## _test = stti_test::has_ ## member_name <enclosing_type, fallback_type, has_member_type_name>(nullptr); \
        auto catch_cant_handle_ ## member_name ## _test = stti_test::is_ ## expected(member_name ## _test); \
        adl_STTI_DETECTION_CAPTURE(member_name ## _test); REQUIRE( catch_cant_handle_ ## member_name ## _test ); \
    }

#define TEST_MEMBER_TYPE_PRESENCE(member_name, expected, enclosing_type, fallback_type) \
    SECTION("STTI, Test member type presence: " #member_name ", for <" #enclosing_type ", " #fallback_type ">"){ \
        auto member_name ## _test = stti_test::has_ ## member_name ## _type<enclosing_type, fallback_type, has_member_type_name, has_member_type_name_ ## member_name ## _t>(nullptr); \
        auto catch_cant_handle_ ## member_name ## _test = stti_test::is_ ## expected(member_name ## _test); \
        adl_STTI_DETECTION_CAPTURE(member_name ## _test); REQUIRE( catch_cant_handle_ ## member_name ## _test ); \
    }

static void test_non_signature() {

    SECTION("Test presence") {
        REQUIRE( (!has_member_type_name_v<stti_test::empty>) );
        REQUIRE( ( has_member_type_name_v<stti_test::type>) ); REQUIRE( ("name"s == has_member_type_name<stti_test::type>::name) );
        REQUIRE( (!has_member_type_name_v<stti_test::template_type>) );

        REQUIRE( (!has_member_type_name_v<stti_test::static_data>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_const_data>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_template_data>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_template_const_data>) );

        REQUIRE( (!has_member_type_name_v<stti_test::static_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_overloaded_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_template_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_template_overloaded_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::static_template_overloaded2_function>) );

        REQUIRE( (!has_member_type_name_v<stti_test::data>) );
        REQUIRE( (!has_member_type_name_v<stti_test::const_data>) );

        REQUIRE( (!has_member_type_name_v<stti_test::function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::overloaded_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::template_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::template_overloaded_function>) );
        REQUIRE( (!has_member_type_name_v<stti_test::template_overloaded2_function>) );
    }

    SECTION("Test member presence: the class traits itself") {
        auto empty_and_void_test = stti_test::has_traits<stti_test::empty, void, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_ttype_test = stti_test::has_traits<stti_test::empty, stti_test::template_type, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_stdata_test = stti_test::has_traits<stti_test::empty, stti_test::static_template_data, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_function_test = stti_test::has_traits<stti_test::empty, stti_test::function, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_sfunction_test = stti_test::has_traits<stti_test::empty, stti_test::static_function, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_data_test = stti_test::has_traits<stti_test::empty, stti_test::data, has_member_type_name, has_member_type_name_t>(nullptr);
        auto empty_and_sdata_test = stti_test::has_traits<stti_test::empty, stti_test::static_data, has_member_type_name, has_member_type_name_t>(nullptr);

        auto empty_and_type_test = stti_test::has_traits<stti_test::empty, stti_test::type, has_member_type_name, has_member_type_name_t>(nullptr);
        auto type_and_void_test = stti_test::has_traits<stti_test::type, void, has_member_type_name, has_member_type_name_t>(nullptr);
        auto type_and_empty_test = stti_test::has_traits<stti_test::type, stti_test::empty, has_member_type_name, has_member_type_name_t>(nullptr);

        adl_STTI_DETECTION_CAPTURE(empty_and_void_test); REQUIRE( (stti_test::is_undefined(empty_and_void_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_ttype_test); REQUIRE( (stti_test::is_undefined(empty_and_ttype_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_stdata_test); REQUIRE( (stti_test::is_undefined(empty_and_stdata_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_function_test); REQUIRE( (stti_test::is_undefined(empty_and_function_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_sfunction_test); REQUIRE( (stti_test::is_undefined(empty_and_sfunction_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_data_test); REQUIRE( (stti_test::is_undefined(empty_and_data_test)) );
        adl_STTI_DETECTION_CAPTURE(empty_and_sdata_test); REQUIRE( (stti_test::is_undefined(empty_and_sdata_test)) );

        adl_STTI_DETECTION_CAPTURE(empty_and_type_test); REQUIRE( (stti_test::is_valid(empty_and_type_test)) );
        adl_STTI_DETECTION_CAPTURE(type_and_void_test); REQUIRE( (stti_test::is_valid(type_and_void_test)) );
        adl_STTI_DETECTION_CAPTURE(type_and_empty_test); REQUIRE( (stti_test::is_valid(type_and_empty_test)) );
    }

    SECTION("Test member presence: is_using_fallback") {
        TEST_MEMBER_DATA_PRESENCE(is_using_fallback, undefined, stti_test::empty, void);
        TEST_MEMBER_DATA_PRESENCE(is_using_fallback, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_DATA_PRESENCE(is_using_fallback, valid, stti_test::type, void);
        TEST_MEMBER_DATA_PRESENCE(is_using_fallback, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: enclosing") {
        TEST_MEMBER_TYPE_PRESENCE(enclosing, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(enclosing, undefined, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(enclosing, valid, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(enclosing, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: fallback") {
        TEST_MEMBER_TYPE_PRESENCE(fallback, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(fallback, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(fallback, undefined, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(fallback, undefined, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: selected") {
        TEST_MEMBER_TYPE_PRESENCE(selected, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(selected, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(selected, valid, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(selected, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: derived") {
        TEST_MEMBER_TYPE_PRESENCE(derived, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(derived, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(derived, valid, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(derived, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: extension") {
        TEST_MEMBER_TYPE_PRESENCE(extension, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(extension, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(extension, valid, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(extension, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Test member presence: functor") {
        TEST_MEMBER_TYPE_PRESENCE(functor, undefined, stti_test::empty, void);
        TEST_MEMBER_TYPE_PRESENCE(functor, valid, stti_test::empty, stti_test::type);
        TEST_MEMBER_TYPE_PRESENCE(functor, valid, stti_test::type, void);
        TEST_MEMBER_TYPE_PRESENCE(functor, valid, stti_test::type, stti_test::empty);
    }

    SECTION("Testing some specifics of traits' members") {
        auto enclosing_test = stti_test::has_enclosing_type<stti_test::type, void, has_member_type_name, has_member_type_name_enclosing_t>(nullptr);
        auto fallback_test = stti_test::has_fallback_type<stti_test::empty, stti_test::type, has_member_type_name, has_member_type_name_fallback_t>(nullptr);
        auto selected_enclosing_test = stti_test::has_selected_type<stti_test::type, void, has_member_type_name, has_member_type_name_selected_t>(nullptr);
        auto selected_fallback_test = stti_test::has_selected_type<stti_test::empty, stti_test::type, has_member_type_name, has_member_type_name_selected_t>(nullptr);
        auto derived_enclosing_test = stti_test::has_derived_type<stti_test::type, void, has_member_type_name, has_member_type_name_derived_t>(nullptr);
        auto derived_fallback_test = stti_test::has_derived_type<stti_test::empty, stti_test::type, has_member_type_name, has_member_type_name_derived_t>(nullptr);
        auto extension_enclosing_test = stti_test::has_extension_type<stti_test::type, void, has_member_type_name, has_member_type_name_extension_t>(nullptr);
        auto extension_fallback_test = stti_test::has_extension_type<stti_test::empty, stti_test::type, has_member_type_name, has_member_type_name_extension_t>(nullptr);

        adl_STTI_DETECTION_CAPTURE(enclosing_test); REQUIRE( (stti_test::is_valid(enclosing_test)) );
        adl_STTI_DETECTION_CAPTURE(fallback_test); REQUIRE( (stti_test::is_valid(fallback_test)) );
        adl_STTI_DETECTION_CAPTURE(selected_enclosing_test); REQUIRE( (stti_test::is_valid(selected_enclosing_test)) );
        adl_STTI_DETECTION_CAPTURE(selected_fallback_test); REQUIRE( (stti_test::is_valid(selected_fallback_test)) );
        adl_STTI_DETECTION_CAPTURE(derived_enclosing_test); REQUIRE( (stti_test::is_valid(derived_enclosing_test)) );
        adl_STTI_DETECTION_CAPTURE(derived_fallback_test); REQUIRE( (stti_test::is_valid(derived_fallback_test)) );
        adl_STTI_DETECTION_CAPTURE(extension_enclosing_test); REQUIRE( (stti_test::is_valid(extension_enclosing_test)) );
        adl_STTI_DETECTION_CAPTURE(extension_fallback_test); REQUIRE( (stti_test::is_valid(extension_fallback_test)) );
    }

    SECTION("Test functor usage") {
        auto empty_and_type_functor = stti_test::test_functor<stti_test::empty, stti_test::type, has_member_type_name>(nullptr, [](auto f) { return f.value == 1;}, 1);
        auto type_and_void_functor = stti_test::test_functor<stti_test::type, void, has_member_type_name>(nullptr, [](auto f) { return f.value == 1;}, 1);
        auto ilist_and_void_functor = stti_test::test_functor<initializer_list_type, void, has_member_type_name>(nullptr, [](auto f) { return f.get_size() == 2;}, 3, 4);
        auto nilist_and_void_functor = stti_test::test_functor<initializer_list_type, void, has_member_type_name>(nullptr, [](auto f) { return f.get_size() == 2;}, {3, 4});
        auto cc_and_void_functor = stti_test::test_functor<custom_constructor_type, void, has_member_type_name>(nullptr, [](auto f) { return f.get_value() == 4;}, 3, 4);

        adl_STTI_DETECTION_CAPTURE(empty_and_type_functor); REQUIRE( (stti_test::is_valid(empty_and_type_functor)) );
        adl_STTI_DETECTION_CAPTURE(type_and_void_functor); REQUIRE( (stti_test::is_valid(type_and_void_functor)) );
        adl_STTI_DETECTION_CAPTURE(ilist_and_void_functor); REQUIRE( (stti_test::is_valid(ilist_and_void_functor)) );
        adl_STTI_DETECTION_CAPTURE(nilist_and_void_functor); REQUIRE( (stti_test::is_valid(nilist_and_void_functor)) );
        adl_STTI_DETECTION_CAPTURE(cc_and_void_functor); REQUIRE( (stti_test::is_valid(cc_and_void_functor)) );
    }
}

template <typename... Ts>
static void test_with_signature() {
    // (A BIG) TODO: Test `has_member_type_name<E, F>::with_signature<...>`
    SECTION("Test presence") {
        REQUIRE( (!has_member_type_name_signature_v<stti_test::empty, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::type, void, Ts...>) );

        // NOT WORKING, because parameter packs cannot be expanded into non-packed template parameters, even if they're empty.
        // A great oversight and lack of knowledge on my part, all this work for nothing (not quite, but still).
        // Relentless refactorings ahead. Will probably steal ideas from N4502 (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf)
        //REQUIRE( ( has_member_type_name_signature_v<stti_test::template_type, void, Ts...>) );

        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_data, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_const_data, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_template_data, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_template_const_data, void, Ts...>) );

        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_overloaded_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_template_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_template_overloaded_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::static_template_overloaded2_function, void, Ts...>) );

        REQUIRE( (!has_member_type_name_signature_v<stti_test::data, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::const_data, void, Ts...>) );

        REQUIRE( (!has_member_type_name_signature_v<stti_test::function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::overloaded_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::template_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::template_overloaded_function, void, Ts...>) );
        REQUIRE( (!has_member_type_name_signature_v<stti_test::template_overloaded2_function, void, Ts...>) );
    }

}

TEST_CASE("unit:adl/stti/has_member_type.hpp", "") {
    test_non_signature();
    test_with_signature<int>();
}
