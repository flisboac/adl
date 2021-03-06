// $flisboac 2017-08-13
#include "adl_catch.hpp"
#include "adl/timer.hpp"

#include <random>
#include <chrono>

static int random_num() {
    std::random_device rd;
    std::mt19937 g{ rd() };
    std::uniform_int_distribution<int> gen{ 0, 65575 };
    return gen(g);
}

template <typename TimerHandleType>
static void payload(std::size_t passes_count, TimerHandleType && timer_handle) {
    for (std::size_t idx = 0; idx < passes_count; ++idx) {
        int i = random_num() % 100;
        i = (i * 8) + 1337;
        i = (i / 2) - 3;
        i = (i * 8) + 32417;
        i = (i / 2) - 5;
        i = (i * 8) + 74348029;
        i = (i / 2) - 7;
    }
}

template <typename DurationType>
static double ms(DurationType duration) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

static void execute() {
    constexpr const int global_passes = 1000;
    constexpr const int function_passes = 100000;

    adl::average_timer handler_timer;
    for (int i = 0; i < global_passes; ++i) {
        payload(function_passes, handler_timer.run());
    }
    WARN("Moving handler - Passes: " << handler_timer.run_count() << ", average time: " << ms(handler_timer.average()));

    adl::average_timer exec_timer;
    for (int i = 0; i < global_passes; ++i) {
        exec_timer.run([=] { payload(function_passes, 0); });
    }
    WARN("Lambda - Passes: " << exec_timer.run_count() << ", average time: " << ms(exec_timer.average()));

    adl::average_timer local_timer;
    for (int i = 0; i < global_passes; ++i) {
        auto local_timer_handle = local_timer.run();
        payload(function_passes, 0);
    }
    WARN("Local handle - Passes: " << local_timer.run_count() << ", average time: " << ms(local_timer.average()));

    adl::average_timer all_timer;
    auto all_timer_handle = all_timer.run();
    for (int i = 0; i < global_passes; ++i) {
        payload(function_passes, 0);
    }
    all_timer_handle.stop();
    WARN("Outside - average time: " << ms(all_timer_handle.duration() / global_passes));

    using namespace adl;
    adl::static_mark_timer<4> mark_timer;
    for (std::size_t i = mark_timer.size(); i < mark_timer.max_size() - 1; ++i) {
        mark_timer.mark();
        for (int i = 0; i < global_passes; ++i) {
            payload(function_passes, 0);
        }
    }
    mark_timer.mark();
    WARN("Mark - timings: " << ms(mark_timer.time(0)) << ", " << ms(mark_timer.time(1)) << ", " << ms(mark_timer.time(2)));
}


TEST_CASE("unit:adl/timer", "[adl][unit]") {
    //execute();
}
