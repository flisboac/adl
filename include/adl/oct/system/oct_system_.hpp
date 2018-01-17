// $flisboac 2017-07-10
/**
 * @file oct_system_.hpp
 */
#ifndef adl__oct__system__oct_system___hpp__
#define adl__oct__system__oct_system___hpp__

#include <initializer_list>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"

#include "adl/oct/system/system_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ConstantType, typename ValueLimits>
class oct_system : public system_base_<domain_space::oct, ConstantType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::oct, ConstantType, ValueLimits>;

public:
    using typename superclass_::counterpart_system_type;
    using typename superclass_::identity_cons_type;
    using typename superclass_::key_type;
    using typename superclass_::constant_type;
    using typename superclass_::literal_var_type;
    using typename superclass_::iterator;
    using typename superclass_::const_iterator;
    using typename superclass_::value_type;
    using typename superclass_::literal_cons_type;
    using typename superclass_::literal_vexpr_type;

    using superclass_::space;
    using superclass_::constraints_;
    using superclass_::variables_;

    oct_system() = default;
    oct_system(oct_system const&) = default;
    oct_system(oct_system &&) noexcept = default;
    oct_system& operator=(oct_system const&) = default;
    oct_system& operator=(oct_system &&) noexcept = default;

    oct_system(std::initializer_list<adl::oct::oct_cons<constant_type>> list);
    oct_system(std::initializer_list<adl::oct::oct_lcons<constant_type>> list);

    oct_system& clear();
    oct_system& reset();

    std::size_t count(oct_var var) const;
    std::size_t count(oct_vexpr vexpr) const;

    const_iterator find(oct_var var) const;
    const_iterator find(oct_vexpr vexpr) const;

    value_type get(oct_var var) const;
    value_type get(oct_vexpr vexpr) const;

    value_type const& at(oct_var var) const;
    value_type const& at(oct_vexpr vexpr) const;

    constant_type const& operator[](oct_var var) const;
    constant_type const& operator[](oct_vexpr vexpr) const;

    template <typename VarType_, typename = common_var_t<VarType_>>
        void setup_var(VarType_ var); // Can only be done if the variable hasn't been configured before
    template <typename ConstantType_, typename VarType_, typename = std::enable_if<
        std::is_convertible<ConstantType_, ConstantType>::value
        && common_cons<ConstantType_, VarType_>::is_oct_space>>
        std::pair<iterator, bool> insert(basic_oct_cons<ConstantType_, VarType_> cons);
    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        std::size_t erase(basic_oct_vexpr<common_var_t<VarType_>> vexpr);

    counterpart_system_type to_counterpart() const;

private:
    template <typename VarType_, typename = common_var_t<VarType_>>
        value_type to_value_(VarType_ var) const;
    template <typename VarType_, typename = common_var_t<VarType_>>
        value_type to_value_(basic_oct_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = common_var_t<VarType_>>
        value_type to_value_(basic_oct_vexpr<VarType_> vexpr);
    template <typename ConstantType_, typename VarType_, typename = std::enable_if_t<
        common_cons<ConstantType_, VarType_>::valid && std::is_convertible<ConstantType, ConstantType_>::value> >
        value_type to_value_(basic_oct_cons<ConstantType_, VarType_> cons);
    template <typename VarType_, typename = common_var_t<VarType_>>
        literal_var_type to_var_(VarType_ var) const;
    template <typename VarType_, typename = common_var_t<VarType_>>
        literal_var_type to_var_(VarType_ var);
};

} // namespace oct

adl_END_ROOT_MODULE

template <typename CharType, typename CharTraits, typename ConstantType, typename ValueLimits>
std::basic_ostream<CharType, CharTraits>& operator<<(
    std::basic_ostream<CharType, CharTraits>& os,
    adl::oct::oct_system<ConstantType, ValueLimits> const& system);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// oct_system
//
//template <typename ConstantType, typename ValueLimits>
//template <typename ConstantType_, typename VarType_, typename>
//inline oct_system<ConstantType, ValueLimits>::oct_system(
//    std::initializer_list<adl::oct::basic_oct_cons<ConstantType_, VarType_>> list
//) {
//    for (auto cons : list) {
//        constant_type value = cons.constant();
//        insert(literal_cons_type(cons.to_vexpr(), value));
//    }
//};

template <typename ConstantType, typename ValueLimits>
inline oct_system<ConstantType, ValueLimits>::oct_system(std::initializer_list<adl::oct::oct_cons<constant_type>> list) {
    for (auto cons : list) {
        constant_type value = cons.constant();
        insert(cons);
    }
};

template <typename ConstantType, typename ValueLimits>
inline oct_system<ConstantType, ValueLimits>::oct_system(std::initializer_list<adl::oct::oct_lcons<constant_type>> list) {
    for (auto cons : list) {
        constant_type value = cons.constant();
        insert(cons);
    }
};

template <typename ConstantType, typename ValueLimits>
inline oct_system<ConstantType, ValueLimits>&
oct_system<ConstantType, ValueLimits>::clear() {
    constraints_.clear();
    variables_.clear();
    return *this;
}

