// $flisboac 2018-02-15
/**
 * @file base.hpp
 */
#ifndef adl__stti__base__hpp__
#define adl__stti__base__hpp__

#include "adl.cfg.hpp"
#include "adl/stti.fwd.hpp"

adl_BEGIN_ROOT_MODULE

template <bool Found, lang_element_kind Kind, lang_element_kind FallbackKind>
struct conditional_lang_elem_kind {
    constexpr static auto const kind = Kind;
};
template <lang_element_kind Kind, lang_element_kind FallbackKind>
struct conditional_lang_elem_kind<false, Kind, FallbackKind> {
    constexpr static auto const kind = FallbackKind;
};

adl_END_ROOT_MODULE

adl_IMPL ::adl::lang_element_flag operator | (::adl::lang_element_flag const& a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}
adl_IMPL ::adl::lang_element_flag operator | (::adl::lang_element_flag const& a, ::adl::lang_element_kind b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}
adl_IMPL ::adl::lang_element_flag operator | (::adl::lang_element_kind const& a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) | int(b));
}

adl_IMPL ::adl::lang_element_flag operator & (::adl::lang_element_flag const& a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}
adl_IMPL ::adl::lang_element_flag operator & (::adl::lang_element_flag const& a, ::adl::lang_element_kind b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}
adl_IMPL ::adl::lang_element_flag operator & (::adl::lang_element_kind const& a, ::adl::lang_element_flag b) {
    return static_cast<::adl::lang_element_flag>(int(a) & int(b));
}

#endif //adl__stti__base__hpp__
