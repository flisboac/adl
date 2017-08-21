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
    is_top,
    is_bottom,

    // comparison
    equals,             // ==
    not_equals,         // !=
    is_proper_subset,   // <
    is_subset,          // <=
    is_superset,        // >=
    is_proper_superset, // >

    // set operations
    set_union,          // |
    set_intersection,   // &

    // System construction
    to_top,
    to_bottom,
    copy,
    forget_oct_var,
    add_oct_cons,
    add_octdiff_cons,
    add_oct_test,
    add_oct_assignment, // assign(var, assigned_var, constant)
    add_oct_backward_assignment,

    // Extrapolation
    widen,
    narrow
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__oper__hpp__
