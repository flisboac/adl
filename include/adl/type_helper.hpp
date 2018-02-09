// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file type_helper.hpp
 */

#ifndef adl__type_helper__hpp__
#define adl__type_helper__hpp__

#include <type_traits>

#include "adl.cfg.hpp"

#ifdef adl_BUILDING_DOCS

#define adl_CREATE_HAS_TRAITS_MEMBER(name)
#define adl_DECLARE_HAS_TRAITS_MEMBER(name)
#define adl_DEFINE_HAS_TRAITS_MEMBER(name)

#define adl_CREATE_NAMED_HAS_TRAITS_MEMBER(detector_name, name)
#define adl_DECLARE_NAMED_HAS_TRAITS_MEMBER(detector_name, name)
#define adl_DEFINE_NAMED_HAS_TRAITS_MEMBER(detector_name, name)

#else

#define adl_COMMON_DECLARE_HAS_MEMBER(detector_name, name) \
    class detector_name ## _detail_; \
    template <typename EnclosingClass, typename FallbackType = void> class detector_name; \
    template <typename EnclosingClass, typename FallbackType = void> constexpr const bool detector_name ## _v = detector_name<EnclosingClass, FallbackType>::value; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _t = typename detector_name<EnclosingClass, FallbackType>::enclosing_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _fallback_t = typename detector_name<EnclosingClass, FallbackType>::fallback_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _selected_t = typename detector_name<EnclosingClass, FallbackType>::selected_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _derived_t = typename detector_name<EnclosingClass, FallbackType>::derived_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _extension_t = typename detector_name<EnclosingClass, FallbackType>::extension_type

/*
 * MEMBER FIELD DETECTION
 */

// A trait in this context is any static member field, static member function or member type, in order to be used in the
// context of a traits class.

#define adl_COMMON_DECLARE_HAS_MEMBER(detector_name, name) \
    class detector_name ## _detail_; \
    template <typename EnclosingClass, typename FallbackType = void> class detector_name; \
    template <typename EnclosingClass, typename FallbackType = void> constexpr const bool detector_name ## _v = detector_name<EnclosingClass, FallbackType>::value; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _t = typename detector_name<EnclosingClass, FallbackType>::enclosing_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _fallback_t = typename detector_name<EnclosingClass, FallbackType>::fallback_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _selected_t = typename detector_name<EnclosingClass, FallbackType>::selected_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _derived_t = typename detector_name<EnclosingClass, FallbackType>::derived_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _extension_t = typename detector_name<EnclosingClass, FallbackType>::extension_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _undefined_t = typename detector_name<EnclosingClass, FallbackType>::undefined_type; \
    template <typename EnclosingClass, typename FallbackType = void> using detector_name ## _type_t = typename detector_name<EnclosingClass, FallbackType>::type; \
    template <typename EnclosingClass, typename FallbackType = void, typename... Args> constexpr const bool detector_name ## _return_v = !std::is_same<typename detector_name<EnclosingClass, FallbackType>::template return_type<Args...>, detector_name ## _undefined_t<EnclosingClass, FallbackType>>::value; \
    template <typename EnclosingClass, typename FallbackType = void, typename... Args> using detector_name ## _return_t = std::enable_if_t<detector_name ## _return_v<EnclosingClass, FallbackType, Args...>, typename detector_name<EnclosingClass, FallbackType>::template return_type<Args...>>

/*
 * MEMBER FIELD DETECTION
 */

#define adl_CREATE_HAS_MEMBER_TRAIT(name) adl_CREATE_NAMED_HAS_MEMBER_TRAIT(has_trait_ ## name, name)
#define adl_DECLARE_HAS_MEMBER_TRAIT(name) adl_DECLARE_NAMED_HAS_MEMBER_TRAIT(has_trait_ ## name, name)
#define adl_DEFINE_HAS_MEMBER_TRAIT(name) adl_DEFINE_NAMED_HAS_MEMBER_TRAIT(has_trait_ ## name, name)

