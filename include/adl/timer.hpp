// $flisboac 2017-08-13
/**
 * @file timer.hpp
 */
#ifndef adl__timer__hpp__
#define adl__timer__hpp__

#include <cstdint>
#include <chrono>
#include <set>
#include <array>

#include "adl.cfg.hpp"
#include "timer.fwd.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace timer_functions {

    struct less_by_run_index {
        template <typename RunType> bool operator()(RunType const& lhs, RunType const& rhs) const noexcept;
    };

    struct less_by_run_duration {
        template <typename RunType> bool operator()(RunType const& lhs, RunType const& rhs) const noexcept;
    };

    struct less_by_run_start {
        template <typename RunType> bool operator()(RunType const& lhs, RunType const& rhs) const noexcept;
    };

    struct less_by_run_stop {
        template <typename RunType> bool operator()(RunType const& lhs, RunType const& rhs) const noexcept;
    };
};

// TODO How to make it thread-safe-ish without causing too much overhead? Atomics/Memory fences? P-O-Mutexes?
template <typename TimerType>
class average_timer_base_ {
private:
    using subclass_ = TimerType;
    using clock_type_ = default_timer_clock_type_;

public:
    using time_point_type = typename clock_type_::time_point;
    using duration_type = typename clock_type_::duration;

    class run_info {
    public:
        run_info() = default;
        run_info(run_info const&) = default;
        run_info(run_info &&) noexcept = default;
        run_info& operator=(run_info const&) = default;
        run_info& operator=(run_info &&) noexcept = default;

        run_info(subclass_& timer, std::intmax_t run_index, time_point_type start_time);

        subclass_& timer();
        subclass_ const& timer() const;
        std::intmax_t run_index() const noexcept;
        duration_type duration() const noexcept;
        time_point_type start_time() const noexcept;
        time_point_type stop_time() const noexcept;
        bool valid() const noexcept;
        bool equals(run_info const& rhs) const noexcept;
        bool operator==(run_info const& rhs) const noexcept;
        bool operator!=(run_info const& rhs) const noexcept;

        void stop_time(time_point_type stop_time);

    private:
        subclass_* timer_ = nullptr;
        std::intmax_t run_index_ = 0;
        time_point_type start_time_ = time_point_type();
        time_point_type stop_time_ = time_point_type();
    };

    class run_handle {
    public:
        run_handle() = delete;
        run_handle(run_handle const&) = delete;
        run_handle(run_handle &&) noexcept = default;
        run_handle& operator=(run_handle const&) = delete;
        run_handle& operator=(run_handle &&) noexcept = default;

    private:
        friend class average_timer_base_;
        run_handle(subclass_& timer, std::intmax_t run_index);
        template <typename Callable, typename... Args>
            run_handle(subclass_& timer, std::intmax_t run_index, Callable callable, Args... args);

    public:
        ~run_handle();

        run_info const& info() const noexcept;
        subclass_ const& timer() const;
        std::intmax_t run_index() const noexcept;
        duration_type duration() const noexcept;
        time_point_type start_time() const noexcept;
        time_point_type stop_time() const noexcept;
        bool valid() const noexcept;
        bool equals(run_handle const& rhs) const noexcept;
        bool operator==(run_handle const& rhs) const noexcept;
        bool operator!=(run_handle const& rhs) const noexcept;

        void stop();

    private:
        run_info info_;
    };

    average_timer_base_() = default;
    average_timer_base_(average_timer_base_ const&) = default;
    average_timer_base_(average_timer_base_ &&) noexcept = default;
    average_timer_base_& operator=(average_timer_base_ const&) = default;
    average_timer_base_& operator=(average_timer_base_ &&) noexcept = default;

    duration_type average() const;
    duration_type duration() const; // alias to average()
    std::intmax_t run_count() const noexcept;

