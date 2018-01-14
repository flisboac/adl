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
    using typename superclass_::key_type;
    using typename superclass_::value_type;
    using typename superclass_::literal_octdiff_conjunction_type;
    using typename superclass_::constant_type;

    octdiff_system() = default;
    octdiff_system(octdiff_system const&) = default;
    octdiff_system(octdiff_system &&) noexcept = default;
    octdiff_system& operator=(octdiff_system const&) = default;
    octdiff_system& operator=(octdiff_system &&) noexcept = default;

    octdiff_system(counterpart_system_type const& oct_sys);

    std::size_t count(oct_var var) const;
    std::size_t count(octdiff_vexpr vexpr) const;
    std::size_t count(oct_vexpr vexpr) const;
    std::size_t count(octdiff_var xi, octdiff_var xj) const;

    const_iterator find(oct_var var) const;
    const_iterator find(octdiff_vexpr vexpr) const;
    const_iterator find(oct_vexpr vexpr) const;
    const_iterator find(octdiff_var xi, octdiff_var xj) const;

    value_type get(oct_var var) const;
    value_type get(octdiff_vexpr vexpr) const;
    value_type get(oct_vexpr vexpr) const;
    value_type get(octdiff_var xi, octdiff_var xj) const;

    value_type const& at(oct_var var) const;
    value_type const& at(octdiff_vexpr vexpr) const;
    value_type const& at(oct_vexpr vexpr) const;

    constant_type const& operator[](oct_var var) const;
    constant_type const& operator[](octdiff_vexpr vexpr) const;
    constant_type const& operator[](oct_vexpr vexpr) const;

private:
    template <typename VarType_, typename ValueType_, typename = std::enable_if<
        std::is_convertible<ValueType, ValueType_>::value
        && common_var<VarType_>::is_octdiff_space>>
        std::pair<iterator, bool> insert_(basic_octdiff_cons<ValueType_, VarType_> cons);
    key_type to_key_(oct_var var) const;
    template <typename VarType_, typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
        key_type to_key_(basic_octdiff_vexpr<VarType_> vexpr) const;
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
inline std::size_t octdiff_system<ValueType, ValueLimits>::count(oct_var var) const {
    const auto key = to_key_(var);
    return key.valid() ? constraints_.count(key) : 0;
};

template <typename ValueType, typename ValueLimits>
inline std::size_t octdiff_system<ValueType, ValueLimits>::count(octdiff_vexpr vexpr) const {
    const auto key = to_key_(vexpr);
    return key.valid() ? constraints_.count(key) : 0;
};

template <typename ValueType, typename ValueLimits>
inline std::size_t octdiff_system<ValueType, ValueLimits>::count(oct_vexpr vexpr) const {
    const auto conj = split_(vexpr);
    const auto key_di = key_type(conj.di());
    const auto key_dj = key_type(conj.dj());
    return (key_di.valid() ? constraints_.count(key_di) : 0) + (key_dj.valid() ? constraints_.count(key_dj) : 0);
};

template <typename ValueType, typename ValueLimits>
inline std::size_t octdiff_system<ValueType, ValueLimits>::count(octdiff_var xi, octdiff_var xj) const {
    return this->count(octdiff_vexpr::make_sub(xi, xj));
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(oct_var var) const {
    const auto key = to_key_(var);
    return key.valid() ? constraints_.find(key) : constraints_.end();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(octdiff_vexpr vexpr) const {
    const auto key = to_key_(vexpr);
    return key.valid() ? constraints_.find(key) : constraints_.end();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(oct_vexpr vexpr) const {
    const auto split = split_(vexpr);
    if (split.di().valid()) {
        auto iter = constraints_.find(split.di());
        if (iter != constraints_.end()) return iter;
    }
    if (split.dj().valid()) {
        auto iter = constraints_.find(split.dj());
        if (iter != constraints_.end()) return iter;
    }
    return constraints_.end();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::const_iterator
octdiff_system<ValueType, ValueLimits>::find(octdiff_var xi, octdiff_var xj) const {
    return this->find(octdiff_vexpr::make_sub(xi, xj));
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type
octdiff_system<ValueType, ValueLimits>::get(oct_var var) const {
    auto iter = find(var);
    if (iter != this->end()) return *iter;
    return value_type::invalid();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type
octdiff_system<ValueType, ValueLimits>::get(octdiff_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter != this->end()) return *iter;
    return value_type::invalid();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type
octdiff_system<ValueType, ValueLimits>::get(oct_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter != this->end()) return *iter;
    return value_type::invalid();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type
octdiff_system<ValueType, ValueLimits>::get(octdiff_var xi, octdiff_var xj) const {
    return this->get(octdiff_vexpr(xi, xj));
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type const&
octdiff_system<ValueType, ValueLimits>::at(oct_var var) const {
    auto iter = find(var);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
}

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type const&
octdiff_system<ValueType, ValueLimits>::at(octdiff_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
}

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::value_type const&
octdiff_system<ValueType, ValueLimits>::at(oct_vexpr vexpr) const {
    auto iter = find(vexpr);
    if (iter == this->end()) throw std::logic_error("Constraint not found.");
    return *iter;
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::constant_type const&
octdiff_system<ValueType, ValueLimits>::operator[](oct_var var) const {
    return at(var).c();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::constant_type const&
octdiff_system<ValueType, ValueLimits>::operator[](octdiff_vexpr vexpr) const {
    return at(vexpr).c();
};

template <typename ValueType, typename ValueLimits>
inline typename octdiff_system<ValueType, ValueLimits>::constant_type const&
octdiff_system<ValueType, ValueLimits>::operator[](oct_vexpr vexpr) const {
    return at(vexpr).c();
};

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
inline typename octdiff_system<ValueType, ValueLimits>::key_type
octdiff_system<ValueType, ValueLimits>::to_key_(oct_var var) const {
    return to_key_(split_(oct_vexpr::make_unit(var)).di());
};

template <typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename octdiff_system<ValueType, ValueLimits>::key_type
octdiff_system<ValueType, ValueLimits>::to_key_(basic_octdiff_vexpr <VarType_> vexpr) const {
    return value_type(vexpr);
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
