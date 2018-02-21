// $flisboac 2018-02-15
/**
 * @file has_member_type.hpp
 */
#ifndef adl__stti__has_member_type__hpp__
#define adl__stti__has_member_type__hpp__

#include "adl/stti/base.hpp"

#define adl_MAKE_HAS_MEMBER_TYPE_NAME(member_name) has_member_type_ ## member_name
#define adl_CREATE_HAS_MEMBER_TYPE(member_name) adl_DECL_HAS_MEMBER_TYPE(member_name); adl_DEFN_HAS_MEMBER_TYPE(member_name)
#define adl_DECL_HAS_MEMBER_TYPE(member_name) adl_NDECL_HAS_MEMBER_TYPE(has_member_type_ ## member_name, member_name)
#define adl_DEFN_HAS_MEMBER_TYPE(member_name) adl_NDEFN_HAS_MEMBER_TYPE(has_member_type_ ## member_name, member_name)
#define adl_NCREATE_HAS_MEMBER_TYPE(detector_name, member_name) adl_NDECL_HAS_MEMBER_TYPE(detector_name, member_name); adl_NDEFN_HAS_MEMBER_TYPE(detector_name, member_name)
#define adl_NDECL_HAS_MEMBER_TYPE(detector_name, member_name) \
    template <typename T> struct detector_name; \
    template <typename T> using detector_name ## _alias_ = detector_name<T>; \
    template <typename T> using detector_name ## _t = std::enable_if_t<detector_name<T>::detected, typename detector_name<T>::type>
#define adl_NDEFN_HAS_MEMBER_TYPE(detector_name, member_name) \
    template <typename T> struct detector_name { \
        template <typename U = T> using make_type = typename U::member_name; \
    private: \
        template <typename U> static make_type<U> detect_(make_type<U>*); \
        template <typename U> static ::adl::nonesuch detect_(U*); \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = decltype(detector_name::detect_<U>(nullptr)); \
        using enclosing_type = T; \
        using type = detect_type<T>; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
        constexpr static ::adl::lang_element_kind const kind = ::adl::conditional_lang_elem_kind_v<detected, ::adl::lang_element_kind::member_type>; \
        template <typename U> using rebind = detector_name ## _alias_<U>; \
        template <typename O, typename U = T, typename None = ::adl::nonesuch> using select_type = ::adl::common_select_type< \
            detector_name ## _alias_<U>::detected, typename detector_name ## _alias_<U>::type, \
            detector_name ## _alias_<O>::detected, typename detector_name ## _alias_<O>::type, \
            None>; \
        template <typename O, typename U = T, typename Empty = ::adl::empty_such> using extend_type = ::adl::common_extend_type< \
            detector_name ## _alias_<U>::detected, typename detector_name ## _alias_<U>::type, \
            detector_name ## _alias_<O>::detected, typename detector_name ## _alias_<O>::type, \
            Empty>; \
    }

#endif //adl__stti__has_member_type__hpp__
