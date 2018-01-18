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

#define adl_MAKE_FUNCTION_CHECK_CLASS(ReturnType, FunctionName, ...) adl_MAKE_FUNCTION_CHECK_CLASS_CV(, ReturnType, FunctionName, __VA_ARGS__)
#define adl_MAKE_FUNCTION_CHECK_CLASS_CV(Cv, ReturnType, FunctionName, ...) \
    template <typename T> \
    class has_ ## FunctionName ## _function { \
    private: \
        template <typename U, ReturnType (U::*)(__VA_ARGS__) Cv> struct SFINAE_ {}; \
        template <typename U> constexpr static const char TestPresence_(SFINAE_<U, &U::FunctionName>*); \
        template <typename U> constexpr static const int TestPresence_(...); \
        constexpr static const bool has_function_ = sizeof(TestPresence_<T>(0)) == sizeof(char); \
    public: \
        static const bool value = has_function_; \
        using type = T; \
    }; \
    template <typename T> constexpr static const bool has_ ## FunctionName ## _function_v = has_ ## FunctionName ## _function <T>::value; \
    template <typename T> using has_ ## FunctionName ## _function_t = std::enable_if_t< \
        has_ ## FunctionName ## _function <T>::value, T>; \
    template <typename U, typename... Args> \
        static std::enable_if_t<has_ ## FunctionName ## _function_v<U>, ReturnType> \
        do_ ## FunctionName (U& oper, Args... args) { return (ReturnType)(oper.FunctionName(args...)); } \
    template <typename U, typename... Args> \
        static std::enable_if_t<!has_ ## FunctionName ## _function_v<U>, ReturnType> \
        do_ ## FunctionName(U& oper, Args...) { return (ReturnType) 0; }


namespace detail_ {

template <typename DbmType, typename ContextType, typename ResultType>
class oper_base_traits_ {
public:
    adl_MAKE_FUNCTION_CHECK_CLASS(void, on_prepare_);
    adl_MAKE_FUNCTION_CHECK_CLASS(ResultType, on_execute_);
    adl_MAKE_FUNCTION_CHECK_CLASS(void, on_finished_);
};

template <typename DbmType, typename ContextType>
class oper_base_traits_<DbmType, ContextType, void> {
public:
    adl_MAKE_FUNCTION_CHECK_CLASS(void, on_prepare_);
    adl_MAKE_FUNCTION_CHECK_CLASS(void, on_execute_);
    adl_MAKE_FUNCTION_CHECK_CLASS(void, on_finished_);
};

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
class oper_base_seq_ : public crtp_base<SubType> {
protected:
    using subtype_ = SubType;
    using traits_ = oper_base_traits_<DbmType, ContextType, ResultType>;
    using timer_type_ = static_mark_timer<5>;

public:
    using context_type = ContextType;
    using dbm_type = DbmType;
    using duration_type = typename timer_type_::duration_type;

    oper_base_seq_();
    oper_base_seq_(oper_base_seq_ const&) = default;
    oper_base_seq_(oper_base_seq_ &&) noexcept = default;
    oper_base_seq_& operator=(oper_base_seq_ const&) = default;
    oper_base_seq_& operator=(oper_base_seq_ &&) noexcept = default;

    // Properties
    oper_state state() const;
    duration_type time() const;
    duration_type time(oper_timing timing) const;

protected:
    oper_state state_;
    timer_type_ timer_;
};

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
class oper_base_ {
#if 0
    // Concept for asynchronous operators

    context_type const& context() const;
    std::shared_ptr<context_type> context_ptr() const;
    oper_state state() const;

    result_type run();
    std::future<result_type> run(std::launch launch_mode);

    duration_type time() const;
    duration_type time(oper_timing timing) const;
#endif
};

template <typename SubType, typename DbmType, typename ResultType>
class oper_base_<SubType, DbmType, seq_context, ResultType> : public detail_::oper_base_seq_<SubType, DbmType, seq_context, ResultType> {
    using superclass_ = detail_::oper_base_seq_<SubType, DbmType, seq_context, ResultType>;
public:
    //static_assert(std::is_reference<ResultType>::value, "References are not allowed, no real use-case for now");
    static_assert(std::is_move_assignable<ResultType>::value
            || std::is_copy_assignable<ResultType>::value,
        "Return type is not valid");

    using typename superclass_::context_type;
    using typename superclass_::dbm_type;
    using typename superclass_::subtype_;
    using typename superclass_::traits_;
    using result_type = ResultType;

