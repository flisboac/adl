// $flisboac 2018-02-15
/**
 * @file has_static_member_function.hpp
 */
#ifndef adl__stti__has_static_member_function__hpp__
#define adl__stti__has_static_member_function__hpp__

#include "adl/stti.fwd.hpp"

#define adl_MAKE_HAS_STATIC_MEMBER_FUNCTION_NAME(member_name) has_STATIC_MEMBER_FUNCTION_ ## member_name
#define adl_CREATE_HAS_STATIC_MEMBER_FUNCTION(member_name) adl_DECL_HAS_STATIC_MEMBER_FUNCTION(member_name); adl_DEFN_HAS_STATIC_MEMBER_FUNCTION(member_name)
#define adl_DECL_HAS_STATIC_MEMBER_FUNCTION(member_name) adl_NDECL_HAS_STATIC_MEMBER_FUNCTION(adl_MAKE_HAS_STATIC_MEMBER_FUNCTION_NAME(member_name), member_name)
#define adl_DEFN_HAS_STATIC_MEMBER_FUNCTION(member_name) adl_NDEFN_HAS_STATIC_MEMBER_FUNCTION(adl_MAKE_HAS_STATIC_MEMBER_FUNCTION_NAME(member_name), member_name)
#define adl_NCREATE_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name) adl_NDECL_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name); adl_NDEFN_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name)
#define adl_NDECL_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name) \
    template <typename T, typename... FArgs> struct detector_name; \
    template <typename T, typename... FArgs> using detector_name ## _alias_ = detector_name<T, FArgs...>; \
    template <typename T, template... FArgs> using detector_name ## _t = std::enable_if_t<detector_name<T, FArgs...>::detected, typename detector_name<T, FArgs...>::type>
#define adl_NDEFN_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name) \
    template <typename T, typename... FArgs> struct detector_name { \
        template <typename U = T> using make_return_type = decltype(U::member_name(std::declval<FArgs>()...)); \
        template <typename U = T> using make_type = make_return_type<U> (*)(FArgs...); \
        template <typename U = T> constexpr static make_type<U> make_pointer() noexcept { return &U::member_name; } \
    private: \
        template <typename U> static make_type<U> detect_(make_type<U>*); \
        template <typename U> static ::adl::nonesuch detect_(U*); \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = decltype(detect_<U>(nullptr)); \
        using enclosing_type = T; \
        using type = detect_type<T>; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
        constexpr static lang_element_kind kind = ::adl::conditional_lang_elem_kind_v<detected, lang_element_kind::static_member_function>; \
        template <typename O, typename U = T, typename None = ::adl::nonesuch> using select_type = ::adl::common_select_type< \
            detector_name ## _alias_<U, FArgs...>::detected, typename detector_name ## _alias_<U, FArgs...>::type, \
            detector_name ## _alias_<O, FArgs...>::detected, typename detector_name ## _alias_<O, FArgs...>::type, \
            None>; \
        template <typename O, typename U = T, typename Empty = empty_type> using extend_type = ::adl::common_extend_type< \
            detector_name ## _alias_<U, FArgs...>::detected, typename detector_name ## _alias_<U, FArgs...>::type, \
            detector_name ## _alias_<O, FArgs...>::detected, typename detector_name ## _alias_<O, FArgs...>::type, \
            Empty>; \
    }

#endif //adl__stti__has_static_member_function__hpp__