template <typename ConstantType, typename ValueLimits>
inline oct_system<ConstantType, ValueLimits>&
oct_system<ConstantType, ValueLimits>::reset() {
    constraints_.clear();
    variables_.reset();
    return *this;
}

template <typename ConstantType, typename ValueLimits>
inline std::size_t oct_system<ConstantType, ValueLimits>::count(oct_var var) const {
    auto key = to_value_(var);
    return constraints_.count(key);
};

template <typename ConstantType, typename ValueLimits>
inline std::size_t oct_system<ConstantType, ValueLimits>::count(oct_vexpr vexpr) const {
    auto key = to_value_(vexpr);
    return constraints_.count(key);
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::const_iterator
oct_system<ConstantType, ValueLimits>::find(oct_var var) const {
    auto idx = to_value_(var);
    return constraints_.find(idx);
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::const_iterator
oct_system<ConstantType, ValueLimits>::find(oct_vexpr vexpr) const {
    auto idx = to_value_(vexpr);
    return constraints_.find(idx);
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::get(oct_var var) const {
    auto iter = find(var);
    if (iter != this->end()) return *iter;
    return value_type::invalid();
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::get(oct_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter != this->end()) return *iter;
    return value_type::invalid();
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::value_type const&
oct_system<ConstantType, ValueLimits>::at(oct_var var) const {
    auto iter = find(var);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::value_type const&
oct_system<ConstantType, ValueLimits>::at(oct_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::constant_type const&
oct_system<ConstantType, ValueLimits>::operator[](oct_var var) const {
    return at(var).c();
};

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::constant_type const&
oct_system<ConstantType, ValueLimits>::operator[](oct_vexpr vexpr) const {
    return at(vexpr).c();
};

template <typename ConstantType, typename ValueLimits>
template <typename VarType, typename>
inline void oct_system<ConstantType, ValueLimits>::setup_var(VarType var) {
    variables_.setup(var);
}

template <typename ConstantType, typename ValueLimits>
template <typename ConstantType_, typename VarType_, typename>
inline std::pair<typename oct_system<ConstantType, ValueLimits>::iterator, bool>
oct_system<ConstantType, ValueLimits>::insert(basic_oct_cons<ConstantType_, VarType_> cons) {
    auto value = to_value_(cons);
    return constraints_.insert(value);
}

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline std::size_t
oct_system<ConstantType, ValueLimits>::erase(basic_oct_vexpr<common_var_t<VarType_>> vexpr) {
    auto value = to_value_(vexpr);
    return constraints_.erase(value);
}

template <typename ConstantType, typename ValueLimits>
inline typename oct_system<ConstantType, ValueLimits>::counterpart_system_type
oct_system<ConstantType, ValueLimits>::to_counterpart() const {
    return counterpart_system_type(*this);
}

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::to_value_(VarType_ var) const {
    auto vexpr = literal_vexpr_type::make_unit(var);
    return to_value_(vexpr);
};

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::to_value_(basic_oct_vexpr<VarType_> vexpr) const {
    if (!vexpr.valid()) throw std::logic_error("Invalid variable expression.");
    auto real_xi = to_var_(vexpr.xi());
    auto real_xj = (vexpr.xj().valid()) ? to_var_(vexpr.xj()) : literal_var_type::invalid();
    typename value_type::vexpr_type real_vexpr(real_xi, real_xj);
    return value_type(real_vexpr, constant_type());
}

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::to_value_(basic_oct_vexpr<VarType_> vexpr) {
    return to_value_(basic_oct_cons<VarType_, constant_type>::make_upper_limit(vexpr, constant_type()));
}

template <typename ConstantType, typename ValueLimits>
template <typename ConstantType_, typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::value_type
oct_system<ConstantType, ValueLimits>::to_value_(basic_oct_cons<ConstantType_, VarType_> cons) {
    if (!cons.valid()) throw std::logic_error("Invalid constraint.");
    auto real_xi = to_var_(cons.xi());
    auto real_xj = (cons.xj().valid()) ? to_var_(cons.xj()) : literal_var_type::invalid();
    typename value_type::vexpr_type real_vexpr(real_xi, real_xj);
    return value_type(real_vexpr, cons.c());
}

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::literal_var_type
oct_system<ConstantType, ValueLimits>::to_var_(VarType_ var) const {
    literal_var_type real_var = variables_.at(var).normalized_var();
    if (var.negative()) real_var.negate();
    return real_var;
}

template <typename ConstantType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ConstantType, ValueLimits>::literal_var_type
oct_system<ConstantType, ValueLimits>::to_var_(VarType_ var) {
    auto iter = variables_.insert(var);
    auto& var_data = *iter.first;
    literal_var_type real_var = var_data.normalized_var();
    if (var.negative()) real_var.negate();
    return real_var;
}

} // namespace oct
adl_END_ROOT_MODULE

template <typename CharType, typename CharTraits, typename ConstantType, typename ValueLimits>
std::basic_ostream<CharType, CharTraits>& operator<<(
    std::basic_ostream<CharType, CharTraits>& os,
    adl::oct::oct_system<ConstantType, ValueLimits> const& system
) {
    os << system.to_string();
    return os;
}

#endif //adl__oct__system__oct_system___hpp__