    // CONSTRUCTORS AND ASSIGNS
    oper_base_() = default;
    oper_base_(oper_base_ const&) = default;
    oper_base_(oper_base_ &&) noexcept = default;
    oper_base_& operator=(oper_base_ const&) = default;
    oper_base_& operator=(oper_base_ &&) noexcept = default;

    // "OPERATIONS"
    result_type get() {
        switch (this->state_) {
            case oper_state::created:
                this->state_ = oper_state::queued;
                this->timer_.mark(); // 1
            case oper_state::queued:
                traits_::do_on_prepare_(static_cast<SubType&>(*this));
                this->state_ = oper_state::prepared;
                this->timer_.mark(); // 2
                // non-stop
            case oper_state::prepared: {
                this->state_ = oper_state::started;
                this->timer_.mark(); // 3
                auto ret = traits_::do_on_execute_(static_cast<SubType&>(*this));
                this->state_ = oper_state::finished;
                traits_::do_on_finished_(static_cast<SubType&>(*this));
                this->timer_.mark(); // 4
                return ret;
            }
            default:
                throw std::logic_error("Illegal operator state");
        }
    }

    subtype_& discard() {
        get();
        return static_cast<SubType&>(*this);
    }
};

template <typename SubType, typename DbmType>
class oper_base_<SubType, DbmType, seq_context, void> : public detail_::oper_base_seq_<SubType, DbmType, seq_context, void> {
private:
    using superclass_ = detail_::oper_base_seq_<SubType, DbmType, seq_context, void>;
    using timer_type_ = static_mark_timer<5>;

public:
    using typename superclass_::context_type;
    using typename superclass_::dbm_type;
    using typename superclass_::duration_type;
    using typename superclass_::subtype_;
    using typename superclass_::traits_;
    using result_type = void;

    // CONSTRUCTORS AND ASSIGNS
    oper_base_() = default;
    oper_base_(oper_base_ const&) = default;
    oper_base_(oper_base_ &&) noexcept = default;
    oper_base_& operator=(oper_base_ const&) = default;
    oper_base_& operator=(oper_base_ &&) noexcept = default;

    // "Operations"
    void get() {
        switch (this->state_) {
            case oper_state::created:
                this->state_ = oper_state::queued;
                this->timer_.mark(); // 1
            case oper_state::queued:
                traits_::do_on_prepare_(static_cast<SubType&>(*this));
                this->state_ = oper_state::prepared;
                this->timer_.mark(); // 2
                // non-stop
            case oper_state::prepared:
                this->state_ = oper_state::started;
                this->timer_.mark(); // 3
                traits_::do_on_execute_(static_cast<SubType&>(*this));
                this->state_ = oper_state::finished;
                traits_::do_on_finished_(static_cast<SubType&>(*this));
                this->timer_.mark(); // 4
                break;
            default:
                throw std::logic_error("Illegal operator state");
        }
    }

    subtype_& discard() {
        get();
        return static_cast<SubType&>(*this);
    };
};


} // namespace detail_
} // namespace cpu
} // namespace oct

adl_END_ROOT_MODULE


//
// [[ IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)
namespace cpu {
namespace detail_ {

//
// oper_state_ (specialized for seq_context)
//
template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
oper_base_seq_<SubType, DbmType, ContextType, ResultType>::oper_base_seq_() : state_(oper_state::created) {
    this->timer_.mark();
};

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
oper_state oper_base_seq_<SubType, DbmType, ContextType, ResultType>::state() const {
    return this->state_;
};

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
typename oper_base_seq_<SubType, DbmType, ContextType, ResultType>::duration_type
oper_base_seq_<SubType, DbmType, ContextType, ResultType>::time() const {
    return this->time(oper_timing::total);
}

template <typename SubType, typename DbmType, typename ContextType, typename ResultType>
typename oper_base_seq_<SubType, DbmType, ContextType, ResultType>::duration_type
oper_base_seq_<SubType, DbmType, ContextType, ResultType>::time(oper_timing timing) const {
    if (this->state_ != oper_state::finished) throw std::logic_error("Invalid state, please run the operator first.");
    switch (timing) {
    case oper_timing::total: return this->time(oper_timing::queue) + this->time(oper_timing::launch) + this->time(oper_timing::execution);
    case oper_timing::wait: return this->timer_.time(0, 1);
    case oper_timing::queue: return this->timer_.time(1, 2);
    case oper_timing::launch: return this->timer_.time(2, 3);
    case oper_timing::execution: return this->timer_.time(3, 4);
    }
};


} // namespace detail_
} // namespace cpu
adl_END_MAIN_MODULE


#endif //adl__oct__cpu__oper_base___hpp__
