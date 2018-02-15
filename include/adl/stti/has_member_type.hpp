// $flisboac 2018-02-12
/**
 * @file has_member_type.hpp
 */
#ifndef adl__stti__has_member_type__hpp__
#define adl__stti__has_member_type__hpp__

#include <type_traits>
#include <utility>

#define adl_STTI_HAS_MEMBER_TYPE_COMMON_VERSION 1

#ifndef adl_STTI_DEFINED
    #define adl_STTI_DEFINED adl_STTI_HAS_MEMBER_TYPE_COMMON_VERSION

    // For use with adl_DECL_FRIEND_HAS_MEMBER_TYPE_ and the like

    #define adl_STTI_IN_GLOBAL_SCOPE
    #define adl_STTI_IN_NAMESPACE(location) location
    #define adl_STTI_IN_CLASS(location) typename location
#endif

#define adl_CREATE_HAS_MEMBER_TYPE_(member_name) adl_DECL_HAS_MEMBER_TYPE_(member_name); adl_DEFN_HAS_MEMBER_TYPE_(member_name)
#define adl_DECL_FRIEND_HAS_MEMBER_TYPE_(location, member_name) adl_NAMED_DECL_FRIEND_HAS_MEMBER_TYPE_(location, has_member_type_ ## member_name, member_name)
#define adl_DECL_HAS_MEMBER_TYPE_(member_name) adl_NAMED_DECL_HAS_MEMBER_TYPE_(has_member_type_ ## member_name, member_name)
#define adl_DEFN_HAS_MEMBER_TYPE_(member_name) adl_NAMED_DEFN_HAS_MEMBER_TYPE_(has_member_type_ ## member_name, member_name)

#define adl_NAMED_CREATE_HAS_MEMBER_TYPE_(traits_name, member_name) adl_NAMED_DECL_HAS_MEMBER_TYPE_(traits_name, member_name); adl_NAMED_DEFN_HAS_MEMBER_TYPE_(traits_name, member_name)
#define adl_NAMED_DECL_FRIEND_HAS_MEMBER_TYPE_(location, traits_name, member_name) \
    template <typename E, typename F> friend class location::traits_name; \
    template <typename E, typename F> template <typename... Args> friend class location::traits_name<E, F>::with_signature

#define adl_NAMED_DECL_HAS_MEMBER_TYPE_(traits_name, member_name) \
    class traits_name ## _detail_; \
    template <typename E, typename F = void> class traits_name; \
    \
    template <typename E, typename F = void> constexpr const bool traits_name ## _v = traits_name<E, F>::detected; \
    template <typename E, typename F = void> constexpr const bool traits_name ## _fallback_v = traits_name<E, F>::is_using_fallback; \
    template <typename E, typename F = void> using traits_name ## _t = std::enable_if_t<traits_name<E, F>::detected, traits_name<E, F>>; \
    template <typename E, typename F = void> using traits_name ## _enclosing_t = typename traits_name<E, F>::enclosing_type; \
    template <typename E, typename F = void> using traits_name ## _fallback_t = typename traits_name<E, F>::fallback_type; \
    template <typename E, typename F = void> using traits_name ## _type_t = typename traits_name<E, F>::type; \
    template <typename E, typename F = void> using traits_name ## _selected_t = typename traits_name<E, F>::selected_type; \
    template <typename E, typename F = void> using traits_name ## _derived_t = typename traits_name<E, F>::derived_type; \
    template <typename E, typename F = void> using traits_name ## _extension_t = typename traits_name<E, F>::extension_type; \
    template <typename E, typename F = void> using traits_name ## _functor_t = typename traits_name<E, F>::functor_type; \
    \
    template <typename E, typename F = void, typename... Ps> constexpr const bool traits_name ## _signature_v = traits_name<E, F>::template with_signature<Ps...>::detected; \
    template <typename E, typename F = void, typename... Ps> constexpr const bool traits_name ## _signature_fallback_v = traits_name<E, F>::template with_signature<Ps...>::is_using_fallback; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature = typename traits_name<E, F>::template with_signature<Ps...>; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_t = std::enable_if_t<traits_name<E, F>::template with_signature<Ps...>::detected, typename traits_name<E, F>::template with_signature<Ps...>>; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_enclosing_t = typename traits_name<E, F>::template with_signature<Ps...>::enclosing_type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_fallback_t = typename traits_name<E, F>::template with_signature<Ps...>::fallback_type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_type_t = typename traits_name<E, F>::template with_signature<Ps...>::type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_selected_t = typename traits_name<E, F>::template with_signature<Ps...>::selected_type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_derived_t = typename traits_name<E, F>::template with_signature<Ps...>::derived_type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_extension_t = typename traits_name<E, F>::template with_signature<Ps...>::extension_type; \
    template <typename E, typename F = void, typename... Ps> using traits_name ## _signature_functor_t = typename traits_name<E, F>::template with_signature<Ps...>::functor_type

