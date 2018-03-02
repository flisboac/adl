// $flisboac 2018-02-27
/**
 * @file allocator.hpp
 */
#ifndef adl__cm__allocator__hpp__
#define adl__cm__allocator__hpp__

#include <memory>

#include "adl.cfg.hpp"
#include "adl/stti/base.hpp"
#include "adl/cm.fwd.hpp"
#include "adl/cm/base.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

namespace detail {

template <typename SubType, typename ConstantType, typename BackendType, typename RealAllocatorType = SubType>
class allocator_traits_template : public valid_type, public std::allocator_traits<RealAllocatorType> {
public:
    using constant_type = ConstantType;
    using backend_type = BackendType;

    constexpr static mem_feature feature(mem_feature feature_mask) noexcept { return detail_::extract_feature(feature_mask, SubType::features); }
    constexpr static mem_access access_of(mem_feature field) noexcept { return cm::access_of(SubType::features, field); }
    constexpr static bool has_feature(mem_feature feature_value) noexcept { return detail_::has_feature(feature_value, SubType::features); }
};

} // namespace detail

template <typename ConstantType, typename BackendType>
class allocator : public std::allocator<ConstantType> {
    using superclass_ = std::allocator<ConstantType>;
public:
    using superclass_::allocator;

    allocator(allocator const&) noexcept = default;
    template <typename U> explicit allocator(allocator<U, BackendType> const& other) noexcept : superclass_(other) {}

    explicit allocator(BackendType&) {}
};

template <typename AllocatorType>
class allocator_traits : public std::allocator_traits<AllocatorType>, public invalid_type {};

} // namespace cm

adl_END_ROOT_MODULE

#endif //adl__cm__allocator__hpp__
