// $flavio.lisboa @ 2018-02-27.
//
/*
 * @file base.hpp
 */

#ifndef adl__cm__base__hpp__
#define adl__cm__base__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/cm.fwd.hpp"

constexpr ::adl::cm::mem_feature operator&(::adl::cm::mem_feature a, ::adl::cm::mem_feature b) noexcept { using T = ::adl::cm::mem_feature; return static_cast<T>(std::underlying_type_t<T>(a) & std::underlying_type_t<T>(b)); }
constexpr ::adl::cm::mem_feature operator|(::adl::cm::mem_feature a, ::adl::cm::mem_feature b) noexcept { using T = ::adl::cm::mem_feature; return static_cast<T>(std::underlying_type_t<T>(a) | std::underlying_type_t<T>(b)); }

constexpr ::adl::cm::scheduling operator&(::adl::cm::scheduling a, ::adl::cm::scheduling b) noexcept { using T = ::adl::cm::scheduling; return static_cast<T>(std::underlying_type_t<T>(a) & std::underlying_type_t<T>(b)); }
constexpr ::adl::cm::scheduling operator|(::adl::cm::scheduling a, ::adl::cm::scheduling b) noexcept { using T = ::adl::cm::scheduling; return static_cast<T>(std::underlying_type_t<T>(a) | std::underlying_type_t<T>(b)); }

adl_BEGIN_ROOT_MODULE

namespace cm {

namespace detail_ {

template <typename T> constexpr T extract_feature(T feature_mask, T features_value) noexcept(T()) {
    return features_value & feature_mask;
}
template <typename T> constexpr bool has_feature(T feature_value, T features_value) noexcept(T()) {
    constexpr auto value = extract_feature(feature_value, features_value);
    return bool(value) && value == feature_value;
}

} // namespace detail

constexpr mem_access access_of(mem_feature features, mem_feature field) noexcept {
    auto value = features & field;
    switch (field) {
        case mem_feature::host_access: return static_cast<mem_access>(std::underlying_type_t<mem_feature>(value) >> 4u);
        case mem_feature::access: return static_cast<mem_access>(std::underlying_type_t<mem_feature>(value) >> 4u);
        default: return mem_access::unspecified;
    }
}

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__base__hpp__
