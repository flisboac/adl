// $flavio.lisboa @ 2018-02-26.
//
/*
 * @file device.hpp
 */


#ifndef adl__cm__device__hpp__
#define adl__cm__device__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/stti/base.hpp"
#include "adl/cm.fwd.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

namespace detail {

template <typename SubClass, typename BackendType>
class device_traits_template : public valid_type {
public:
    using backend_type = BackendType;
    using device_type = cm::basic_device<backend_type>;
    template <template <typename> class SchedulerClass, typename... Args>
    using make_scheduler_type = std::enable_if_t<
            std::is_constructible<SchedulerClass<backend_type>, device_type, Args...>::value,
            SchedulerClass<backend_type>
        >;
    template <template <typename> class SchedulerClass, typename... Args>
    using safe_make_scheduler_type = std::enable_if_t<
            std::is_constructible<SchedulerClass<backend_type>, std::error_code&, device_type, Args...>::value,
            SchedulerClass<backend_type>
    >;
};

template <typename SubClass, typename BackendType>
class device_template : public std::enable_shared_from_this<SubClass> {
private:
    using subclass_ = SubClass;

    subclass_ const& as_subclass_() const noexcept { return static_cast<subclass_ const&>(*this); }
    subclass_ & as_subclass_() noexcept { return static_cast<subclass_&>(*this); }

public:
    using backend_type = BackendType;

    backend_type backend() const noexcept { return this->as_subclass_().backend_; }

    template <template <typename> class SchedulerClass,
        typename... Args>
    typename device_traits<subclass_>::template make_scheduler_type<SchedulerClass, Args...>
    make_scheduler(Args... args) noexcept(SchedulerClass<backend_type>(std::declval<Args>()...)) {
        return SchedulerClass<backend_type>(args...);
    };

    template <template <typename> class SchedulerClass,
            typename... Args>
    typename device_traits<subclass_>::template safe_make_scheduler_type<SchedulerClass, Args...>
    safe_make_scheduler(std::error_code& err, Args... args) noexcept(SchedulerClass<backend_type>(std::declval<std::error_code>(), std::declval<Args>()...)) {
        return SchedulerClass<backend_type>(err, args...);
    };
};

} // namespace detail

template <typename BackendType>
class device_traits : public invalid_type {};

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__device__hpp__
