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
        template <typename U = T> using make_static_member_type = make_return_type<U> (*)(FArgs...); \
        template <typename U = T> using make_mut_member_type = make_return_type<U> (U::*)(FArgs...); \
        template <typename U = T> using make_const_member_type = make_return_type<U> (U::*)(FArgs...) const; \
        template <typename U = T> using make_cv_member_type = make_return_type<U> (U::*)(FArgs...) const volatile; \
        template <typename U = T> constexpr static make_static_member_type<U> make_static_pointer() noexcept { return &U::member_name; } \
        template <typename U = T> constexpr static make_mut_member_type<U> make_mut_member_pointer() noexcept { return &U::member_name; } \
        template <typename U = T> constexpr static make_const_member_type<U> make_const_member_pointer() noexcept { return &U::member_name; } \
        template <typename U = T> constexpr static make_cv_member_type<U> make_cv_member_pointer() noexcept { return &U::member_name; } \
    private: \
        template <typename U, template <typename> class MT, ::adl::lang_element_kind Kind = ::adl::lang_element_kind::none> \
        struct make_type_ { \
            using type = U; \
            template <typename V> using make_type = MT<V>; \
            constexpr static ::adl::lang_element_kind const kind = Kind; \
            template <typename V> constexpr static make_type<V> make_pointer() noexcept { return &V::member_name; } \
        }; \
        template <typename U> using make_nonesuch_ = ::adl::nonesuch; \
        template <typename U> using make_nonesuch_type_ = make_type_<::adl::nonesuch, make_nonesuch_>; \
        template<typename U, make_static_member_type<U>> struct detect_member_impl_static_ { using type = make_type_<make_static_member_type<U>, make_static_member_type, ::adl::lang_element_kind::static_member_function>; }; \
        template<typename U, make_mut_member_type<U>> struct detect_member_impl_mut_ { using type = make_type_<make_mut_member_type<U>, make_mut_member_type, ::adl::lang_element_kind::member_function>; }; \
        template<typename U, make_const_member_type<U>> struct detect_member_impl_const_ { using type = make_type_<make_const_member_type<U>, make_const_member_type, ::adl::lang_element_kind::member_function>; }; \
        template<typename U, make_cv_member_type<U>> struct detect_member_impl_cv_ { using type = make_type_<make_cv_member_type<U>, make_cv_member_type, ::adl::lang_element_kind::member_function>; }; \
        template <typename U> static typename detect_member_impl_static_<U, &U::member_name>::type detect_member_static_(detect_member_impl_static_<U, &U::member_name>*); \
        template <typename U> static make_nonesuch_type_<U> detect_member_static_(U*); \
        template <typename U> static typename detect_member_impl_mut_<U, &U::member_name>::type detect_member_mut_(detect_member_impl_mut_<U, &U::member_name>*); \
        template <typename U> static make_nonesuch_type_<U> detect_member_mut_(U*); \
        template <typename U> static typename detect_member_impl_const_<U, &U::member_name>::type detect_member_const_(detect_member_impl_const_<U, &U::member_name>*); \
        template <typename U> static make_nonesuch_type_<U> detect_member_const_(U*); \
        template <typename U> static typename detect_member_impl_cv_<U, &U::member_name>::type detect_member_cv_(detect_member_impl_cv_<U, &U::member_name>*); \
        template <typename U> static make_nonesuch_type_<U> detect_member_cv_(U*); \
        template <typename U> using detect_type_ = \
            typename std::conditional< \
            (decltype(detector_name::detect_member_static_<U>(nullptr))::kind != ::adl::lang_element_kind::none), \
                decltype(detector_name::detect_member_static_<U>(nullptr)), \
                typename std::conditional< \
                (decltype(detector_name::detect_member_mut_<U>(nullptr))::kind != ::adl::lang_element_kind::none), \
                    decltype(detector_name::detect_member_mut_<U>(nullptr)), \
                    typename std::conditional< \
                    (decltype(detector_name::detect_member_const_<U>(nullptr))::kind != ::adl::lang_element_kind::none), \
                        decltype(detector_name::detect_member_const_<U>(nullptr)), \
                        decltype(detector_name::detect_member_cv_<U>(nullptr)) \
                    >::type \
                >::type \
            >::type; \
        using type_ = detect_type_<T>; \
    public: \
        constexpr static char const* const name = #member_name; \
        template <typename U> using detect_type = typename detect_type_<U>::type; \
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