#define adl_NAMED_DEFN_HAS_MEMBER_TYPE_(traits_name, member_name) \
    struct traits_name ## _detail_ { \
        template <typename EnclosingType, typename FallbackType> friend class traits_name; \
        \
        class undefined_ {}; \
        class empty_ {}; \
        template <typename E, typename F, bool EbaseF = std::is_base_of<E, F>::value, bool FbaseE = std::is_base_of<F, E>::value> struct derived_ : E, F {}; \
        /* Ignore F       */ template <typename E, bool EbaseF, bool FbaseE> struct derived_<E, void, EbaseF, FbaseE> : E {}; \
        /* E is base of F */ template <typename E, typename F> struct derived_<E, F, true, false> : F {}; \
        /* F is base of E */ template <typename E, typename F> struct derived_<E, F, false, true> : E {}; \
        /* E is same of F */ template <typename E, typename F> struct derived_<E, F, true, true> : E {}; \
        \
        template <typename E, typename F, bool ED, bool FD> \
        struct traits_base_ { \
            constexpr static bool const detected = false; \
        }; \
        template <typename E, typename F, bool FD> \
        struct traits_base_<E, F, true, FD> { \
            constexpr static bool const is_using_fallback = false; \
            using enclosing_type = E; \
            using derived_type = derived_<E, void>; \
            using extension_type = empty_; \
        }; \
        template <typename E, typename F> \
        struct traits_base_<E, F, false, true> { \
            constexpr static bool const is_using_fallback = true; \
            using fallback_type = F; \
            using derived_type = derived_<E, F>; \
            using extension_type = typename std::conditional<!std::is_base_of<F, E>::value, F, empty_>::type; \
        }; \
        template <typename T> \
        struct simple_detector_ { \
        private: \
            template <typename U> static typename U::member_name detect_(typename U::member_name*); \
            template <typename U> static undefined_ detect_(U*); \
        protected: \
            template <typename U> struct make_functor_type_ { \
                template <typename... Args> typename U::name operator()(Args... args) { return {std::forward<Args>(args)...}; } \
                template <typename V> typename U::name operator()(std::initializer_list<V> list) { return typename U::name(list); } \
            }; \
        public: \
            constexpr static char const*const name = #member_name; \
            using selected_type = T; \
            using type = decltype(detect_<T>(nullptr)); \
            constexpr static bool const detected = !std::is_same<undefined_, type>::value; \
        }; \
        \
        template <typename E, typename F, bool ED, bool FD> \
        struct simple_traits_ : public traits_base_<E, F, ED, FD> { }; \
        \
        template <typename E, typename F, bool FD> \
        struct simple_traits_<E, F, true, FD> : public traits_base_<E, F, true, FD>, public simple_detector_<E> { \
            using functor_type = typename simple_detector_<E>::template make_functor_type_<E>; \
        }; \
        \
        template <typename E, typename F> \
        struct simple_traits_<E, F, false, true> : public traits_base_<E, F, false, true>, public simple_detector_<F> { \
            using functor_type = typename simple_detector_<F>::template make_functor_type_<F>; \
        }; \
        \
        template <typename T, typename... Args> \
        struct signature_detector_ { \
        protected: \
            template <typename U, typename... TArgs> using make_type_ = typename U::template member_name<TArgs...>; \
        private: \
            template <typename U, typename... PArgs> static make_type_<U, PArgs...> detect_(make_type_<U, PArgs...>*); \
            template <typename U, typename...> static undefined_ detect_(U*); \
        protected: \
            template <typename U> struct make_functor_type_ { \
                template <typename... CArgs> typename U::name operator()(CArgs... args) { return {std::forward<Args>(args)...}; } \
                template <typename V> typename U::name operator()(std::initializer_list<V> list) { return typename U::name(list); } \
            }; \
        public: \
            constexpr static char const*const name = #member_name; \
            using selected_type = T; \
            using type = decltype(detect_<T, Args...>(nullptr)); \
            constexpr static bool const detected = !std::is_same<undefined_, type>::value; \
        }; \
        \
        template <typename E, typename F, bool ED, bool FD, typename... PT> \
        struct signature_traits_ : public traits_base_<E, F, ED, FD> { }; \
        \
        template <typename E, typename F, bool FD, typename... PT> \
        struct signature_traits_<E, F, true, FD, PT...> : public traits_base_<E, F, true, FD>, public signature_detector_<E, PT...> { \
            template <typename... TArgs> using make_type = typename signature_detector_<E, PT...>::template make_type_<E, TArgs...>; \
            template <typename... TArgs> using make_functor_type = typename signature_detector_<E, PT...>::template make_functor_type_<make_type<TArgs...>>; \
            using functor_type = typename signature_detector_<E, PT...>::template make_functor_type<PT...>; \
        }; \
        \
        template <typename E, typename F, typename... PT> \
        struct signature_traits_<E, F, false, true, PT...> : public traits_base_<E, F, false, true>, public signature_detector_<F, PT...> { \
            template <typename... TArgs> using make_type = typename signature_detector_<F, PT...>::template make_type_<F, TArgs...>; \
            template <typename... TArgs> using make_functor_type = typename signature_detector_<F, PT...>::template make_functor_type_<make_type<TArgs...>>; \
            using functor_type = typename signature_detector_<F, PT...>::template make_functor_type<PT...>; \
        }; \
        \
        template <typename E, typename F> \
        struct traits_ : public simple_traits_<E, F, simple_detector_<E>::detected, simple_detector_<F>::detected> { \
            static_assert(!std::is_same<E, void>::value, "The enclosing type cannot be void."); \
            static_assert(!std::is_same<E, F>::value, "The enclosing type cannot be the same as the fallback type."); \
            template <typename... Sig> using with_signature = signature_traits_<E, F, \
                signature_detector_<E, Sig...>::detected, signature_detector_<F, Sig...>::detected, Sig...>; \
        }; \
    }; \
    \
    template <typename EnclosingType, typename FallbackType> class traits_name : public traits_name ## _detail_::traits_<EnclosingType, FallbackType> {}

#endif //adl__stti__has_member_type__hpp__
