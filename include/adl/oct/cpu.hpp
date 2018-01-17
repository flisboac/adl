// $flisboac 2017-08-26
/**
 * @file cpu.hpp
 */
#ifndef adl__oct__cpu__hpp__
#define adl__oct__cpu__hpp__

#include "adl/oct/cpu/oper_base_.hpp"
#include "adl/oct/cpu/seq_context.hpp"
#include "adl/oct/cpu/async_context.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"

// Closure
#include "adl/oct/cpu/close_oper.hpp"
#include "adl/oct/cpu/strong_close_oper.hpp"
#include "adl/oct/cpu/strengthen_oper.hpp"
#include "adl/oct/cpu/tighten_oper.hpp"
#include "adl/oct/cpu/closure_oper.hpp"

// Validity checking
#include "adl/oct/cpu/is_coherent_oper.hpp"
#include "adl/oct/cpu/is_consistent_oper.hpp"
#include "adl/oct/cpu/is_int_consistent_oper.hpp"

// System construction
#include "adl/oct/cpu/add_cons_close_oper.hpp"


#endif //adl__oct__cpu__hpp__
