// $flisboac 2017-08-20
/**
 * @file oper.hpp
 */
#ifndef adl__oct__oper__hpp__
#define adl__oct__oper__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"


adl_BEGIN_ROOT_MODULE
namespace oct {

enum class oper_state {
    created,   // Waiting to be queued or executed. Does not count to total timing because this represents a form of delayed execution, like std::launch::lazy or some delayed call to CL's enqueue
    queued,    // Command was called and queued. Waiting for definite execution (e.g. on a queue waiting for a thread to pick it up, CL_PROFILING_COMMAND_QUEUED)
    prepared,  // Prepared for execution (e.g. CL_PROFILING_COMMAND_SUBMIT)
    started,   // Operation has already started (and is currently in) execution.
    finished   // Operation has finished its execution.
};

enum class oper_timing {
    total,    // Total execution time. Sum of queue, launch and execution.
    wait,     // Time elapsed in between oper_state::created and some starting state (oper_state::queued, oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    queue,    // Time elapsed in between oper_state::queued and the first (actual) operational state (oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    launch,   // Time elapsed in between oper_state::prepared and oper_state::started, if any. Must be zero if not applicable.
    execution // Time elapsed in between oper_state::started and oper_state::finished (including "teardown" time, if any).
};

enum class oper_kind {
    custom = -1,
    invalid = 0,

    // Closure
    close,
    strengthen,
    tighten,
    closure, // Implements an is_not_empty verification. Accounts for StrongClosure and TightClosure, from Aziem's paper, depending on the value's type

    // Validity checking
    is_coherent,
    is_int_consistent,
    is_consistent,
    is_closed, // Implies a stronger form of consistency (center diagonal's elements = 0)
    is_strongly_closed, // Accounts for strong closure (for reals) and tight closure (for integers). Not applicable after tightening!
    is_tightly_closed, // Applicable only for the result of tighten
    is_weakly_closed, // Applicable only for the result of tighten

    // comparison
    equals,             // ==
    not_equals,         // !=
    is_proper_subset,   // <
    is_subset,          // <=
    is_superset,        // >=
    is_proper_superset, // >
    is_top,
    is_bottom,

    // set operations
    set_union,          // |
    set_intersection,   // &

    // Basic system construction
    to_top,
    to_bottom,
    copy,
    forget_oct_var,
    add_oct_cons,
    add_octdiff_cons,
    add_dbm,

    // Abstraction-based system construction
    add_oct_test,
    add_oct_assignment, // assign(var, assigned_var, constant)
    add_oct_backward_assignment,

    // Extrapolation
    widen,
    narrow
};

template <typename DbmType, typename ContextType>
class null_oper {};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__oper__hpp__