    run_handle run();
    template <typename Callable, typename... Args>
        run_handle run(Callable callable, Args... args);

private:
    void add_run_(run_info info);
    subclass_& as_subclass_() noexcept;
    subclass_ const& as_subclass_() const noexcept;

private:
    std::intmax_t run_count_ = 0;
    duration_type average_ = duration_type();
};


class adl_CLASS average_timer : public average_timer_base_<average_timer> {
private:
    using superclass_ = average_timer_base_<average_timer>;
    friend superclass_;
    using typename superclass_::run_info;
    void on_finished_run_(run_info hnd);
};


template <typename LessFunction>
class average_stats_timer : public average_timer_base_<average_stats_timer<LessFunction>> {
private:
    using superclass_ = average_timer_base_<average_stats_timer>;
    friend superclass_;

public:
    using typename superclass_::run_handle;
    using typename superclass_::run_info;

    average_stats_timer() = default;
    average_stats_timer(average_stats_timer const&) = default;
    average_stats_timer(average_stats_timer &&) noexcept = default;
    average_stats_timer& operator=(average_stats_timer const&) = default;
    average_stats_timer& operator=(average_stats_timer &&) noexcept = default;

    explicit average_stats_timer(LessFunction less_function);

    run_info const& best_run() const;
    run_info const& worst_run() const;

private:
    void on_finished_run_(run_info hnd);

private:
    LessFunction less_function_ = LessFunction();
    run_info best_run_;
    run_info worst_run_;
};

template <std::size_t MeasurementsCount>
class static_mark_timer {
private:
    using clock_type_ = default_timer_clock_type_;

public:
    using time_point_type = typename clock_type_::time_point;
    using duration_type = typename clock_type_::duration;

public:
    static_mark_timer() = default;
    static_mark_timer(static_mark_timer const&) = default;
    static_mark_timer(static_mark_timer &&) noexcept = default;
    static_mark_timer& operator=(static_mark_timer const&) = default;
    static_mark_timer& operator=(static_mark_timer &&) noexcept = default;

    void mark();
    std::size_t max_size() const noexcept;
    std::size_t size() const noexcept;

    time_point_type measurement(std::size_t measurement_index) const;
    duration_type total_time() const;
    duration_type time(std::size_t initial_index) const;
    duration_type time(std::size_t initial_index, std::size_t final_index) const;

private:
    std::array<time_point_type, MeasurementsCount> measurements_;
    std::size_t size_ = 0;
};


adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

//
// timer_functions
//
namespace timer_functions {
    template <typename RunType>
    inline bool less_by_run_index::operator()(RunType const& lhs, RunType const& rhs) const noexcept {
        return &lhs.timer() < &rhs.timer() || (&rhs.timer() == &lhs.timer() && lhs.run_index() < rhs.run_index());
    }

    template <typename RunType>
    inline bool less_by_run_duration::operator()(RunType const& lhs, RunType const& rhs) const noexcept {
        return &lhs.timer() < &rhs.timer() || (&rhs.timer() == &lhs.timer() && lhs.duration() < rhs.duration());
    }

    template <typename RunType>
    inline bool less_by_run_start::operator()(RunType const& lhs, RunType const& rhs) const noexcept {
        return &lhs.timer() < &rhs.timer() || (&rhs.timer() == &lhs.timer() && lhs.start_time() < rhs.start_time());
    }

    template <typename RunType>
    inline bool less_by_run_stop::operator()(RunType const& lhs, RunType const& rhs) const noexcept {
        return &lhs.timer() < &rhs.timer() || (&rhs.timer() == &lhs.timer() && rhs.stop_time() < lhs.stop_time());
    }
}

//
// average_timer_base_::run_info
//
template <typename TimerType>
inline average_timer_base_<TimerType>::run_info::run_info(
    subclass_& timer,
    std::intmax_t run_index,
    time_point_type start_time
) : timer_(&timer),
    run_index_(run_index),
    start_time_(start_time) {}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::subclass_&
