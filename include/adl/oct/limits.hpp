// $flisboac 2017-04-02
/**
 * @file limits.hpp
 */
#ifndef adl__oct__limits__hpp__
#define adl__oct__limits__hpp__

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"
#include "adl/oct/domain_space.hpp"


//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)

/**
 * An increment of `std::numeric_limits` to define the values and functions needed for matrix calculations.
 * @tparam ValueType The value's type.
 */
template <typename ValueType>
struct value_limits : public std::numeric_limits<ValueType> {
    using std::numeric_limits<ValueType>::has_infinity;
    using std::numeric_limits<ValueType>::infinity;
    using std::numeric_limits<ValueType>::max;
    constexpr static ValueType top();
};

/**
 * A `std::numeric_limits`-like traits class defining the various variable-ID limits.
 * All variable ID fields have **normalized values** -- variable IDs are in their **positive**
 * form.
 * @tparam Domain The domain to which the limits apply.
 * @tparam VarIdType The variable's underlying ID type.
 */
template <domain_space Domain, typename VarIdType>
struct var_id_limits {
    // Types
   	using var_id_type = VarIdType;

   	// Constexpr static values
    constexpr static bool valid = false;
   	constexpr static const domain_space space = Domain;
};

/**
 * Specialization of `adl::oct::var_id_limits` for the octagonal domain space.
 * @tparam VarIdType The variable ID's type.
 * @see adl::oct::var_id_limits<>
 */
template <typename VarIdType>
struct var_id_limits<domain_space::oct, VarIdType> {
    // Types
   	using var_id_type = VarIdType;

   	// Constexpr static values
    constexpr static bool valid = true;
   	constexpr static const domain_space space = domain_space::oct;
    constexpr static oct::domain_space counterpart_space = oct::domain_space::octdiff;
   	constexpr static const std::size_t max_variables = max_oct_variables<VarIdType>;
   	constexpr static const var_id_type invalid_var_id = var_id_type(0);
   	constexpr static const var_id_type first_var_id = var_id_type(1);
   	constexpr static const var_id_type last_var_id = var_id_type(max_variables - 1);
   	constexpr static const var_id_type begin_var_id = first_var_id;
   	constexpr static const var_id_type end_var_id = var_id_type(max_variables);
   	constexpr static const var_id_type rbegin_var_id = last_var_id;
   	constexpr static const var_id_type rend_var_id = 0;

    constexpr static const char *const base_var_name_format = "x%d";
    constexpr static const char *const positive_var_name_format = "%s";
    constexpr static const char *const negative_var_name_format = "-%s";
};

/**
 * Specialization of `adl::oct::var_id_limits` for the octagonal-difference domain space.
 * @tparam VarIdType The variable ID's type.
 */
template <typename VarIdType>
struct var_id_limits<domain_space::octdiff, VarIdType> {
    // Types
   	using var_id_type = VarIdType;

    // Constexpr static values
    constexpr static bool valid = true;
    constexpr static oct::domain_space space = oct::domain_space::octdiff;
    constexpr static oct::domain_space counterpart_space = oct::domain_space::oct;
    constexpr static std::size_t max_variables = max_octdiff_variables<VarIdType>;
    constexpr static var_id_type invalid_var_id = var_id_limits<domain_space::oct, VarIdType>::invalid_var_id;
    constexpr static var_id_type first_var_id = var_id_limits<domain_space::oct, VarIdType>::first_var_id;
    constexpr static var_id_type last_var_id = var_id_type(max_variables - 3);
    constexpr static var_id_type begin_var_id = first_var_id;
    constexpr static var_id_type end_var_id = var_id_type(last_var_id + 2);
    constexpr static var_id_type rbegin_var_id = last_var_id;
    constexpr static var_id_type rend_var_id = var_id_limits<domain_space::oct, VarIdType>::rend_var_id;

    constexpr static const char *const base_var_name_format = var_id_limits<domain_space::oct, VarIdType>::base_var_name_format;
    constexpr static const char *const positive_var_name_format = "%s__pos";
    constexpr static const char *const negative_var_name_format = "%s__neg";
};

adl_END_MAIN_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <typename Constant>
constexpr Constant adl::oct::value_limits<Constant>::top() {
    return has_infinity ? infinity() : max();
}

adl_END_MAIN_MODULE

#endif //adl__oct__limits__hpp__
