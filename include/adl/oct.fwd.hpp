// $flisboac 2017-02-28
/**
 * @file oct.fwd.hpp
 */
#ifndef adl__oct__fwd__hpp__
#define adl__oct__fwd__hpp__


#include <limits>

#include "adl.cfg.hpp"


adl_BEGIN_MAIN_MODULE(oct)

//
// domain_space.hpp
//

enum class domain_space;

//
// limits.hpp
//

using default_var_id_type = int;
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
template <typename ValueType, typename ValueLimits = value_limits<ValueType>> struct value_traits;
template <domain_space Domain, typename VarIdLimits = var_id_limits<Domain>> struct var_id_traits;
template <typename VarType> struct var_traits;

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


//
// vexpr.hpp
//
enum class vexpr_oper;
template <template <typename VarType> class VexprType, typename VarType> class vexpr_base_;
template <typename VarType> class oct_vexpr;
template <typename VarType> class octdiff_vexpr;

adl_END_MAIN_MODULE

#endif // adl__oct__fwd__hpp__
