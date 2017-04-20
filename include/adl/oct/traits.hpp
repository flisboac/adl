// $flisboac 2017-04-08
/**
 * @file traits.hpp
 */
#ifndef adl__oct__traits__hpp__
#define adl__oct__traits__hpp__


#include "adl/std/string_view.hpp"
#include "adl/char_helper.hpp"

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"

#include "adl/oct/domain_space.hpp"
#include "adl/oct/limits.hpp"

//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <typename ValueType, typename ValueLimits>
struct adl_CLASS value_traits {
    // Types
    using value_limits = ValueLimits;

    // Static assertions
    static_assert(value_limits::is_specialized,
        "The value_limits class is not valid. Please, specialize it correctly "
        "for the provided value type.");
    static_assert(std::is_same<typename value_limits::value_type, ValueType>::value,
        "The ValueType provided must be the same as ValueLimits::value_type.");

    // Constexpr static values
    constexpr static bool valid = true;
};


template <domain_space Domain, typename VarIdLimits>
struct adl_CLASS var_id_traits {
    // Types
    using var_id_limits = VarIdLimits;
    using var_id_type = typename var_id_limits::var_id_type;

    // Constexpr static values
    constexpr static bool valid = true;
    constexpr static auto space = var_id_limits::space;
    constexpr static auto counterpart_space = var_id_limits::counterpart_space;

    // static utility functions
    template <typename N> constexpr static void static_assert_valid_arithmetic_type() noexcept;

    // static property functions
    constexpr static bool is_valid_id(var_id_type id) noexcept;
    constexpr static bool is_id_in_range(var_id_type id) noexcept;
    constexpr static bool is_positive_id(var_id_type id) noexcept;
    constexpr static bool is_negative_id(var_id_type id) noexcept;

    // static functions
    constexpr static var_id_type normalize_id(var_id_type id) noexcept;
    constexpr static var_id_type negate_id(var_id_type id) noexcept;
    constexpr static var_id_type increment_id(var_id_type id, size_t offset = 1) noexcept;
    constexpr static var_id_type decrement_id(var_id_type id, size_t offset = 1) noexcept;
    constexpr static bool id_equals(var_id_type id1, var_id_type id2) noexcept;
    constexpr static int id_compare(var_id_type id1, var_id_type id2) noexcept;
    constexpr static int id_sign(var_id_type id) noexcept;
    constexpr static const char* id_sign_format(var_id_type id) noexcept;

    // static conversion functions
    constexpr static var_id_type name_to_id(char const* name) noexcept;
    constexpr static var_id_type name_to_id(string_view name) noexcept;
    static var_id_type name_to_id(std::string name);
    template <typename N> constexpr static var_id_type arithmetic_to_range(N value) noexcept;
    template <typename N> constexpr static var_id_type arithmetic_to_valid(N value) noexcept;
    template <typename N = long long int> constexpr static N id_to_arithmetic(var_id_type id, N fallback_value = N()) noexcept;
    constexpr static var_id_type id_to_range(var_id_type id) noexcept; // first-end
    constexpr static var_id_type id_to_valid(var_id_type id) noexcept; // first-last
    constexpr static std::size_t id_to_index(var_id_type id) noexcept;
    constexpr static var_id_type id_to_counterpart(var_id_type id) noexcept;
    constexpr static var_id_type id_to_normal_oct(var_id_type id) noexcept;
    static void id_to_name(
        std::string& name,
        var_id_type id,
        std::string const& base_name_format = var_id_limits::base_var_name_format,
        std::string const& pos_neg_format = std::string());
    static std::string id_to_name(
        var_id_type id,
        std::string const& base_name_format = var_id_limits::base_var_name_format,
        std::string const& pos_neg_format = std::string());

private:
    constexpr static var_id_type raw_normalize_id_(var_id_type id) noexcept;
    template <typename N> constexpr static var_id_type arithmetic_to_id_limits_(N value) noexcept;
};

template <typename VarType>
struct adl_CLASS var_traits {
    constexpr static bool valid = false;
#if 0
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::oct>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type
    using var_type = VarType;
    using counterpart_var_type = VarType;
    using vexpr_type = oct_vexpr<VarType>;
    using counterpart_vexpr_type = octdiff_vexpr<VarType>;

