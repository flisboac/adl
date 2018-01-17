// $flisboac 2017-04-02
/**
 * @file limits.hpp
 */
#ifndef adl__oct__limits__hpp__
#define adl__oct__limits__hpp__

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"


//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)

/**
 * An increment of `std::numeric_limits` to define the values and functions needed for matrix calculations.
 * @tparam ConstantType The value's type.
 */
template <typename ConstantType>
struct constant_limits {
    using value_type = ConstantType;
    using numeric_limits = std::numeric_limits<ConstantType>;

    static_assert(numeric_limits::is_specialized,
          "The constant_limits class is not valid. Please, specialize it correctly "
          "for the provided value type.");

    constexpr static const bool valid = numeric_limits::is_specialized;
	constexpr static const bool integer = numeric_limits::is_integer;
	constexpr static const bool native_integer = numeric_limits::is_integer;

	constexpr static value_type top() noexcept;
    constexpr static value_type bottom() noexcept;
    constexpr static bool is_null(value_type value) noexcept;
    constexpr static bool is_top(value_type c) noexcept;
	constexpr static ConstantType const& min(ConstantType const& a, ConstantType const& b);
	constexpr static ConstantType min(std::initializer_list<ConstantType> list);
	template <typename ConstantType_, typename = std::enable_if_t<std::numeric_limits<ConstantType_>::is_integer>>
        constexpr static ConstantType_ const& floor_div(ConstantType_ const& b) { return b; }
	template <typename ConstantType_, typename = std::enable_if_t<!std::numeric_limits<ConstantType_>::is_integer>>
        constexpr static ConstantType floor(ConstantType_ b) { return std::floor(b); }
    static std::string to_string(value_type value);
};

/**
 * A `std::numeric_limits`-like traits class defining the various variable-ID limits.
 * All variable ID fields have **normalized values** -- variable IDs are in their **positive**
 * form.
 * @tparam Domain The domain to which the limits apply.
 * @tparam VarIdType The variable's underlying ID type.
 */
template <domain_space Domain>
struct var_id_limits {
    // Types
   	using var_id_type = default_var_id_type;

   	// Constexpr static values
    constexpr static const bool valid = false;
   	constexpr static const domain_space space = Domain;
};

/**
 * Specialization of `adl::oct::var_id_limits` for the octagonal domain space.
 * @tparam VarIdType The variable ID's type.
 * @see adl::oct::var_id_limits<>
 */
template <>
struct var_id_limits<domain_space::oct> {
    // Types
   	using var_id_type = default_var_id_type;
    using counterpart_var_id_limits = var_id_limits<domain_space::octdiff>;

   	// Constexpr static values
    constexpr static const bool valid = true;
   	constexpr static const domain_space space = domain_space::oct;
    constexpr static const domain_space counterpart_space = oct::domain_space::octdiff;
   	constexpr static const std::size_t max_variables = max_oct_variables<var_id_type>;
   	constexpr static const var_id_type invalid_var_id = var_id_type(0);
   	constexpr static const var_id_type first_var_id = var_id_type(1);
   	constexpr static const var_id_type last_var_id = var_id_type(max_variables - 1);
   	constexpr static const var_id_type begin_var_id = first_var_id;
   	constexpr static const var_id_type end_var_id = var_id_type(max_variables);
   	constexpr static const var_id_type rbegin_var_id = last_var_id;
   	constexpr static const var_id_type rend_var_id = 0;

    constexpr static const var_id_type min_var_id_value = -last_var_id;
    constexpr static const var_id_type max_var_id_value = end_var_id;

    constexpr static const char *const base_var_name_format = "x%d";
    constexpr static const char *const positive_var_name_format = "%s";
    constexpr static const char *const negative_var_name_format = "-%s";
};

/**
 * Specialization of `adl::oct::var_id_limits` for the octagonal-difference domain space.
 * @tparam VarIdType The variable ID's type.
 */
template <>
struct var_id_limits<domain_space::octdiff> {
    // Types
   	using var_id_type = default_var_id_type;
	using counterpart_var_id_limits = var_id_limits<domain_space::oct>;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const domain_space space = oct::domain_space::octdiff;
    constexpr static const domain_space counterpart_space = oct::domain_space::oct;
    constexpr static const std::size_t max_variables = max_octdiff_variables<var_id_type>;
    constexpr static const var_id_type invalid_var_id = var_id_limits<domain_space::oct>::invalid_var_id;
    constexpr static const var_id_type first_var_id = var_id_limits<domain_space::oct>::first_var_id;
    constexpr static const var_id_type last_var_id = var_id_type(max_variables - 3);
    constexpr static const var_id_type begin_var_id = first_var_id;
    constexpr static const var_id_type end_var_id = var_id_type(last_var_id + 2);
    constexpr static const var_id_type rbegin_var_id = last_var_id;
    constexpr static const var_id_type rend_var_id = var_id_limits<domain_space::oct>::rend_var_id;

    constexpr static const var_id_type min_var_id_value = 0;
    constexpr static const var_id_type max_var_id_value = end_var_id;

    constexpr static const char *const base_var_name_format = var_id_limits<domain_space::oct>::base_var_name_format;
    constexpr static const char *const positive_var_name_format = "%s__pos";
    constexpr static const char *const negative_var_name_format = "%s__neg";
};

adl_END_MAIN_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <typename Constant>
constexpr typename adl::oct::constant_limits<Constant>::value_type
adl::oct::constant_limits<Constant>::top() noexcept {
    return numeric_limits::has_infinity ? numeric_limits::infinity() : numeric_limits::max();
}

template <typename Constant>
constexpr bool
adl::oct::constant_limits<Constant>::is_top(Constant c) noexcept {
    return numeric_limits::has_infinity ? numeric_limits::infinity() <= c  : numeric_limits::max() == c;
}

template <typename Constant>
constexpr typename adl::oct::constant_limits<Constant>::value_type
adl::oct::constant_limits<Constant>::bottom() noexcept {
    return numeric_limits::has_infinity ? -numeric_limits::infinity() : numeric_limits::min();
}

template <typename Constant>
constexpr bool adl::oct::constant_limits<Constant>::is_null(value_type value) noexcept {
    return value >= top();
}

template <typename Constant>
adl_IMPL std::string constant_limits<Constant>::to_string(value_type value) {
    return std::to_string(value);
}

template <typename Constant>
constexpr Constant const& constant_limits<Constant>::min(Constant const& a, Constant const& b) {
	return std::min(a, b);
}

template <typename Constant>
constexpr Constant constant_limits<Constant>::min(std::initializer_list<Constant> list) {
	return std::min(list);
}

adl_END_MAIN_MODULE

#endif //adl__oct__limits__hpp__
