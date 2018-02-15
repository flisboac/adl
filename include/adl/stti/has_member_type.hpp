// $flisboac 2018-02-15
/**
 * @file has_member_type.hpp
 */
#ifndef adl__stti__has_member_type__hpp__
#define adl__stti__has_member_type__hpp__

#include "adl/stti.fwd.hpp"

#define adl_MAKE_HAS_MEMBER_TYPE_NAME(member_name) has_MEMBER_TYPE_ ## member_name
#define adl_CREATE_HAS_MEMBER_TYPE(member_name) adl_DECL_HAS_MEMBER_TYPE(member_name); adl_DEFN_HAS_MEMBER_TYPE(member_name)
#define adl_DECL_HAS_MEMBER_TYPE(member_name) adl_NDECL_HAS_MEMBER_TYPE(adl_MAKE_HAS_MEMBER_TYPE_NAME(member_name), member_name)
#define adl_DEFN_HAS_MEMBER_TYPE(member_name) adl_NDEFN_HAS_MEMBER_TYPE(adl_MAKE_HAS_MEMBER_TYPE_NAME(member_name), member_name)
#define adl_NCREATE_HAS_MEMBER_TYPE(detector_name, member_name) adl_NDECL_HAS_MEMBER_TYPE(detector_name, member_name); adl_NDEFN_HAS_MEMBER_TYPE(detector_name, member_name)
#define adl_NDECL_HAS_MEMBER_TYPE(detector_name, member_name) \
    template <typename T> struct detector_name; \
    template <typename T> using detector_name ## _t = std::enable_if_t<detector_name<T>::detected, typename detector_name<T>::type>
#define adl_NDEFN_HAS_MEMBER_TYPE(detector_name, member_name) \
    template <typename T> struct detector_name { \
        template <typename U = T> using make_type = typename U::member_name; \
    private: \
        template <typename U> make_type<U> detect_(make_type<U>*); \
        template <typename U> ::adl::nonesuch detect_(U*); \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = decltype(detect_<U>(nullptr)); \
        using type = detect_type<T>; \
        constexpr static lang_element_kind kind = lang_element_kind::member_type; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
    }

#endif //adl__stti__has_member_type__hpp__
