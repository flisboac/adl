// $flisboac 2018-02-15
/**
 * @file stti.fwd.hpp
 */
#ifndef adl__stti__fwd__hpp__
#define adl__stti__fwd__hpp__

#include "adl.cfg.hpp"
#include "adl/std/detected.hpp"

adl_BEGIN_ROOT_MODULE

template <typename... Args> using nonesuch_t = nonesuch;
class empty_such {};

template <bool DetectedOnEnclosing, typename EnclosingType, bool DetectedOnFallback, typename FallbackType, typename NoneSuchType = nonesuch>
using common_select_type = typename std::conditional<DetectedOnEnclosing,
    EnclosingType,
    typename std::conditional<DetectedOnFallback,
        FallbackType,
        NoneSuchType
    >::type
>::type;

// Used when the enclosing class is somehow part of the inheritance tree from which we want to make sure a specific
// member is present.
template <bool DetectedOnEnclosing, typename EnclosingType, bool DetectedOnFallback, typename FallbackType, typename EmptyType = empty_such>
using common_extend_type = typename std::conditional<DetectedOnEnclosing,
    EmptyType,
    std::enable_if_t<DetectedOnFallback, FallbackType>
>::type;

enum class lang_element_flag {
    type = 0x1,
    function = 0x2,
    data = 0x3,
    member = 1 << 4,
    templated = 1 << 5,
    static_qualified = 1 << 6,

    // The difficult (if not impossible?) ones
    const_qualified = 1 << 7,
    volatile_qualified = 1 << 8,
    noexcept_qualified = 1 << 9,
    constexpr_qualified = 1 << 10
};

enum class lang_element_kind {
    none = 0,
    member_type = int(lang_element_flag::member) | int(lang_element_flag::type),
    template_member_type = int(lang_element_flag::templated) | int(lang_element_flag::member) | int(lang_element_flag::type),
    static_member_function = int(lang_element_flag::static_qualified) | int(lang_element_flag::member) | int(lang_element_flag::function),
    static_template_member_function = int(lang_element_flag::static_qualified) | int(lang_element_flag::templated) | int(lang_element_flag::member) | int(lang_element_flag::function),
    static_member_data = int(lang_element_flag::static_qualified) | int(lang_element_flag::member) | int(lang_element_flag::data),
    static_template_member_data = int(lang_element_flag::static_qualified) | int(lang_element_flag::templated) | int(lang_element_flag::member) | int(lang_element_flag::data),
    member_function = int(lang_element_flag::member) | int(lang_element_flag::function),
    template_member_function = int(lang_element_flag::templated) | int(lang_element_flag::member) | int(lang_element_flag::function),
    member_data = int(lang_element_flag::member) | int(lang_element_flag::data)
};

template <bool Found, lang_element_kind Kind, lang_element_kind FallbackKind = lang_element_kind::none> struct conditional_lang_elem_kind {
    constexpr static auto const kind = Kind;
};
template <lang_element_kind Kind, lang_element_kind FallbackKind> struct conditional_lang_elem_kind<false, Kind, FallbackKind> {
    constexpr static auto const kind = FallbackKind;
};
template <bool Found, lang_element_kind Kind, lang_element_kind FallbackKind = lang_element_kind::none>
lang_element_kind const conditional_lang_elem_kind_v = conditional_lang_elem_kind<Found, Kind, FallbackKind>::kind;

adl_END_ROOT_MODULE

#endif //adl__stti__fwd__hpp__
