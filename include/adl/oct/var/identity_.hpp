// $flisboac 2017-04-23
/**
 * @file identity_.hpp
 */
#ifndef adl__oct__var__identity___hpp__
#define adl__oct__var__identity___hpp__

//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <>
struct adl_CLASS var_traits<oct_var> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::oct>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_type = oct_var;
    using counterpart_var_type = octdiff_var;

    // Constexpr static values
    constexpr static bool valid = true;
    constexpr static auto space = var_id_traits::space;
    constexpr static auto counterpart_space = var_id_traits::counterpart_space;

    // static functions
    constexpr static var_type var_with_id(const var_type& var, var_id_type id);
    constexpr static counterpart_var_type var_to_counterpart(const var_type& var, var_id_type counterpart_id);
};

template <>
struct adl_CLASS var_traits<octdiff_var> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::octdiff>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_type = octdiff_var;
    using counterpart_var_type = oct_var;

    // Constexpr static values
    constexpr static bool valid = true;
    constexpr static auto space = var_id_traits::space;
    constexpr static auto counterpart_space = var_id_traits::counterpart_space;

    // static functions
    constexpr static var_type var_with_id(const var_type& var, var_id_type id);
    constexpr static counterpart_var_type var_to_counterpart(const var_type& var, var_id_type counterpart_id);
};

class adl_CLASS oct_var : public unnamed_var_base_<oct_var> {
public:
    using unnamed_var_base_<oct_var>::unnamed_var_base_;
};

class adl_CLASS octdiff_var : public unnamed_var_base_<octdiff_var> {
public:
    using unnamed_var_base_<octdiff_var>::unnamed_var_base_;
};

namespace literals {
    inline namespace oct {
        constexpr oct::oct_var operator "" _ov(unsigned long long int id);
    } // oct
} // literals


adl_END_MAIN_MODULE

template <typename Traits>
inline std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    const adl::oct::oct_var& var
);

template <typename Traits>
inline std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    const adl::oct::octdiff_var& var
);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

//
// specialization: var_traits<oct_var>
//
constexpr typename var_traits<oct_var>::var_type
var_traits<oct_var>::var_with_id(
    const var_traits<oct_var>::var_type& var,
    var_id_type id
) {
    return var_type(id);
}

constexpr typename var_traits<oct_var>::counterpart_var_type
var_traits<oct_var>::var_to_counterpart(
    const var_traits<oct_var>::var_type& var,
    var_id_type counterpart_id
) {
    return counterpart_var_type(counterpart_id);
}

//
// specialization: var_traits<octdiff_var>
//
constexpr typename var_traits<octdiff_var>::var_type
var_traits<octdiff_var>::var_with_id(
    const var_traits<octdiff_var>::var_type& var,
    var_id_type id
) {
    return var_type(id);
}

constexpr typename var_traits<octdiff_var>::counterpart_var_type
var_traits<octdiff_var>::var_to_counterpart(
    const var_traits<octdiff_var>::var_type& var,
    var_id_type counterpart_id
) {
    return counterpart_var_type(counterpart_id);
}

adl_END_MAIN_MODULE
adl_BEGIN_ROOT_MODULE

namespace literals {
inline namespace oct {

    constexpr oct::oct_var operator ""_ov(unsigned long long int id) {
        return oct::oct_var(static_cast<long long int>(id));
    }

} // namespace oct
} // namespace literals

adl_END_ROOT_MODULE

template <typename Traits>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    const adl::oct::octdiff_var& var
) {
    using namespace adl::operators::oct::var::ios_;
    return os << var;
}

template <typename Traits>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    const adl::oct::oct_var& var
) {
    using namespace adl::operators::oct::var::ios_;
    return os << var;
}


#endif //adl__oct__var__identity___hpp__
