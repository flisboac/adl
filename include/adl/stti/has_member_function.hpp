// $flisboac 2018-02-15
/**
 * @file has_member_function.hpp
 */
#ifndef adl__stti__has_member_function__hpp__
#define adl__stti__has_member_function__hpp__

#include "adl/stti/base.hpp"

#define adl_MAKE_HAS_MEMBER_FUNCTION_NAME(member_name) has_member_function_ ## member_name
#define adl_CREATE_HAS_MEMBER_FUNCTION(member_name) adl_DECL_HAS_MEMBER_FUNCTION(member_name); adl_DEFN_HAS_MEMBER_FUNCTION(member_name)
#define adl_DECL_HAS_MEMBER_FUNCTION(member_name) adl_NDECL_HAS_MEMBER_FUNCTION(has_member_function_ ## member_name, member_name)
#define adl_DEFN_HAS_MEMBER_FUNCTION(member_name) adl_NDEFN_HAS_MEMBER_FUNCTION(has_member_function_ ## member_name, member_name)
#define adl_NCREATE_HAS_MEMBER_FUNCTION(detector_name, member_name) adl_NDECL_HAS_MEMBER_FUNCTION(detector_name, member_name); adl_NDEFN_HAS_MEMBER_FUNCTION(detector_name, member_name)
#define adl_NDECL_HAS_MEMBER_FUNCTION(detector_name, member_name) \
    template <typename T, typename... FArgs> struct detector_name; \
    template <typename T, typename... FArgs> using detector_name ## _alias_ = detector_name<T, FArgs...>; \
    template <typename T, typename... FArgs> using detector_name ## _t = std::enable_if_t<detector_name<T, FArgs...>::detected, typename detector_name<T, FArgs...>::type>
#define adl_NDEFN_HAS_MEMBER_FUNCTION(detector_name, member_name) \
    template <typename T, typename... FArgs> struct detector_name { \
        template <typename U = T> using make_return_type = decltype(std::declval<U>().member_name(std::declval<FArgs>()...)); \
        template <typename U = T> using make_member_type = make_return_type<U> (U::*)(FArgs...); \
        template <typename U = T> using make_static_type = make_return_type<U> (*)(FArgs...); \
        template <typename U = T> constexpr static make_member_type<U> make_member_pointer() noexcept { return &U::member_name; } \
        template <typename U = T> constexpr static make_static_type<U> make_static_pointer() noexcept { return &U::member_name; } \
    private: \
        template <typename U, template <typename> class MT, ::adl::lang_element_kind Kind = ::adl::lang_element_kind::none> \
        struct make_type_ { \
            using type = MT<U>; \
            constexpr static ::adl::lang_element_kind const kind = Kind; \
            template <typename V> constexpr static MT<V> make_pointer() noexcept { return &V::member_name; } \
        }; \
        template <typename U> using make_nonesuch_ = ::adl::nonesuch; \
        template <typename U> using make_nonesuch_type_ = make_type_<U, make_nonesuch_>; \
        template <typename U> static make_type_<U, make_member_type, ::adl::lang_element_kind::member_function> detect_(make_type_<U, make_member_type>*); \
        template <typename U> static make_type_<U, make_static_type, ::adl::lang_element_kind::static_member_function> detect_(make_type_<U, make_static_type>*); \
        template <typename U> static make_nonesuch_type_<U> detect_(U*); \
        using type_ = decltype(detect_<T>(nullptr)); \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = typename decltype(detector_name::detect_<U>(nullptr))::type; \
        using enclosing_type = T; \
        using type = typename type_::type; \
        constexpr static bool const detected = !std::is_same<type, ::adl::nonesuch>::value; \
        constexpr static ::adl::lang_element_kind const kind = ::adl::conditional_lang_elem_kind_v<detected, type_::kind>; \
        template <typename U> using rebind = detector_name ## _alias_<U, FArgs...>; \
        template <typename O, typename U = T, typename None = ::adl::nonesuch> using select_type = ::adl::common_select_type< \
            detector_name ## _alias_<U, FArgs...>::detected, typename detector_name ## _alias_<U, FArgs...>::type, \
            detector_name ## _alias_<O, FArgs...>::detected, typename detector_name ## _alias_<O, FArgs...>::type, \
            None>; \
        template <typename O, typename U = T, typename Empty = ::adl::empty_such> using extend_type = ::adl::common_extend_type< \
            detector_name ## _alias_<U, FArgs...>::detected, typename detector_name ## _alias_<U, FArgs...>::type, \
            detector_name ## _alias_<O, FArgs...>::detected, typename detector_name ## _alias_<O, FArgs...>::type, \
            Empty>; \
        template <typename U = T> constexpr static auto make_pointer() noexcept { return type_::template make_pointer<U>(); } \
        template <typename U = T> using make_type = std::enable_if_t< \
            !std::is_same<detect_type<U>, ::adl::nonesuch>::value, \
            detect_type<U>>; \
    }

#endif //adl__stti__has_member_function__hpp__
