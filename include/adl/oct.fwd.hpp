// $flisboac 2017-02-28
/**
 * @file oct.fwd.hpp
 */
#ifndef adl__oct__fwd__hpp__
#define adl__oct__fwd__hpp__

#include <type_traits>
#include <limits>

#include "adl.cfg.hpp"


adl_BEGIN_MAIN_MODULE(oct)

//
// domain_space.hpp
//

enum class domain_space {
    oct = 1,
    octdiff
};

//
// limits.hpp
//

using default_var_id_type = int;
using default_value_type = float;

template <typename ValueType> class value_limits;
template <domain_space Domain> struct var_id_limits;

/**
 * The hard-limit maximum number of octagonal variables supported for a given variable id's type.
 * The maximum value takes into consideration the maximum array size, as a support to matrix calculations later on in
 * the library; this makes it possible to transform variable ids into array indexes without overflow, and independent
 * of the variable id's type size.
 * @tparam VarIdType The variable's underlying ID type.
 */
template <typename VarIdType = default_var_id_type>
constexpr static const std::size_t max_oct_variables =
    (std::numeric_limits<VarIdType>::max() / 2) > (std::numeric_limits<std::size_t>::max() / 2)
        ? (std::numeric_limits<std::size_t>::max() / 2)
        : static_cast<std::size_t>((std::numeric_limits<VarIdType>::max() / 2));

/**
 * The hard-limit maximum number of octagonal difference variables supported for a given variable id's type.
 * By definition -- as per the literature --, this is at least twice as many as octagonal variables for the
 * same type.
 * @tparam VarIdType The variable's underlying ID type.
 */
template <typename VarIdType = default_var_id_type>
constexpr static const std::size_t max_octdiff_variables = max_oct_variables<VarIdType> * 2;

//
// traits.hpp
//
template <domain_space Domain, typename VarIdLimits = var_id_limits<Domain>> struct var_id_traits;
template <typename VarType> struct var_traits;
template <domain_space DomainSpace> struct domain_space_traits;

//
// var.hpp
//
template <typename VarType, typename VarTraits = var_traits<VarType>> class var_base_;
template <typename VarType, typename VarTraits = var_traits<VarType>> class unnamed_var_base_;
template <typename VarType, typename VarTraits = var_traits<VarType>> class lit_named_var_base_;

class oct_var;
class octdiff_var;
class oct_lvar;
class octdiff_lvar;

template <typename FirstVarType, typename SecondVarType = FirstVarType> struct common_var;
template <typename FirstVarType, typename SecondVarType = FirstVarType> using common_var_t = typename common_var<FirstVarType, SecondVarType>::type;


//
// vexpr.hpp
//
enum class vexpr_oper;
template <typename VarType> class vexpr_base_;
template <typename VarType = oct_var> class oct_vexpr;
template <typename VarType = octdiff_var> class octdiff_vexpr;

template <typename FirstVarType, typename SecondVarType = FirstVarType> struct common_vexpr;
template <typename FirstVarType, typename SecondVarType = FirstVarType> using common_vexpr_t = typename common_vexpr<FirstVarType, SecondVarType>::type;


//
// cons.hpp
//
template <typename ValueType, typename VarType> class cons_base_;
template <typename ValueType, typename VarType = oct_var> class oct_cons;
template <typename ValueType, typename VarType = octdiff_var> class octdiff_cons;
template <typename ValueType, typename VarType = octdiff_var> class octdiff_conjunction;

template <typename ValueType, typename FirstVarType, typename SecondVarType = FirstVarType> struct common_cons;
template <typename ValueType, typename FirstVarType, typename SecondVarType = FirstVarType> using common_cons_t = typename common_cons<ValueType, FirstVarType, SecondVarType>::type;

template <typename ValueType, typename FirstVarType, typename SecondVarType = FirstVarType> struct common_octdiff_conjunction;
template <typename ValueType, typename FirstVarType, typename SecondVarType = FirstVarType> using common_octdiff_conjunction_t = typename common_octdiff_conjunction<ValueType, FirstVarType, SecondVarType>::type;


//
// system.hpp
//
template <domain_space Domain> class var_set;
template <domain_space Domain, typename ValueType, typename ValueLimits = value_limits<ValueType>> class system_base_;
template <typename ValueType, typename ValueLimits = value_limits<ValueType>> class oct_system;
template <typename ValueType, typename ValueLimits = value_limits<ValueType>> class octdiff_system;


//
// buffer.hpp
//
using default_buffer_flags_type = int;
enum class buffer_barrier : default_buffer_flags_type;


//
// dbm.hpp
//
enum class dbm_major {
    row, col
};

// dbm/traits.hpp
template <typename ValueType, typename ValueLimits = value_limits<ValueType>> class dbm_types_;
template <typename DbmType> struct dbm_traits;
template <typename DbmType> using dbm_t = std::enable_if_t<dbm_traits<DbmType>::valid, DbmType>;

// dbm/base_.hpp
template <typename Subclass, typename ValueType, typename ValueLimits = value_limits<ValueType>> class dbm_base_;

// dbm/dense_dbm.hpp
template <typename Subclass, typename ValueType, typename ValueLimits = value_limits<ValueType>> class dense_dbm_base_;
template <typename ValueType, typename ValueLimits = value_limits<ValueType>, typename Allocator = std::allocator<ValueType>> class dense_dbm;

// dbm/coherent_dbm.hpp
template <typename Subclass, typename ValueType, typename ValueLimits = value_limits<ValueType>> class coherent_dbm_base_;
template <typename ValueType, typename ValueLimits = value_limits<ValueType>, typename Allocator = std::allocator<ValueType>> class coherent_dbm;


adl_END_MAIN_MODULE

#endif // adl__oct__fwd__hpp__
