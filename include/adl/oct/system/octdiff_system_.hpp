// $flisboac 2017-07-10
/**
 * @file octdiff_system_.hpp
 */
#ifndef adl__oct__system__octdiff_system___hpp__
#define adl__oct__system__octdiff_system___hpp__


#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/system/system_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename ValueLimits>
class octdiff_system : public system_base_<domain_space::octdiff, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::octdiff, ValueType, ValueLimits>;

public:
    using superclass_::constraints_;
    using superclass_::variables_;
    using superclass_::space;

    using typename superclass_::counterpart_system_type;
    using typename superclass_::iterator;
    using typename superclass_::const_iterator;
    using typename superclass_::value_type;
    using typename superclass_::literal_octdiff_conjunction_type;
    using typename superclass_::constant_type;

    octdiff_system() = default;
    octdiff_system(octdiff_system const&) = default;
    octdiff_system(octdiff_system &&) noexcept = default;
    octdiff_system& operator=(octdiff_system const&) = default;
    octdiff_system& operator=(octdiff_system &&) noexcept = default;

    octdiff_system(counterpart_system_type const& oct_sys);

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        std::size_t count(basic_octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        std::size_t count(basic_oct_vexpr<VarType_> vexpr) const;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        const_iterator find(basic_octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        const_iterator find(basic_oct_vexpr<VarType_> vexpr) const;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        value_type const& get(basic_octdiff_vexpr <VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        value_type const& get(basic_oct_vexpr <VarType_> vexpr) const;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type const& operator[](basic_octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type const& operator[](basic_oct_vexpr<VarType_> vexpr) const;

private:
    template <typename VarType_, typename ValueType_, typename = std::enable_if<
        std::is_convertible<ValueType, ValueType_>::value
        && common_var<VarType_>::is_octdiff_space>>
        std::pair<iterator, bool> insert_(basic_octdiff_cons<ValueType_, VarType_> cons);
    template <typename VarType_, typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
        value_type to_value_(basic_octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if_t<common_var<VarType_>::is_oct_space>>
        literal_octdiff_conjunction_type split_(basic_oct_vexpr<VarType_> vexpr) const;
};

} // namespace oct

adl_END_ROOT_MODULE

template <typename CharType, typename CharTraits, typename ValueType, typename ValueLimits>
std::basic_ostream<CharType, CharTraits>& operator<<(
    std::basic_ostream<CharType, CharTraits>& os,
    adl::oct::octdiff_system<ValueType, ValueLimits> const& system);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// octdiff_system
//

template <typename ValueType, typename ValueLimits>
octdiff_system<ValueType, ValueLimits>::octdiff_system(counterpart_system_type const& oct_sys) {
    for (auto& cons : oct_sys) {
        auto conjunction = cons.split();
        insert_(conjunction.di());
        insert_(conjunction.dj());
    }
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline std::size_t
octdiff_system<ValueType, ValueLimits>::count(basic_octdiff_vexpr<VarType_> vexpr) const {
    auto value = to_value_(vexpr);
    return constraints_.count(value);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline std::size_t
octdiff_system<ValueType, ValueLimits>::count(basic_oct_vexpr<VarType_> vexpr) const {
    std::size_t c = 0;
    auto split = split_(vexpr);
    if (split.di().valid()) c += count(split.di());
    if (split.dj().valid()) c += count(split.dj());
    return c;
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(basic_octdiff_vexpr<VarType_> vexpr) const {
    auto value = to_value_(vexpr);
    return constraints_.find(value);
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(basic_oct_vexpr<VarType_> vexpr) const {
    auto split = split_(vexpr);
    if (split.di().valid()) {
        auto iter = constraints_.find(split.di());
        if (iter != constraints_.end()) return iter;
    }
    if (split.dj().valid()) {
        auto iter = constraints_.find(split.dj());
        if (iter != constraints_.end()) return iter;
    }
    return constraints_.end();
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::value_type const&
octdiff_system<ValueType, ValueLimits>::get(basic_octdiff_vexpr <VarType_> vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::value_type const&
octdiff_system<ValueType, ValueLimits>::get(basic_oct_vexpr <VarType_> vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
};

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::constant_type const&
octdiff_system<ValueType, ValueLimits>::operator[](basic_octdiff_vexpr<VarType_> vexpr) const {
    return get(vexpr).c();
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::constant_type const&
octdiff_system<ValueType, ValueLimits>::operator[](basic_oct_vexpr<VarType_> vexpr) const {
    return get(vexpr).c();
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename ValueType_, typename>
inline std::pair<typename octdiff_system<ValueType, ValueLimits>::iterator, bool>
octdiff_system<ValueType, ValueLimits>::insert_(basic_octdiff_cons<ValueType_, VarType_> cons) {
    if (cons.valid()) {
        auto iter = constraints_.insert(cons);
        if (cons.xi().valid()) variables_.insert(cons.xi());
        if (cons.xj().valid()) variables_.insert(cons.xj());
        return iter;
    }
    return {constraints_.end(), false};
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::value_type
octdiff_system<ValueType, ValueLimits>::to_value_(basic_octdiff_vexpr<VarType_> vexpr) const {
    return value_type(vexpr, value_type());
}

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::literal_octdiff_conjunction_type
octdiff_system<ValueType, ValueLimits>::split_(basic_oct_vexpr<VarType_> vexpr) const {
    if (vexpr.valid()) {
        const basic_oct_cons<value_type, VarType_> cons(vexpr, value_type());
        auto split = cons.split();
        return split;
    }
    return literal_octdiff_conjunction_type::invalid();
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__system__octdiff_system___hpp__
