// $flisboac 2018-02-15
/**
 * @file has_template_member_type.hpp
 */
#ifndef adl__stti__has_template_member_type__hpp__
#define adl__stti__has_template_member_type__hpp__

#include "adl/stti/base.hpp"

#define adl_MAKE_HAS_TEMPLATE_MEMBER_TYPE_NAME(member_name) has_template_member_type_ ## member_name
#define adl_CREATE_HAS_TEMPLATE_MEMBER_TYPE(member_name, template_args) adl_DECL_HAS_TEMPLATE_MEMBER_TYPE(member_name, template_args); adl_DEFN_HAS_TEMPLATE_MEMBER_TYPE(member_name, template_args)
#define adl_DECL_HAS_TEMPLATE_MEMBER_TYPE(member_name, template_args) adl_NDECL_HAS_TEMPLATE_MEMBER_TYPE(has_template_member_type_ ## member_name, member_name, template_args)
#define adl_DEFN_HAS_TEMPLATE_MEMBER_TYPE(member_name, template_args) adl_NDEFN_HAS_TEMPLATE_MEMBER_TYPE(has_template_member_type_ ## member_name, member_name, template_args)
#define adl_NCREATE_HAS_TEMPLATE_MEMBER_TYPE(detector_name, member_name, template_args) adl_NDECL_HAS_TEMPLATE_MEMBER_TYPE(detector_name, member_name, template_args); adl_NDEFN_HAS_TEMPLATE_MEMBER_TYPE(detector_name, member_name, template_args)
#define adl_NDECL_HAS_TEMPLATE_MEMBER_TYPE(detector_name, member_name, template_args) \
    template <typename T, adl_STTI_M_GEN_TPARAMS template_args, typename... TArgs> struct detector_name; \
    template <typename T, adl_STTI_M_GEN_TPARAMS template_args, typename... TArgs> using detector_name ## _alias_ = detector_name<T, adl_STTI_M_GEN_TARGS template_args, TArgs...>; \
    template <typename T, adl_STTI_M_GEN_TPARAMS template_args, typename... TArgs> using detector_name ## _t = std::enable_if_t<detector_name<T, adl_STTI_M_GEN_TARGS template_args, TArgs...>::detected, typename detector_name<T, adl_STTI_M_GEN_TARGS template_args, TArgs...>::type>
#define adl_NDEFN_HAS_TEMPLATE_MEMBER_TYPE(detector_name, member_name, template_args) \
    template <typename T, adl_STTI_M_GEN_TPARAMS template_args, typename... TArgs> struct detector_name { \
        template <typename U = T> using make_nopack_type = typename U::template member_name<adl_STTI_M_GEN_TARGS template_args>; \
        template <typename U = T> using make_pack_type = typename U::template member_name<adl_STTI_M_GEN_TARGS template_args, TArgs...>; \
    protected: \
        template <typename U, typename... FArgs> static std::enable_if_t<0 < sizeof...(FArgs), make_pack_type<U>> detect_(make_pack_type<U>*); \
        template <typename U, typename... FArgs> static std::enable_if_t<0 == sizeof...(FArgs), make_nopack_type<U>> detect_(make_nopack_type<U>*); \
        template <typename U, typename... FArgs> static ::adl::nonesuch detect_(U*); \
    public: \
        constexpr static char const* const name = #member_name ; \
        template <typename U> using detect_type = decltype(detector_name::detect_<U, TArgs...>(nullptr)); \
        template <typename U = T> using make_type = std::enable_if_t<!std::is_same<detect_type<U>, ::adl::nonesuch>::value, detect_type<U>>; \
        using enclosing_type = T; \
        using type = detect_type<T>; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
        constexpr static ::adl::lang_element_kind const kind = ::adl::conditional_lang_elem_kind_v<detected, ::adl::lang_element_kind::template_member_type>; \
        constexpr static ::adl::lang_element_flag const flags = kind | ::adl::lang_element_flag::none; \
        template <typename U> using rebind = detector_name ## _alias_<U, adl_STTI_M_GEN_TARGS template_args, TArgs...>; \
        template <typename O, typename U = T, typename None = ::adl::nonesuch> using select_type = ::adl::common_select_type< \
            detector_name ## _alias_<U, adl_STTI_M_GEN_TARGS template_args, TArgs...>::detected, typename detector_name ## _alias_<U, adl_STTI_M_GEN_TARGS template_args, TArgs...>::type, \
            detector_name ## _alias_<O, adl_STTI_M_GEN_TARGS template_args, TArgs...>::detected, typename detector_name ## _alias_<O, adl_STTI_M_GEN_TARGS template_args, TArgs...>::type, \
            None>; \
        template <typename O, typename U = T, typename Empty = ::adl::empty_such> using extend_type = ::adl::common_extend_type< \
            detector_name ## _alias_<U, adl_STTI_M_GEN_TARGS template_args, TArgs...>::detected, typename detector_name ## _alias_<U, adl_STTI_M_GEN_TARGS template_args, TArgs...>::type, \
            detector_name ## _alias_<O, adl_STTI_M_GEN_TARGS template_args, TArgs...>::detected, typename detector_name ## _alias_<O, adl_STTI_M_GEN_TARGS template_args, TArgs...>::type, \
            Empty>; \
    }

#endif //adl__stti__has_template_member_type__hpp__
