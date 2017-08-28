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
    waiting,   // e.g. CL_QUEUED, std::launch::lazy's lazy evaluation
    launching, // Preparing for execution
    started,   // Executing
    finished   // Finished execution
};

enum class oper_timing {
    total, // Total time, from start to finish.
    queue, // The total time the operation waited to be launched, if any.
    launch, // Time it took for the operation to be launched (e.g. sending to device, creating helper objects)
    execution // Time it took for the operation to be executed. May include destruction/deallocation time.
};

enum class oper_kind {
    custom = -1,
    invalid = 0,

    // Closure
    shortest_path,
    strengthen,
    tighten,
    closure, // Implements an is_not_empty verification. Accounts for StrongClosure and TightClosure, from Aziem's paper, depending on the value's type

    // Validity checking
    is_coherent,
    is_consistent, // Accounts for both the int and real operators, depending on the value's type
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