    // Constexpr static values
    constexpr static bool valid = false;
    constexpr static auto space = var_id_traits::space;
    constexpr static auto counterpart_space = var_id_traits::counterpart_space;

    // static functions
    constexpr static var_type var_with_id(const var_type& var, var_id_type id);
    constexpr static counterpart_var_type var_to_counterpart(const var_type& var, var_id_type counterpart_id);
#endif
};

adl_END_MAIN_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

//
// var_id_traits
//

template <domain_space Domain, typename VarIdLimits>
template <typename N>
constexpr void var_id_traits<Domain, VarIdLimits>::static_assert_valid_arithmetic_type() noexcept {
    using return_limits = std::numeric_limits<N>;
    static_assert(std::is_arithmetic<N>::value,
        "Value type must be arithmetic.");
}

template <domain_space Domain, typename VarIdLimits>
constexpr bool var_id_traits<Domain, VarIdLimits>::is_valid_id(var_id_type id) noexcept {
    bool valid = space == domain_space::oct || id >= 0;
    if (valid) id = raw_normalize_id_(id);
    return valid && id >= var_id_limits::first_var_id && id <= var_id_limits::last_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr bool var_id_traits<Domain, VarIdLimits>::is_id_in_range(var_id_type id) noexcept {
    return is_valid_id(id)
        || id == var_id_limits::invalid_var_id
        || id == var_id_limits::end_var_id
        || id == var_id_limits::rend_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr bool var_id_traits<Domain, VarIdLimits>::is_positive_id(var_id_type id) noexcept {
    return var_id_limits::space == domain_space::octdiff
        ? bool(id & 1)
        : (id > 0);
}

template <domain_space Domain, typename VarIdLimits>
constexpr bool var_id_traits<Domain, VarIdLimits>::is_negative_id(var_id_type id) noexcept {
    return var_id_limits::space == domain_space::octdiff
        ? !bool(id & 1)
        : (id < 0);
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename adl::oct::var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::normalize_id(var_id_type id) noexcept {
    return is_valid_id(id)
        ? raw_normalize_id_(id)
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::negate_id(var_id_type id) noexcept {
    return is_valid_id(id)
        ? var_id_limits::space == domain_space::oct
            ? -id
            : (id & 1) ? id + 1 : id - 1
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::increment_id(
    var_id_type id,
    size_t offset
) noexcept {
    return is_valid_id(id)
        ? normalize_id(raw_normalize_id_(id) + (var_id_limits::space == domain_space::octdiff ? offset << 1 : offset))
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::decrement_id(
    var_id_type id,
    size_t offset
) noexcept {
    return is_valid_id(id)
        ? normalize_id(raw_normalize_id_(id) - (var_id_limits::space == domain_space::octdiff ? offset << 1 : offset))
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr bool var_id_traits<Domain, VarIdLimits>::id_equals(var_id_type a, var_id_type b) noexcept {
    return a == b;
}

template <domain_space Domain, typename VarIdLimits>
constexpr int var_id_traits<Domain, VarIdLimits>::id_compare(var_id_type a, var_id_type b) noexcept {
    return space == domain_space::oct
        ? var_id_traits<domain_space::octdiff, typename var_id_limits::counterpart_var_id_limits>::id_compare(
            id_to_counterpart(a), id_to_counterpart(b))
        : (b < a) - (a < b);
}

template <domain_space Domain, typename VarIdLimits>
constexpr int var_id_traits<Domain, VarIdLimits>::id_sign(var_id_type id) noexcept {
    return is_valid_id(id)
           ? (is_negative_id(id) ? -1 : 1)
           : 0;
}

template <domain_space Domain, typename VarIdLimits>
constexpr char const* var_id_traits<Domain, VarIdLimits>::id_sign_format(var_id_type id) noexcept {
    return is_valid_id(id)
        ? is_negative_id(id)
            ? var_id_limits::negative_var_name_format
            : var_id_limits::positive_var_name_format
        : "";
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::raw_normalize_id_(var_id_type id) noexcept {
    return var_id_limits::space == domain_space::oct
        ? (id < 0) ? -id : id
        : (id & 1) ? id : id - 1;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::name_to_id(char const* name) noexcept {
    return name_to_id(string_view(name));
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::name_to_id(string_view name) noexcept {
    constexpr long long int fallback_value = var_id_limits::invalid_var_id;
    return space == domain_space::oct
        ? arithmetic_to_valid(char_helper<char>::to_integer(fallback_value, name.data(), name.length()))
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
adl_IMPL typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::name_to_id(std::string name) {
    return name_to_id(string_view(name.data(), name.size()));
}

template <domain_space Domain, typename VarIdLimits>
template <typename N>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::arithmetic_to_range(N value) noexcept {
    var_id_type id = arithmetic_to_id_limits_(value);
    return id_to_range(var_id_type(value));
}

template <domain_space Domain, typename VarIdLimits>
template <typename N>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::arithmetic_to_valid(N value) noexcept {
    var_id_type id = arithmetic_to_id_limits_(value);
    return id_to_valid(var_id_type(value));
}

template <domain_space Domain, typename VarIdLimits>
template <typename N>
constexpr N var_id_traits<Domain, VarIdLimits>::id_to_arithmetic(var_id_type id, N fallback_value) noexcept {
    using return_limits = std::numeric_limits<N>;
    static_assert_valid_arithmetic_type<N>();
    // Here, we depend on type promotion...
    if (return_limits::max() >= id && return_limits::lowest() <= id && is_id_in_range(id)) return N(id);
    return fallback_value;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::id_to_range(var_id_type id) noexcept {
    return is_id_in_range(id)
        ? var_id_type(id)
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::id_to_valid(var_id_type id) noexcept {
    return is_valid_id(id)
        ? var_id_type(id)
        : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr std::size_t var_id_traits<Domain, VarIdLimits>::id_to_index(var_id_type id) noexcept {
    return var_id_limits::space == domain_space::octdiff
        ? size_t(id - 1)
        : var_id_traits<domain_space::octdiff, typename var_id_limits::counterpart_var_id_limits>::id_to_index(id_to_counterpart(id));
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::id_to_counterpart(var_id_type id) noexcept {
    return id == var_id_limits::end_var_id
        ? var_id_traits<var_id_limits::counterpart_space, typename var_id_limits::counterpart_var_id_limits>::var_id_limits::end_var_id
        : is_valid_id(id)
            ? var_id_limits::space == domain_space::oct
                ? var_id_type((id < 0) ? (-id << 1) : (id << 1) - 1)    // for oct
                : var_id_type((id & 1) ? (id >> 1) + 1 : -(id >> 1))
            : var_id_limits::invalid_var_id;
}

template <domain_space Domain, typename VarIdLimits>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::id_to_normal_oct(var_id_type id) noexcept {
    return var_id_limits::space == domain_space::oct
        ? normalize_id(id)
        : id_to_counterpart(normalize_id(id));
}

template <domain_space Domain, typename VarIdLimits>
adl_IMPL void var_id_traits<Domain, VarIdLimits>::id_to_name(
    std::string& name,
    var_id_type id,
    std::string const& base_name_format,
    std::string const& pos_neg_format
) {
    if (is_valid_id(id)) {
        string_printf(name, base_name_format, id_to_normal_oct(id));

        if (!pos_neg_format.empty()) {
            std::string formatted_name;
            string_printf(formatted_name, pos_neg_format, name);
            name = formatted_name;
        }
    }
}

template <domain_space Domain, typename VarIdLimits>
adl_IMPL std::string var_id_traits<Domain, VarIdLimits>::id_to_name(
    var_id_type id,
    std::string const& base_name_format,
    std::string const& pos_neg_format
) {
    std::string name;
    id_to_name(name, id, base_name_format, pos_neg_format);
    return name;
}

template <domain_space Domain, typename VarIdLimits>
template <typename N>
constexpr typename var_id_traits<Domain, VarIdLimits>::var_id_type
var_id_traits<Domain, VarIdLimits>::arithmetic_to_id_limits_(N value) noexcept {
    using return_limits = std::numeric_limits<N>;
    static_assert_valid_arithmetic_type<N>();
    if (value >= var_id_limits::min_var_id_value && value <= var_id_limits::max_var_id_value) return var_id_type(value);
    return var_id_limits::invalid_var_id;
}

adl_END_MAIN_MODULE


#endif //adl__oct__traits__hpp__