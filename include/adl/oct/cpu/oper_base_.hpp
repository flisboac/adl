// $flisboac 2017-08-26
/**
 * @file oper_base_.hpp
 */
#ifndef adl__oct__cpu__oper_base___hpp__
#define adl__oct__cpu__oper_base___hpp__

#include <atomic>
#include <future>
#include <memory>

#include "adl.cfg.hpp"
#include "adl/timer.hpp"

#include "adl/crtp.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/dbm/traits.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {
namespace cpu {

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
class oper_base_ : public crtp_base<SubType> {
public:
    using context_type = ContextType;
    using dbm_type = DbmType;
    using result_type = ResultType;
    using timer_type = static_mark_timer<5>;
    using duration_type = typename timer_type::duration_type;

    context_type const& context() const;
    oper_state state() const;

    result_type run();

    duration_type time() const;
    duration_type time(oper_timing timing) const;

protected:
    std::shared_ptr<context_type> context_;
    oper_state state_;
};

template <typename SubType, typename DbmType, typename ResultType>
class oper_base_<SubType, async_context, DbmType, ResultType> : public crtp_base<SubType> {
public:
    using context_type = async_context;
    using dbm_type = DbmType;
    using result_type = ResultType;
    using timer_type = static_mark_timer<5>;
    using duration_type = typename timer_type::duration_type;

    context_type const& context() const;
    std::shared_ptr<context_type> context_ptr() const;
    oper_state state() const;

    result_type run();
    std::future<result_type> run(std::launch launch_mode);

    duration_type time() const;
    duration_type time(oper_timing timing) const;

protected:
    std::shared_ptr<context_type> context_;
    std::atomic<oper_state> state_;
};

} // namespace cpu
} // namespace oct

adl_END_ROOT_MODULE


#endif //adl__oct__cpu__oper_base___hpp__