#define adl_CREATE_NAMED_HAS_MEMBER_TRAIT(detector_name, name) adl_DECLARE_NAMED_HAS_MEMBER_TRAIT(detector_name, name); adl_DEFINE_NAMED_HAS_MEMBER_TRAIT(detector_name, name)
#define adl_DECLARE_NAMED_HAS_MEMBER_TRAIT(detector_name, name) adl_COMMON_DECLARE_HAS_MEMBER(detector_name, name)
#define adl_DEFINE_NAMED_HAS_MEMBER_TRAIT(detector_name, name) \
    class detector_name ## _detail_ { \
        template <typename EnclosingClass, typename FallbackType> friend class detector_name; \
        template <typename EnclosingClass> \
        class impl { \
    	public: \
            class null_ { null_ operator()(...); }; \
        private: \
            using yes_ = char[2]; \
            using no_ = char[1]; \
            template<typename U> static yes_ & check_(decltype(U::name)*); /*static-member-{function,field}*/ \
            template<typename U> static yes_ & check_(typename U::name*); /*member-type*/ \
            template<typename U> static no_ & check_(U*); \
            template<typename U> static decltype(&U::name) get_function_type_(decltype(&U::name)*); \
            template<typename U> static null_ get_function_type_(U*); \
            template<typename U> static decltype(U::name) get_field_type_(decltype(U::name)*); \
            template<typename U> static null_ get_field_type_(U*); \
            template<typename U> static typename U::name get_type_type_(typename U::name*); \
            template<typename U> static null_ get_type_type_(U*); \
            template<typename U, typename... Args> static decltype(std::declval<U>().name(std::declval<Args>()...)) get_call_type_(decltype(&U::name)*); \
            template<typename U, typename... Args> static null_ get_call_type_(U*); \
        public: \
            using function_type = decltype(get_function_type_<EnclosingClass>(nullptr)); \
            using field_type = decltype(get_field_type_<EnclosingClass>(nullptr)); \
            using type_type = decltype(get_type_type_<EnclosingClass>(nullptr)); \
            using type = typename std::conditional<!std::is_same<type_type, null_>::value, type_type, typename std::conditional<!std::is_same<field_type, null_>::value, field_type, function_type>::type>::type; \
            template <typename... Args> using return_type = decltype(get_call_type_<EnclosingClass, Args...>(nullptr)); \
            constexpr static const bool value = sizeof(check_<EnclosingClass>(nullptr)) == sizeof(yes_); \
        }; \
        template <typename EnclosingClass, typename FallbackType, bool valid = impl<EnclosingClass>::value> \
        class types : public std::false_type { }; \
        template <typename EnclosingClass, typename FallbackType> \
        class types<EnclosingClass, FallbackType, true> : public std::true_type { \
            struct derived_fallback_ : public EnclosingClass, FallbackType { }; \
            struct derived_ : public EnclosingClass { }; \
            struct empty_type_ {}; \
        public: \
        	using undefined_type = typename impl<EnclosingClass>::null_; \
        	using type = typename impl<EnclosingClass>::type; \
            using enclosing_type = EnclosingClass; \
            using fallback_type = FallbackType; \
            using selected_type = typename std::conditional<value, enclosing_type, fallback_type>::type; \
            using derived_type = typename std::conditional<value, derived_, derived_fallback_>::type; \
            using extension_type = typename std::conditional<value, empty_type_, fallback_type>::type; \
        }; \
        template <typename EnclosingClass> \
        class types<EnclosingClass, void, true> : public std::true_type { \
            struct derived_ : public EnclosingClass { }; \
            struct empty_type_ {}; \
        public: \
        	using undefined_type = typename impl<EnclosingClass>::null_; \
        	using type = typename impl<EnclosingClass>::type; \
        	template <typename... Args> using return_type = typename impl<EnclosingClass>::template return_type<Args...>; \
            using enclosing_type = EnclosingClass; \
            using fallback_type = void; \
            using selected_type = typename std::conditional<value, enclosing_type, void>::type; \
            using derived_type = derived_; \
            using extension_type = empty_type_; \
        }; \
    }; \
    template <typename EnclosingClass, typename FallbackType> class detector_name : public detector_name ## _detail_::template types<EnclosingClass, FallbackType> {}


#endif // adl_BUILDING_DOCS

#endif // adl__type_helper__hpp__
