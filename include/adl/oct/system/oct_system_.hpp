// $flisboac 2017-07-10
/**
 * @file oct_system_.hpp
 */
#ifndef adl__oct__system__oct_system___hpp__
#define adl__oct__system__oct_system___hpp__


#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

#include "adl/oct/system/system_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename ValueLimits>
class oct_system : public system_base_<domain_space::oct, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::oct, ValueType, ValueLimits>;

public:
    using typename superclass_::counterpart_system_type;
    using typename superclass_::identity_cons_type;
    using typename superclass_::value_type;
    using typename superclass_::literal_var_type;
    using typename superclass_::iterator;
    using typename superclass_::const_iterator;
    using typename superclass_::constant_type;

    using superclass_::space;
    using superclass_::constraints_;
    using superclass_::variables_;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space> >
        std::size_t count(oct_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space> >
        const_iterator find(oct_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space> >
        value_type const& at(oct_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space> >
        constant_type const& operator[](oct_vexpr<VarType_> vexpr) const;

    oct_system& clear();
    oct_system& reset();
    template <typename VarType_, typename = common_var_t<VarType_>>
        void setup_var(VarType_ var); // Can only be done if the variable hasn't been configured before
    template <typename ValueType_, typename VarType_, typename = std::enable_if<
        std::is_convertible<ValueType_, ValueType>::value
        && common_cons<ValueType_, VarType_>::is_oct_space>>
        std::pair<iterator, bool> insert(oct_cons<ValueType_, VarType_> cons);
    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        std::size_t erase(oct_vexpr<common_var_t<VarType_>> vexpr);

    counterpart_system_type to_counterpart() const;

private:
    template <typename VarType_, typename = common_var_t<VarType_>>
        value_type to_value_(oct_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = common_var_t<VarType_>>
        value_type to_value_(oct_vexpr<VarType_> vexpr);
    template <typename ValueType_, typename VarType_, typename = std::enable_if_t<
        common_cons<ValueType_, VarType_>::valid && std::is_convertible<ValueType, ValueType_>::value> >
        value_type to_value_(oct_cons<ValueType_, VarType_> cons);
    template <typename VarType_, typename = common_var_t<VarType_>>
        literal_var_type to_var_(VarType_ var) const;
    template <typename VarType_, typename = common_var_t<VarType_>>
        literal_var_type to_var_(VarType_ var);
};

} // namespace oct

adl_END_ROOT_MODULE

template <typename CharType, typename CharTraits, typename ValueType, typename ValueLimits>
std::basic_ostream<CharType, CharTraits>& operator<<(
    std::basic_ostream<CharType, CharTraits>& os,
    adl::oct::oct_system<ValueType, ValueLimits> const& system);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// oct_system
//

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline std::size_t oct_system<ValueType, ValueLimits>::count(oct_vexpr<VarType_> vexpr) const {
    auto idx = to_value_(vexpr);
    return constraints_.count(idx);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::const_iterator
oct_system<ValueType, ValueLimits>::find(oct_vexpr<VarType_> vexpr) const {
    auto idx = to_value_(vexpr);
    return constraints_.find(idx);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::value_type const&
oct_system<ValueType, ValueLimits>::at(oct_vexpr<VarType_> vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::constant_type const&
oct_system<ValueType, ValueLimits>::operator[](oct_vexpr<VarType_> vexpr) const {
    return at(vexpr).c();
}

template <typename ValueType, typename ValueLimits>
inline oct_system<ValueType, ValueLimits>&
oct_system<ValueType, ValueLimits>::clear() {
    constraints_.clear();
    variables_.clear();
    return *this;
}

template <typename ValueType, typename ValueLimits>
inline oct_system<ValueType, ValueLimits>&
oct_system<ValueType, ValueLimits>::reset() {
    constraints_.clear();
    variables_.reset();
    return *this;
}

template <typename ValueType, typename ValueLimits>
template <typename VarType, typename>
inline void oct_system<ValueType, ValueLimits>::setup_var(VarType var) {
    variables_.setup(var);
}

template <typename ValueType, typename ValueLimits>
template <typename ValueType_, typename VarType_, typename>
inline std::pair<typename oct_system<ValueType, ValueLimits>::iterator, bool>
oct_system<ValueType, ValueLimits>::insert(oct_cons<ValueType_, VarType_> cons) {
    auto value = to_value_(cons);
    return constraints_.insert(value);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline std::size_t
oct_system<ValueType, ValueLimits>::erase(oct_vexpr<common_var_t<VarType_>> vexpr) {
    auto value = to_value_(vexpr);
    return constraints_.erase(value);
}

template <typename ValueType, typename ValueLimits>
inline typename oct_system<ValueType, ValueLimits>::counterpart_system_type
oct_system<ValueType, ValueLimits>::to_counterpart() const {
    return counterpart_system_type(*this);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::value_type
oct_system<ValueType, ValueLimits>::to_value_(oct_vexpr<VarType_> vexpr) const {
    if (!vexpr.valid()) throw std::logic_error("Invalid variable expression.");
    auto real_xi = to_var_(vexpr.xi());
    auto real_xj = (vexpr.xj().valid()) ? to_var_(vexpr.xj()) : literal_var_type::invalid();
    typename value_type::vexpr_type real_vexpr(real_xi, real_xj);
    return value_type(real_vexpr, ValueType());
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::value_type
oct_system<ValueType, ValueLimits>::to_value_(oct_vexpr<VarType_> vexpr) {
    if (!vexpr.valid()) throw std::logic_error("Invalid variable expression.");
    auto real_xi = to_var_(vexpr.xi());
    auto real_xj = (vexpr.xj().valid()) ? to_var_(vexpr.xj()) : literal_var_type::invalid();
    typename value_type::vexpr_type real_vexpr(real_xi, real_xj);
    return value_type(real_vexpr, ValueType());
}

template <typename ValueType, typename ValueLimits>
template <typename ValueType_, typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::value_type
oct_system<ValueType, ValueLimits>::to_value_(oct_cons<ValueType_, VarType_> cons) {
    if (!cons.valid()) throw std::logic_error("Invalid constraint.");
    auto real_xi = to_var_(cons.xi());
    auto real_xj = (cons.xj().valid()) ? to_var_(cons.xj()) : literal_var_type::invalid();;
    typename value_type::vexpr_type real_vexpr(real_xi, real_xj);
    return value_type(real_vexpr, cons.c());
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::literal_var_type
oct_system<ValueType, ValueLimits>::to_var_(VarType_ var) const {
    literal_var_type real_var = variables_.get(var).normalized_var();
    return real_var;
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename oct_system<ValueType, ValueLimits>::literal_var_type
oct_system<ValueType, ValueLimits>::to_var_(VarType_ var) {
    auto iter = variables_.insert(var);
    auto& var_data = *iter.first;
    literal_var_type real_var = var_data.normalized_var();
    return real_var;
}

} // namespace oct
adl_END_ROOT_MODULE

template <typename CharType, typename CharTraits, typename ValueType, typename ValueLimits>
std::basic_ostream<CharType, CharTraits>& operator<<(
    std::basic_ostream<CharType, CharTraits>& os,
    adl::oct::oct_system<ValueType, ValueLimits> const& system
) {
    os << system.to_string();
    return os;
}

#endif //adl__oct__system__oct_system___hpp__
