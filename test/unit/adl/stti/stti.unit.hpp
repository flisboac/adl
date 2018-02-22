// $flisboac 2018-02-12
/**
 * @file stti.unit.hpp
 */
#ifndef adl__stti__stti__unit__hpp__test__
#define adl__stti__stti__unit__hpp__test__

#include "adl_catch.hpp"
#include "adl/assert.hpp"

#define adl_STTI_DETECTION_CAPTURE(name) INFO(#name " := " << stti_test::name_of(name))
#define adl_STTI_DETECTION_NAMED_CAPTURE(name, var_name) INFO(name << ", " #name " := " << stti_test::name_of(var_name))

namespace stti_test {

template <typename T> struct literal {
    T value = T();
};
using int_literal = literal<int>;
using double_literal = literal<double>;

struct empty {};

//
// TYPE MEMBER
//
struct type { using name = int_literal; };
struct template_type { template <typename T = int> using name = literal<T>; };
struct templatepack_type { template <typename T = int, typename...> using name = literal<T>; };
struct templatenum_type { template <typename T = int, int n = 0> using name = literal<T>; };

//
// STATIC DATA MEMBER
//
struct static_data { static int_literal name; };
struct static_const_data { constexpr static int_literal name {2}; };
struct static_template_data { template <typename T> static literal<T> name {11}; };
struct static_template_const_data { template <typename T> static literal<T> const name {12}; };


//
// STATIC FUNCTION MEMBER
//
struct static_function { static int_literal name() { return {1}; } };
struct static_overloaded_function {
    static int_literal name() { return {1}; }
    static int_literal name(int v) { return {v}; }
    static double_literal name(double v) { return {v}; }
};
struct static_template_function { template <typename T> static literal<T> name(T v) { return {v}; } };
struct static_template_overloaded_function {
    static int_literal name() { return {1}; }
    template <typename T> static literal<T> name(T v) { return {v}; }
};
struct static_template_overloaded2_function {
    static int_literal name() { return {1}; }
    static int_literal name(int v) { return {v}; }
    template <typename T> static literal<T> name(T v) { return {v}; }
};

//
// DATA MEMBER
//
struct data { int_literal name {1}; };
struct const_data { int_literal const name {2}; };

//
// FUNCTION MEMBER
//
struct function { int_literal name() { return {1}; } };
struct overloaded_function {
    int_literal name() { return {1}; }
    int_literal name(int v) { return {v}; }
    double_literal name(double v) { return {v}; }
};
struct template_function { template <typename T> literal<T> name(T v) { return {v}; } };
struct template_overloaded_function {
    int_literal name() { return {1}; }
    template <typename T> literal<T> name(T v) { return {v}; }
};
struct template_overloaded2_function {
    int_literal name() { return {1}; }
    int_literal name(int v) { return {v}; }
    int_literal name(int v) const { return {v}; }
    template <typename T> literal<T> name(T v) { return {v}; }
};

//
// HELPERS
//

enum class detection_state { undefined, valid, invalid };
constexpr bool is_valid(detection_state a) { return a == detection_state::valid; }
constexpr bool is_invalid(detection_state a) { return a == detection_state::invalid; }
constexpr bool is_undefined(detection_state a) { return a == detection_state::undefined; }
constexpr char const* name_of(detection_state a) {
    switch(a) {
    case detection_state::undefined: return "detection_state::undefined";
    case detection_state::valid: return "detection_state::valid";
    case detection_state::invalid: return "detection_state::invalid";
    }
}

template <typename U, typename V> class dummy_template_type {};
class dummy_functor_type {};
template <typename T> struct dummy_template_type_alias { template <typename U, typename V> using type = T; };

namespace common {

