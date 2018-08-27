// $flisboac 2017-03-20
/**
 * @file assert.hpp
 */
#ifndef adl__assert__hpp__
#define adl__assert__hpp__

#include <type_traits>
#include <utility>

#include "adl.cfg.hpp"

#define adl_static_assert(_condition__) static_assert((_condition__), #_condition__)
#define adl_static_assert_constexpr(_expr__) static_assert( ((_expr__), true), "'" #_expr__ "' is not constexpr.")

adl_BEGIN_ROOT_MODULE

namespace assert {

template <bool Condition>
constexpr inline bool condition() {
    static_assert(Condition, "Static assertion failed.");
    return true;
}

template <bool (*ConditionFunction)(void)>
constexpr inline bool function() {
    static_assert(ConditionFunction(), "Static assertion failed.");
    return true;
}

#if 0
template <R (T::*Function)(Ps...), typename T, typename T, typename... Ps>
constexpr inline void constexpr_member_function() {
    static_assert(( (T().*Function)(Ps()...), true ), "");
};
#endif

template <typename T>
constexpr inline bool literal_class() {
    static_assert(std::is_trivially_destructible<T>::value,
        "Type is not trivially destructible.");
    return true;
}

template <typename T, typename T1, typename... Ts>
constexpr inline bool constructible() {
    static_assert(std::is_constructible<T, T1, Ts...>::value,
        "Type is not constructible from the specified set of types.");
    return true;
}

template <typename T, typename T1, typename... Ts>
constexpr inline bool literal_constructible() {
    static_assert(std::is_constructible<T, T1, Ts...>::value,
        "Type is not trivially constructible from the specified set of types.");
    static_assert((T(T1(), Ts()...), true), "");
    return true;
};

template <typename T>
constexpr inline bool constructible() {
    static_assert(std::is_default_constructible<T>::value,
        "Type is not default-constructible.");
    return true;
};

template <typename T>
constexpr inline bool literal_constructible() {
    static_assert(std::is_default_constructible<T>::value,
        "Type is not trivially default-constructible.");
    static_assert((T(), true), "");
    return true;
};

template <typename T>
constexpr inline bool moveable() {
    static_assert(std::is_move_constructible<T>::value,
        "Type is not move-constructible.");
    static_assert(std::is_move_assignable<T>::value,
        "Type is not move-assignable.");
    return true;
}

template <typename T>
constexpr inline bool literal_moveable() {
    static_assert(std::is_move_constructible<T>::value,
        "Type is not trivially move-constructible.");
    static_assert(std::is_move_assignable<T>::value,
        "Type is not trivially move-assignable.");
    static_assert(( T(T()), true ), "");
    return true;
}

template <typename T>
constexpr inline bool copyable() {
    static_assert(std::is_copy_constructible<T>::value,
        "Type is not move-constructible.");
    static_assert(std::is_copy_assignable<T>::value,
        "Type is not move-assignable.");
    return true;
}

template <typename T>
constexpr inline bool literal_copyable() {
    static_assert(std::is_trivially_copy_constructible<T>::value,
        "Type is not trivially move-constructible.");
    static_assert(std::is_trivially_copy_assignable<T>::value,
        "Type is not trivially move-assignable.");
    return true;
}

template <typename T, typename T1>
constexpr inline bool convertible() {
    static_assert(std::is_convertible<T, T1>::value,
        "Type is not convertible to the type parameter passed.");
    return true;
};

template <typename T, typename T1>
constexpr inline bool literal_convertible() {
    static_assert(std::is_convertible<T, T1>::value,
        "Type is not convertible to the type parameter passed.");
    static_assert(((T1) T(), true), "");
    return true;
};

template <typename T, typename T1>
constexpr inline bool seamlessly_convertible() {
    return (convertible<T, T1>() && convertible<T1, T>());
};

template <typename T, typename T1>
constexpr inline bool literal_seamlessly_convertible() {
    return (literal_convertible<T, T1>() && literal_convertible<T1, T>());
};

template <typename T, typename T1>
constexpr inline bool explicitly_convertible() {
    static_assert(std::is_constructible<T, T1>::value && !std::is_convertible<T1, T>::value,
        "Type is not explicitly convertible to the given parameter type.");
    return true;
}

template <typename T, typename T1>
constexpr inline bool literal_explicitly_convertible() {
    static_assert(std::is_constructible<T, T1>::value && !std::is_convertible<T1, T>::value,
        "Type is not explicitly convertible to the given parameter type.");
    static_assert(((T1) T(), true), "");
    return true;
}

template <typename T, typename T1>
constexpr inline bool seamlessly_explicitly_convertible() {
    return (explicitly_convertible<T, T1>() && explicitly_convertible<T1, T>());
};

template <typename T, typename T1>
constexpr inline bool literal_seamlessly_explicitly_convertible() {
    return (literal_explicitly_convertible<T, T1>() && literal_explicitly_convertible<T1, T>());
};

template <typename T, typename T1>
constexpr bool assignable() {
    static_assert(std::is_assignable<T, T1>::value,
        "Type is not assignable.");
    return true;
};

template <typename T, typename T1>
constexpr bool literal_assignable() {
    assignable<T, T1>();
    static_assert( (T() = T1(), true), "");
    return true;
};

}

adl_END_ROOT_MODULE

#endif // adl__assert__hpp__
