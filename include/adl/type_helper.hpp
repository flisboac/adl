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

#define adl_CREATE_HAS_TRAITS_MEMBER(name) adl_CREATE_NAMED_HAS_TRAITS_MEMBER(has_field_ ## name, name)
#define adl_DECLARE_HAS_TRAITS_MEMBER(name) adl_DECLARE_NAMED_HAS_TRAITS_MEMBER(has_field_ ## name, name)
#define adl_DEFINE_HAS_TRAITS_MEMBER(name) adl_DEFINE_NAMED_HAS_TRAITS_MEMBER(has_field_ ## name, name)

#define adl_CREATE_NAMED_HAS_TRAITS_MEMBER(detector_name, name) adl_DECLARE_NAMED_HAS_TRAITS_MEMBER(detector_name, name); adl_DEFINE_NAMED_HAS_TRAITS_MEMBER(detector_name, name)
#define adl_DECLARE_NAMED_HAS_TRAITS_MEMBER(detector_name, name) adl_COMMON_DECLARE_HAS_MEMBER(detector_name, name)
#define adl_DEFINE_NAMED_HAS_TRAITS_MEMBER(detector_name, name) \
    class detector_name ## _detail_ { \
        template <typename EnclosingClass, typename FallbackType> friend class detector_name; \
        template <typename EnclosingClass> \
        class impl { \
            using yes_ = char[2]; \
            using no_ = char[1]; \
            class null_; \
            template<typename U> static yes_ & check_(decltype(U::name)*); \
            template<typename U> static no_ & check_(U*); \
            template<typename U> static decltype(&U::name()) get_(decltype(U::name)*); \
            template<typename U> static null_ get_(U*); \
        public: \
            using type = decltype(get_<EnclosingClass>(nullptr)); \
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