average_timer_base_<TimerType>::run_info::timer() {
    if (!timer_) throw std::runtime_error("Dangling timer pointer, instance not properly initialized!");
    return *timer_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::subclass_ const&
average_timer_base_<TimerType>::run_info::timer() const {
    if (!timer_) throw std::runtime_error("Dangling timer pointer, instance not properly initialized!");
    return *timer_;
}

template <typename TimerType>
inline std::intmax_t average_timer_base_<TimerType>::run_info::run_index() const noexcept {
    return run_index_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::duration_type
average_timer_base_<TimerType>::run_info::duration() const noexcept {
    return stop_time_ - start_time_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::time_point_type
average_timer_base_<TimerType>::run_info::start_time() const noexcept {
    return start_time_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::time_point_type
average_timer_base_<TimerType>::run_info::stop_time() const noexcept {
    return stop_time_;
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_info::valid() const noexcept {
    return timer_ != nullptr && (          // Must always have a parent timer
        start_time_ <= time_point_type()   // Hasn't started yet.
        || stop_time_ <= time_point_type() // Has started but not stopped
        || stop_time_ >= start_time_);     // started and stopped
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_info::equals(run_info const& rhs) const noexcept {
    return (timer_ == rhs.timer_ && run_index_ == rhs.run_index_) || (timer_ == nullptr && rhs.timer_ == nullptr);
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_info::operator==(run_info const& rhs) const noexcept {
    return equals(rhs);
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_info::operator!=(run_info const& rhs) const noexcept {
    return !equals(rhs);
}

template <typename TimerType>
inline void average_timer_base_<TimerType>::run_info::stop_time(time_point_type stop_time) {
    stop_time_ = stop_time;
}

//
// average_timer_base_::run_handle
//

template <typename TimerType>
inline average_timer_base_<TimerType>::run_handle::run_handle(subclass_& timer, std::intmax_t run_index)
    : info_(timer, run_index, clock_type_::now()) {}

template <typename TimerType>
template <typename Callable, typename... Args>
inline average_timer_base_<TimerType>::run_handle::run_handle(
    subclass_& timer,
    std::intmax_t run_index,
    Callable callable, Args... args
) : info_(timer, run_index, clock_type_::now()) {
    callable(args...);
    stop();
};

template <typename TimerType>
inline average_timer_base_<TimerType>::run_handle::~run_handle() {
    stop();
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::run_info const&
average_timer_base_<TimerType>::run_handle::info() const noexcept {
    return info_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::subclass_ const&
average_timer_base_<TimerType>::run_handle::timer() const {
    return info_.timer();
}

template <typename TimerType>
inline std::intmax_t average_timer_base_<TimerType>::run_handle::run_index() const noexcept {
    return info_.run_index();
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::duration_type
average_timer_base_<TimerType>::run_handle::duration() const noexcept {
    return info_.duration();
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::time_point_type
average_timer_base_<TimerType>::run_handle::start_time() const noexcept {
    return info_.start_time();
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::time_point_type
average_timer_base_<TimerType>::run_handle::stop_time() const noexcept {
    return info_.stop_time();
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_handle::valid() const noexcept {
    return info_.valid();
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_handle::equals(run_handle const& rhs) const noexcept {
    return info_.equals(rhs.info_);
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_handle::operator==(run_handle const& rhs) const noexcept {
    return equals(rhs);
}

template <typename TimerType>
inline bool average_timer_base_<TimerType>::run_handle::operator!=(run_handle const& rhs) const noexcept {
    return !equals(rhs);
}

template <typename TimerType>
inline void average_timer_base_<TimerType>::run_handle::stop() {
    if (stop_time() == time_point_type()) {
        info_.stop_time(clock_type_::now());
        static_cast<average_timer_base_&>(info_.timer()).add_run_(info_);
    }
}


//
// average_timer_base_
//
template <typename TimerType>
inline typename average_timer_base_<TimerType>::duration_type average_timer_base_<TimerType>::average() const {
    return average_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::duration_type average_timer_base_<TimerType>::duration() const {
    return average();
}

template <typename TimerType>
inline std::intmax_t average_timer_base_<TimerType>::run_count() const noexcept {
    return run_count_;
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::run_handle average_timer_base_<TimerType>::run() {
    return run_handle(as_subclass_(), run_count_++);
}

template <typename TimerType>
template <typename Callable, typename... Args>
inline typename average_timer_base_<TimerType>::run_handle
average_timer_base_<TimerType>::run(Callable callable, Args... args) {
    return run_handle(as_subclass_(), run_count_++, callable, args...);
}

template <typename TimerType>
inline void average_timer_base_<TimerType>::add_run_(run_info info) {
    average_ = ((run_count_ - 1) * average_ + info.duration()) / run_count_;
    return (void) as_subclass_().on_finished_run_(info);
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::subclass_&
average_timer_base_<TimerType>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename TimerType>
inline typename average_timer_base_<TimerType>::subclass_ const&
average_timer_base_<TimerType>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

//
// average_timer
//

inline void average_timer::on_finished_run_(run_info hnd) {
    // NOP
}

//
// average_stats_timer
//
template <typename LessFunction>
inline average_stats_timer<LessFunction>::average_stats_timer(LessFunction less_function) :
    best_run_(),
    worst_run_(),
    less_function_(less_function) {}

template <typename LessFunction>
inline typename average_stats_timer<LessFunction>::run_info const&
average_stats_timer<LessFunction>::best_run() const {
    return best_run_;
}

template <typename LessFunction>
inline typename average_stats_timer<LessFunction>::run_info const&
average_stats_timer<LessFunction>::worst_run() const {
    return worst_run_;
}

template <typename LessFunction>
inline void average_stats_timer<LessFunction>::on_finished_run_(run_info hnd) {
    if (this->run_count() > 1) {
        if (less_function_(best_run_, hnd)) best_run_ = hnd;
        if (less_function_(hnd, best_run_)) worst_run_ = hnd;
    } else {
        best_run_ = hnd;
        worst_run_ = hnd;
    }
}

//
// static_mark_timer
//

template <std::size_t MeasurementsCount>
inline void static_mark_timer<MeasurementsCount>::mark() {
    if (size_ >= MeasurementsCount) throw std::logic_error("Measurement overflow");
    measurements_[size_++] = clock_type_::now();
}

template <std::size_t MeasurementsCount>
inline std::size_t static_mark_timer<MeasurementsCount>::max_size() const noexcept {
    return MeasurementsCount;
}

template <std::size_t MeasurementsCount>
inline std::size_t static_mark_timer<MeasurementsCount>::size() const noexcept {
    return size_;
}

template <std::size_t MeasurementsCount>
inline typename static_mark_timer<MeasurementsCount>::time_point_type
static_mark_timer<MeasurementsCount>::measurement(std::size_t measurement_index) const {
    return measurements_.at(measurement_index);
}

template <std::size_t MeasurementsCount>
inline typename static_mark_timer<MeasurementsCount>::duration_type
static_mark_timer<MeasurementsCount>::total_time() const {
    return time(0, size_ - 1);
}

template <std::size_t MeasurementsCount>
inline typename static_mark_timer<MeasurementsCount>::duration_type
static_mark_timer<MeasurementsCount>::time(std::size_t initial_index) const {
    return time(initial_index, initial_index + 1);
}

template <std::size_t MeasurementsCount>
inline typename static_mark_timer<MeasurementsCount>::duration_type
static_mark_timer<MeasurementsCount>::time(std::size_t initial_index, std::size_t final_index) const {
    return measurement(final_index) - measurement(initial_index);
}

adl_END_ROOT_MODULE

#endif //adl__timer__hpp__
