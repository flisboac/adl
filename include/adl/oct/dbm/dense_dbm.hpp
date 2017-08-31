// $flisboac 2017-08-13
/**
 * @file dense_dbm.hpp.hpp
 */
#ifndef adl__oct__dbm__dense_dbm__hpp__
#define adl__oct__dbm__dense_dbm__hpp__

#include <vector>
#include <iosfwd>
#include <type_traits>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/dbm/dbm_base_.hpp"
#include "adl/oct/dbm/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {


template <typename Subclass, typename ValueType, typename ValueLimits>
class dense_dbm_base_ : public dbm_base_<Subclass, ValueType, ValueLimits> {
private:
    using superclass_ = dbm_base_<Subclass, ValueType, ValueLimits>;

protected:
    using traits_ = dbm_traits<Subclass>;

public:
    friend class crtp_base<Subclass>;

    using typename superclass_::subclass_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::value_limits;
    using typename superclass_::identity_cons_type;

    using superclass_::no_index;
    using superclass_::to_end_index_;

    dense_dbm_base_(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_(dense_dbm_base_ &&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ &&) noexcept = default;

protected:
    explicit dense_dbm_base_(dbm_major major) noexcept;

public:
    bool dense() const noexcept;
    bool autocoherent() const noexcept;

    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        identity_cons_type get(VarTypeA_ xi, VarTypeB_ xj) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        identity_cons_type get(octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        identity_cons_type get(oct_vexpr<VarType_> vexpr) const;

    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        constant_type const& at(VarTypeA_ xi, VarTypeB_ xj) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type const& at(octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type const& at(oct_vexpr<VarType_> vexpr) const;

    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        constant_type& at(VarTypeA_ xi, VarTypeB_ xj);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type& at(octdiff_vexpr<VarType_> vexpr);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type& at(oct_vexpr<VarType_> vexpr);

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type const& operator[](octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type const& operator[](oct_vexpr<VarType_> vexpr) const;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type& operator[](octdiff_vexpr<VarType_> vexpr);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type& operator[](oct_vexpr<VarType_> vexpr);

protected:
    dbm_major const major_;
};

template <typename ValueType, typename ValueLimits, typename Allocator>
class dense_dbm : public dense_dbm_base_<dense_dbm<ValueType, ValueLimits>, ValueType, ValueLimits> {
private:
    using superclass_ = dense_dbm_base_<dense_dbm, ValueType, ValueLimits>;
    using container_type_ = std::vector<ValueType, Allocator>;

public:
    using typename superclass_::superclass_;
    using typename superclass_::subclass_;
    using typename superclass_::traits_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::value_limits;

    using allocator_type = Allocator;

    dense_dbm();
    dense_dbm(dense_dbm const&) = default;
    dense_dbm(dense_dbm &&) noexcept = default;
    dense_dbm& operator=(dense_dbm const&) = default;
    dense_dbm& operator=(dense_dbm &&) noexcept = default;

    template <typename ValueType_,
            typename ValueLimits_,
            typename = std::enable_if_t<std::is_convertible<ValueType, ValueType_>::value> >
        dense_dbm(octdiff_system<ValueType_, ValueLimits_> const& rhs, dbm_major major = traits_::default_major);
    explicit dense_dbm(std::size_t max_vars, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        explicit dense_dbm(VarType_ last_var, dbm_major major = traits_::default_major);

    explicit dense_dbm(std::size_t max_vars, constant_type value, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        explicit dense_dbm(VarType_ last_var, constant_type value, dbm_major major = traits_::default_major);

    static dense_dbm const null();
    static dense_dbm top(std::size_t max_vars, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        static dense_dbm top(VarType_ last_var, dbm_major major = traits_::default_major);
    static dense_dbm bottom(std::size_t max_vars, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        static dense_dbm bottom(VarType_ last_var, dbm_major major = traits_::default_major);

    std::size_t size() const noexcept;
    allocator_type get_allocator() const;

    void initialize(constant_type value);
    template <typename VarType_, typename = common_var_t<VarType_>>
        void resize(VarType_ new_last_var, constant_type value = value_limits::top());

private:
    constant_type& constant_(std::size_t index);
    constant_type const& constant_(std::size_t index) const;

public:
    container_type_ data_;
};

} // namespace oct
adl_END_ROOT_MODULE


template <typename CharType,
        typename CharTraits,
        typename DbmType,
        typename = adl::oct::is_valid_dbm_t<DbmType>>
std::basic_ostream<CharType, CharTraits>& operator<<(
        std::basic_ostream<CharType, CharTraits>& os,
        adl::oct::dense_dbm_base_<DbmType, typename DbmType::value_type, typename DbmType::value_limits> const& dbm);


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// dense_dbm_base_
//
template <typename SubClass, typename ValueType, typename ValueLimits>
dense_dbm_base_<SubClass, ValueType, ValueLimits>::dense_dbm_base_(dbm_major major) noexcept
    : major_(major) {};

template <typename SubClass, typename ValueType, typename ValueLimits>
bool dense_dbm_base_<SubClass, ValueType, ValueLimits>::dense() const noexcept {
    return true;
};

template <typename SubClass, typename ValueType, typename ValueLimits>
bool dense_dbm_base_<SubClass, ValueType, ValueLimits>::autocoherent() const noexcept {
    return false;
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarTypeA_, typename VarTypeB_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::identity_cons_type
dense_dbm_base_<SubClass, ValueType, ValueLimits>::get(VarTypeA_ xi, VarTypeB_ xj) const {
    auto constant = at(xi, xj);
    if (!value_limits::is_null(constant)) return identity_cons_type(identity_vexpr_type(xi, xj), constant);
    return identity_cons_type::invalid();
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::identity_cons_type
dense_dbm_base_<SubClass, ValueType, ValueLimits>::get(octdiff_vexpr<VarType_> vexpr) const {
    return get(vexpr.xi(), vexpr.xj());
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::identity_cons_type
dense_dbm_base_<SubClass, ValueType, ValueLimits>::get(oct_vexpr<VarType_> vexpr) const {
    const oct_cons<VarType_, ValueType> cons(vexpr, ValueType());
    auto conj = cons.split();
    auto constant_di = at(conj.di());
    auto constant_dj = at(conj.dj());
    if (constant_di == constant_dj) return identity_cons_type::invalid();
    return identity_cons_type(conj.di().to_identity_vexpr(), constant_di);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarTypeA_, typename VarTypeB_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(VarTypeA_ xi, VarTypeB_ xj) const {
    return this->as_const_().at(xi, xj);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(octdiff_vexpr<VarType_> vexpr) const {
    return this->as_const_().at(vexpr);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(oct_vexpr<VarType_> vexpr) const {
    return this->as_const_().at(vexpr);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarTypeA_, typename VarTypeB_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(VarTypeA_ xi, VarTypeB_ xj) {
    if (xi.invalid() || xj.invalid()) throw std::logic_error("Invalid variables passed as indexes to DBM.");
    auto real_xi = major_ == dbm_major::row ? to_octdiff_identity(xi) : to_octdiff_identity(xj);
    auto real_xj = major_ == dbm_major::row ? to_octdiff_identity(xj) : to_octdiff_identity(xi);
    std::size_t var_size = this->as_subclass_().size();
    std::size_t index = real_xi.to_index() * var_size + real_xj.to_index();
    return this->as_subclass_().constant_(index);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(octdiff_vexpr<VarType_> vexpr) {
    return at( vexpr.xi(), vexpr.xj() );
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::at(oct_vexpr<VarType_> vexpr) {
    const oct_cons<VarType_, ValueType> cons(vexpr, ValueType());
    auto conj = cons.split();
    return at(conj.di());
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::operator[](octdiff_vexpr<VarType_> vexpr) const {
    return at(vexpr);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type
const& dense_dbm_base_<SubClass, ValueType, ValueLimits>::operator[](oct_vexpr<VarType_> vexpr) const {
    return at(vexpr);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::operator[](octdiff_vexpr<VarType_> vexpr) {
    return at(vexpr);
};

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline typename dense_dbm_base_<SubClass, ValueType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ValueType, ValueLimits>::operator[](oct_vexpr<VarType_> vexpr) {
    return at(vexpr);
};


//
// dense_dbm
//
template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm()
    : superclass_(traits_::default_major) {};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename ValueType_, typename ValueLimits_, typename>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm(
    octdiff_system<ValueType_, ValueLimits_> const& rhs,
    dbm_major major)
    : superclass_(major)
{
    resize(rhs.vars().last_var());
    for (auto cons : rhs) superclass_::at(cons) = cons.c();
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm(
    std::size_t max_vars,
    dbm_major major)
    : superclass_(major)
{
    resize(max_vars);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename VarType_, typename>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm(VarType_ last_var, dbm_major major)
    : dense_dbm(to_end_index_(last_var), major) {};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm(std::size_t max_vars, constant_type value, dbm_major major)
    : superclass_(major)
{
    resize(max_vars, value);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename VarType_, typename>
inline dense_dbm<ValueType, ValueLimits, Allocator>::dense_dbm(VarType_ last_var, constant_type value, dbm_major major)
    : dense_dbm(to_end_index_(last_var), value, major) {};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator> const dense_dbm<ValueType, ValueLimits, Allocator>::null() {
    return dense_dbm();
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator> dense_dbm<ValueType, ValueLimits, Allocator>::top(
    std::size_t max_vars,
    dbm_major major
) {
    return dense_dbm(max_vars, value_limits::top(), major);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename VarType_, typename>
inline dense_dbm<ValueType, ValueLimits, Allocator> dense_dbm<ValueType, ValueLimits, Allocator>::top(
    VarType_ last_var,
    dbm_major major
) {
    return top(to_end_index_(last_var), major);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline dense_dbm<ValueType, ValueLimits, Allocator> dense_dbm<ValueType, ValueLimits, Allocator>::bottom(
    std::size_t max_vars,
    dbm_major major
) {
    return dense_dbm(max_vars, value_limits::bottom(), major);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename VarType_, typename>
inline dense_dbm<ValueType, ValueLimits, Allocator> dense_dbm<ValueType, ValueLimits, Allocator>::bottom(
    VarType_ last_var,
    dbm_major major
) {
    return bottom(to_end_index_(last_var), major);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline std::size_t dense_dbm<ValueType, ValueLimits, Allocator>::size() const noexcept {
    return data_.size() >> 1;
};

template <typename ValueType, typename ValueLimits, typename Allocator>
typename dense_dbm<ValueType, ValueLimits, Allocator>::allocator_type
inline dense_dbm<ValueType, ValueLimits, Allocator>::get_allocator() const {
    return data_.get_allocator();
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline void dense_dbm<ValueType, ValueLimits, Allocator>::initialize(constant_type value) {
    data_.assign(data_.size(), value);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
template <typename VarType_, typename>
inline void dense_dbm<ValueType, ValueLimits, Allocator>::resize(VarType_ new_last_var, constant_type value) {
    data_.resize(to_end_index_(new_last_var), value);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline typename dense_dbm<ValueType, ValueLimits, Allocator>::constant_type&
dense_dbm<ValueType, ValueLimits, Allocator>::constant_(std::size_t index) {
    return data_.at(index);
};

template <typename ValueType, typename ValueLimits, typename Allocator>
inline typename dense_dbm<ValueType, ValueLimits, Allocator>::constant_type const&
dense_dbm<ValueType, ValueLimits, Allocator>::constant_(std::size_t index) const {
    return data_.at(index);
};

} // namespace oct
adl_END_ROOT_MODULE


#endif //adl__oct__dbm__dense_dbm__hpp__
