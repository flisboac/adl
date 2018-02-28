// $flisboac 2018-02-27
/**
 * @file allocator.hpp
 */
#ifndef adl__cm__allocator__hpp__
#define adl__cm__allocator__hpp__

#include <memory>

#include "adl.cfg.hpp"
#include "adl/cm.fwd.hpp"
#include "adl/cm/base.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

namespace allocator_detail_ {

template <typename T> constexpr T extract_feature(T feature_mask, T features_value) noexcept(T()) {
    return features_value & feature_mask;
}
template <typename T> constexpr bool has_feature(T feature_value, T features_value) noexcept(T()) {
    constexpr auto value = extract_feature(feature_value, features_value);
    return bool(value) && value == feature_value;
}

}

template <typename ConstantType, typename BackendType>
class allocator : public std::allocator<ConstantType> {
    using superclass_ = std::allocator<ConstantType>;
public:
    using superclass_::allocator;

    allocator() = default;
    allocator(allocator const&) noexcept = default;
    template <typename U> explicit allocator(allocator<U> const& other) noexcept : superclass_(other) {}

    explicit allocator(BackendType&) {}
};

template <typename AllocatorType>
class allocator_traits : public std::allocator_traits<AllocatorType> {
    static_assert("Invalid allocator type.");
};

template <typename ConstantType, typename BackendType>
class allocator_traits<allocator<ConstantType, BackendType>> : public std::allocator_traits<std::allocator<ConstantType>> {
    using constant_type = ConstantType;
    using backend_type = BackendType;

    template <typename T = mem_feature> constexpr static T const features = T();
    template <typename T> constexpr static T feature(T feature_mask) noexcept(T()) { return allocator_detail_::extract_feature(feature_mask, features<T>); }
    template <typename T> constexpr static bool has_feature(T feature_value) noexcept(T()) { return allocator_detail_::has_feature(feature_value, features<T>); }
};

} // namespace cm

adl_END_ROOT_MODULE

#endif //adl__cm__allocator__hpp__
