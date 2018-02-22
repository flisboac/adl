// $flisboac 2018-02-15
/**
 * @file base.hpp
 */
#ifndef adl__stti__base__hpp__
#define adl__stti__base__hpp__

#include "adl.cfg.hpp"
#include "adl/stti.fwd.hpp"

#define adl_STTI_M_GET_NTH_(_1, _2, _3, _4, _5, _6, _7, _8, _N, ...) _N
#define adl_STTI_M_UNPAREN(a) a

#define adl_STTI_M_GEN_TPARAMS_ELEM_(a, b) adl_STTI_M_UNPAREN a b
#define adl_STTI_M_GEN_TARGS_ELEM_(a, b) b

#define adl_STTI_M_GEN_TPARAMS_01(_F, _1, ...) _F(_1, _a1)
#define adl_STTI_M_GEN_TPARAMS_02(_F, _1, _2, ...) adl_STTI_M_GEN_TPARAMS_01(_F, _1), _F(_2, _a2)
#define adl_STTI_M_GEN_TPARAMS_03(_F, _1, _2, _3, ...) adl_STTI_M_GEN_TPARAMS_02(_F, _1, _2), _F(_3, _a3)
#define adl_STTI_M_GEN_TPARAMS_04(_F, _1, _2, _3, _4, ...) adl_STTI_M_GEN_TPARAMS_03(_F, _1, _2, _3), _F(_4, _a4)
#define adl_STTI_M_GEN_TPARAMS_05(_F, _1, _2, _3, _4, _5, ...) adl_STTI_M_GEN_TPARAMS_04(_F, _1, _2, _3, _4), _F(_5, _a5)
#define adl_STTI_M_GEN_TPARAMS_06(_F, _1, _2, _3, _4, _5, _6, ...) adl_STTI_M_GEN_TPARAMS_05(_F, _1, _2, _3, _4, _5), _F(_6, _a6)
#define adl_STTI_M_GEN_TPARAMS_07(_F, _1, _2, _3, _4, _5, _6, _7, ...) adl_STTI_M_GEN_TPARAMS_06(_F, _1, _2, _3, _4, _5, _6), _F(_7, _a7)
#define adl_STTI_M_GEN_TPARAMS_08(_F, _1, _2, _3, _4, _5, _6, _7, _8, ...) adl_STTI_M_GEN_TPARAMS_07(_F, _1, _2, _3, _4, _5, _6, _7), _F(_8, _a8)
#define adl_STTI_M_GEN_TPARAMS_N(...) adl_STTI_M_GET_NTH_(__VA_ARGS__, \
	adl_STTI_M_GEN_TPARAMS_08, adl_STTI_M_GEN_TPARAMS_07, adl_STTI_M_GEN_TPARAMS_06, adl_STTI_M_GEN_TPARAMS_05, \
	adl_STTI_M_GEN_TPARAMS_04, adl_STTI_M_GEN_TPARAMS_03, adl_STTI_M_GEN_TPARAMS_02, adl_STTI_M_GEN_TPARAMS_01)

#define adl_STTI_M_GEN_TPARAMS(...) adl_STTI_M_GEN_TPARAMS_N(__VA_ARGS__)(adl_STTI_M_GEN_TPARAMS_ELEM_, __VA_ARGS__)
#define adl_STTI_M_GEN_TARGS(...) adl_STTI_M_GEN_TPARAMS_N(__VA_ARGS__)(adl_STTI_M_GEN_TARGS_ELEM_, __VA_ARGS__)


adl_BEGIN_ROOT_MODULE

template <bool Found, lang_element_kind Kind, lang_element_kind FallbackKind>
struct conditional_lang_elem_kind {
    constexpr static auto const kind = Kind;
};
template <lang_element_kind Kind, lang_element_kind FallbackKind>
struct conditional_lang_elem_kind<false, Kind, FallbackKind> {
    constexpr static auto const kind = FallbackKind;
};
template <bool Found, lang_element_flag Flags, lang_element_flag FallbackFlags>
struct conditional_lang_elem_flag {
    constexpr static auto const flags = Flags;
};
template <lang_element_flag Flags, lang_element_flag FallbackFlags>
struct conditional_lang_elem_flag<false, Flags, FallbackFlags> {
    constexpr static auto const flags = FallbackFlags;
};

adl_END_ROOT_MODULE

constexpr ::adl::lang_element_flag operator | (::adl::lang_element_flag a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}
constexpr ::adl::lang_element_flag operator | (::adl::lang_element_flag a, ::adl::lang_element_kind b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}
constexpr ::adl::lang_element_flag operator | (::adl::lang_element_kind a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}

constexpr ::adl::lang_element_flag operator & (::adl::lang_element_flag a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}
constexpr ::adl::lang_element_flag operator & (::adl::lang_element_flag a, ::adl::lang_element_kind b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}
constexpr ::adl::lang_element_flag operator & (::adl::lang_element_kind a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}

#endif //adl__stti__base__hpp__
