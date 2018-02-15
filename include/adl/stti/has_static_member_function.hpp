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
    template <typename T, template... FArgs> using detector_name ## _t = std::enable_if_t<detector_name<T, FArgs...>::detected, typename detector_name<T, FArgs...>::type>
#define adl_NDEFN_HAS_STATIC_MEMBER_FUNCTION(detector_name, member_name) \
    template <typename T, typename... FArgs> struct detector_name { \
        template <typename U = T> using make_return_type = decltype(U::member_name(std::declval<FArgs>()...)); \
        template <typename U = T> using make_type = make_return_type<U> (*)(FArgs...); \
        template <typename U = T> static make_type<U> make_pointer() { return &U::member_name; } \
    private: \
        template <typename U> make_type<U> detect_(make_type<U>*); \
        template <typename U> ::adl::nonesuch detect_(U*); \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = decltype(detect_<U>(nullptr)); \
        using type = detect_type<T>; \
        constexpr static lang_element_kind kind = lang_element_kind::static_member_function; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
    }

#endif //adl__stti__has_static_member_function__hpp__