    template <typename E, typename F, typename VT, bool D = VT::detected>
    constexpr detection_state has_traits(typename VT::enclosing_type*) {
        if (!D) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, bool D = VT::detected>
    constexpr detection_state has_traits(typename VT::fallback_type*) {
        if (!D) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, bool D = VT::detected>
    constexpr detection_state has_traits(VT*) {
        if (D) return detection_state::invalid;
        return detection_state::valid;
    }

    template <typename E, typename F, typename VT>
    constexpr detection_state has_enclosing_type(typename VT::enclosing_type* dummy) {
        using test_type = typename VT::enclosing_type;
        if (!std::is_same<test_type, E>::value) return detection_state::invalid; // The enclosing type must be the same as the one passed as a parameter to the TTI class
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_enclosing_type(E* dummy) { return detection_state::undefined; }


    template <typename E, typename F, typename VT>
    constexpr detection_state has_fallback_type(typename VT::fallback_type* dummy) {
        using test_type = typename VT::fallback_type; // Same logic as the enclosing_type
        if (!std::is_same<test_type, F>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_fallback_type(E* dummy) { return detection_state::undefined; }


    template <typename E, typename F, typename VT, bool FB = VT::is_using_fallback>
    constexpr detection_state has_is_using_fallback(typename VT::fallback_type* dummy) {
        if (!FB) return detection_state::invalid;
        if (has_enclosing_type<E, F, VT>(nullptr) != detection_state::undefined) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, bool FB = VT::is_using_fallback>
    constexpr detection_state has_is_using_fallback(typename VT::enclosing_type* dummy) {
        if (FB) return detection_state::invalid;
        if (has_fallback_type<E, F, VT>(nullptr) != detection_state::undefined) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_is_using_fallback(E* dummy) { return detection_state::undefined; }


    template <typename E, typename F, typename VT, typename test_type = typename VT::selected_type>
    constexpr detection_state has_selected_type(typename VT::enclosing_type* dummy) {
        using original_type = typename VT::enclosing_type;
        if (!std::is_same<test_type, original_type>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, typename test_type = typename VT::selected_type>
    constexpr detection_state has_selected_type(typename VT::fallback_type* dummy) {
        using original_type = typename VT::fallback_type;
        if (!std::is_same<test_type, original_type>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_selected_type(E* dummy) { return detection_state::undefined; }


    template <typename E, typename F, typename VT, typename test_type = typename VT::derived_type>
    constexpr detection_state has_derived_type(typename VT::enclosing_type* dummy) {
        if (std::is_same<test_type, void>::value) return detection_state::invalid;
        if (std::is_same<test_type, E>::value) return detection_state::invalid;
        if (!std::is_base_of<E, test_type>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, typename test_type = typename VT::derived_type>
    constexpr detection_state has_derived_type(typename VT::fallback_type* dummy) {
        if (std::is_same<test_type, void>::value) return detection_state::invalid;
        if (std::is_same<test_type, E>::value) return detection_state::invalid;
        if (std::is_same<test_type, F>::value) return detection_state::invalid;
        if (!std::is_base_of<E, test_type>::value) return detection_state::invalid;
        if (!std::is_base_of<F, test_type>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_derived_type(E* dummy) { return detection_state::undefined; }


    template <typename E, typename F, typename VT, typename test_type = typename VT::extension_type>
    constexpr detection_state has_extension_type(typename VT::enclosing_type* dummy) {
        if (std::is_same<test_type, void>::value) return detection_state::invalid;
        if (std::is_same<test_type, E>::value) return detection_state::invalid;
        if (std::is_same<test_type, F>::value) return detection_state::invalid;
        if (std::is_base_of<E, test_type>::value) return detection_state::invalid;
        if (std::is_base_of<F, test_type>::value) return detection_state::invalid;
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT, typename test_type = typename VT::extension_type>
    constexpr detection_state has_extension_type(typename VT::fallback_type* dummy) {
        if (std::is_same<test_type, void>::value) return detection_state::invalid;
        if (std::is_base_of<F, E>::value) {
            if (std::is_same<test_type, E>::value) return detection_state::invalid;
            if (std::is_same<test_type, F>::value) return detection_state::invalid;
            if (std::is_base_of<E, test_type>::value) return detection_state::invalid;
            if (std::is_base_of<F, test_type>::value) return detection_state::invalid;
        } else {
            if (!std::is_same<test_type, F>::value) return detection_state::invalid;
        }
        return detection_state::valid;
    }
    template <typename E, typename F, typename VT>
    constexpr detection_state has_extension_type(E* dummy) { return detection_state::undefined; }


    template <typename VT>
    constexpr typename VT::functor_type get_functor(typename VT::functor_type* dummy) {
        return typename VT::functor_type();
    };
    template <typename VT>
    constexpr dummy_functor_type get_functor(VT* dummy) { return dummy_functor_type(); };

} // namespace common


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>>
constexpr detection_state has_traits(decltype(VT::detected)*) {
    if (!std::is_same<VT, ST>::value) return detection_state::invalid;
    return common::has_traits<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_traits(E*) {
    return detection_state::undefined;
}


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>>
constexpr detection_state has_enclosing_type(typename VT::enclosing_type* dummy) {
    using test_type = typename VT::enclosing_type;
    if (!std::is_same<test_type, ST>::value) return detection_state::invalid;
    return common::has_enclosing_type<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_enclosing_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>>
constexpr detection_state has_fallback_type(typename VT::fallback_type* dummy) {
    using test_type = typename VT::fallback_type;
    if (!std::is_same<test_type, ST>::value) return detection_state::invalid;
    return common::has_fallback_type<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_fallback_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, typename VT = V<E, F>>
constexpr detection_state has_is_using_fallback(typename V<E, F>::selected_type* dummy) {
    return common::has_is_using_fallback<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V>
constexpr detection_state has_is_using_fallback(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>, typename test_type = typename VT::selected_type>
constexpr detection_state has_selected_type(typename VT::selected_type* dummy) {
    if (!std::is_same<test_type, ST>::value) return detection_state::invalid;
    return common::has_selected_type<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_selected_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>, typename test_type = typename VT::derived_type>
constexpr detection_state has_derived_type(typename VT::derived_type* dummy) {
    if (!std::is_same<test_type, ST>::value) return detection_state::invalid;
    return common::has_derived_type<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_derived_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>, typename test_type = typename VT::extension_type>
constexpr detection_state has_extension_type(typename VT::extension_type* dummy) {
    if (!std::is_same<test_type, ST>::value) return detection_state::invalid;
    return common::has_extension_type<E, F, VT>(nullptr);
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_extension_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S, typename VT = V<E, F>, typename ST = S<E, F>>
constexpr detection_state has_functor_type(typename VT::functor_type* dummy) {
    if (!std::is_same<typename VT::functor_type, ST>::value) return detection_state::invalid;
    return detection_state::valid;
}
template <typename E, typename F, template <typename, typename> class V, template <typename, typename> class S>
constexpr detection_state has_functor_type(E* dummy) { return detection_state::undefined; }


template <typename E, typename F, template <typename, typename> class V, typename A>
constexpr detection_state test_functor(typename V<E, F>::functor_type* dummy, bool (*test_fn)(decltype(std::declval<typename V<E, F>::functor_type>()(std::declval<std::initializer_list<A>>()))), std::initializer_list<A> list) {
    auto result = test_fn( typename V<E, F>::functor_type()(std::forward<std::initializer_list<A>>(list)) );
    return result ? detection_state::valid : detection_state::invalid;
};
template <typename E, typename F, template <typename, typename> class V, typename... Args>
constexpr detection_state test_functor(typename V<E, F>::functor_type* dummy, bool (*test_fn)(decltype(std::declval<typename V<E, F>::functor_type>()(std::declval<Args>()...))), Args... args) {
    auto result = test_fn( typename V<E, F>::functor_type()(std::forward<Args>(args)...) );
    return result ? detection_state::valid : detection_state::invalid;
};
template <typename E, typename F, template <typename, typename> class V>
constexpr detection_state test_functor(E* dummy, ...) {
    INFO("Could not test functor!");
    return detection_state::undefined;
};


} // namespace stti_test

#endif //adl__stti__stti__unit__hpp__test__
