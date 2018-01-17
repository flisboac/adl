// $flisboac 2017-08-13
/**
 * @file dense_dbm.hpp.hpp
 */
#ifndef adl__oct__dbm__dense_dbm_base___hpp__
#define adl__oct__dbm__dense_dbm_base___hpp__

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

#if adl_CONFIG_COMPILER_IS_GCC && defined(major)
    // Stupid GCC/G++/glibc/libstdc++/whatever (who is to blame, again?)
    // FIXME Wait for this bug fix: https://sourceware.org/bugzilla/show_bug.cgi?id=19239
    #undef major
#endif

adl_BEGIN_ROOT_MODULE
namespace oct {


template <typename Subclass, typename ConstantType, typename ValueLimits>
class dense_dbm_base_ : public dbm_base_<Subclass, ConstantType, ValueLimits> {
private:
    using superclass_ = dbm_base_<Subclass, ConstantType, ValueLimits>;

protected:
    using traits_ = dbm_traits<Subclass>;

public:
    friend class crtp_base<Subclass>;

    using typename superclass_::subclass_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::constant_limits;
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
    dbm_major major() const noexcept;

    octdiff_cons<constant_type> get(oct_var xi) const;
    octdiff_cons<constant_type> get(octdiff_var xi, octdiff_var xj) const;
    octdiff_cons<constant_type> get(oct_vexpr vexpr) const;
    octdiff_cons<constant_type> get(octdiff_vexpr vexpr) const;

    constant_type& at(oct_var xi);
    constant_type& at(octdiff_var xi, octdiff_var xj);
    constant_type& at(oct_vexpr vexpr);
    constant_type& at(octdiff_vexpr vexpr);

    constant_type const& at(oct_var xi) const;
    constant_type const& at(octdiff_var xi, octdiff_var xj) const;
    constant_type const& at(oct_vexpr vexpr) const;
    constant_type const& at(octdiff_vexpr vexpr) const;

    bool is_top(oct_var xi) const { return is_top(this->at(xi)); }
    bool is_top(octdiff_var xi, octdiff_var xj) const { return is_top(this->at(xi, xj)); }
    bool is_top(oct_vexpr vexpr) const { return is_top(this->at(vexpr)); }
    bool is_top(octdiff_vexpr vexpr) const { return is_top(this->at(vexpr)); }

    constant_type const& operator[](octdiff_vexpr vexpr) const;
    constant_type const& operator[](oct_vexpr vexpr) const;
    constant_type const& operator[](oct_var var) const;
    constant_type const& operator[](std::size_t index) const;

    constant_type& operator[](octdiff_vexpr vexpr);
    constant_type& operator[](oct_vexpr vexpr);
    constant_type& operator[](oct_var var);
    constant_type& operator[](std::size_t index);

    constexpr static constant_type top() { return constant_limits::top(); }
    constexpr static bool is_top(constant_type c) { return constant_limits::is_top(c); }

protected:
    dbm_major const major_;
};

} // namespace oct
adl_END_ROOT_MODULE


template <typename CharType,
        typename CharTraits,
        typename DbmType,
        typename = adl::oct::is_valid_dbm_t<DbmType>>
std::basic_ostream<CharType, CharTraits>& operator<<(
        std::basic_ostream<CharType, CharTraits>& os,
        adl::oct::dense_dbm_base_<DbmType, typename DbmType::value_type, typename DbmType::constant_limits> const& dbm);


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// dense_dbm_base_
//
template <typename SubClass, typename ConstantType, typename ValueLimits>
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::dense_dbm_base_(dbm_major major) noexcept
    : major_(major) {};

template <typename SubClass, typename ConstantType, typename ValueLimits>
bool dense_dbm_base_<SubClass, ConstantType, ValueLimits>::dense() const noexcept {
    return true;
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
bool dense_dbm_base_<SubClass, ConstantType, ValueLimits>::autocoherent() const noexcept {
    return false;
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
dbm_major dense_dbm_base_<SubClass, ConstantType, ValueLimits>::major() const noexcept {
    return major_;
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline octdiff_cons<typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type>
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::get(oct_var xi) const {
    return get(oct_vexpr::make_unit(xi));
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline octdiff_cons<typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type>
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::get(octdiff_var xi, octdiff_var xj) const {
    using vexpr_type_ = typename octdiff_cons<constant_type>::vexpr_type;
    auto constant = at(xi, xj);
    if (!constant_limits::is_null(constant)) return octdiff_cons<constant_type>(vexpr_type_::make_sub(xi, xj), constant);
    return identity_cons_type::invalid();
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline octdiff_cons<typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type>
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::get(oct_vexpr vexpr) const {
    const oct_cons<constant_type> cons(vexpr, constant_type());
    auto conj = cons.split();
    auto constant_di = at(conj.di());
    auto constant_dj = at(conj.dj());
    if (constant_di != constant_dj || constant_limits::is_null(constant_di)) return identity_cons_type::invalid();
    return octdiff_cons<constant_type>(conj.di().to_vexpr(), constant_di);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline octdiff_cons<typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type>
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::get(octdiff_vexpr vexpr) const {
    return get(vexpr.xi(), vexpr.xj());
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(oct_var xi) {
    return const_cast<constant_type&>(const_cast<dense_dbm_base_ const&>(*this).at(xi));
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(octdiff_var xi, octdiff_var xj) {
    return const_cast<constant_type&>(const_cast<dense_dbm_base_ const&>(*this).at(xi, xj));
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(oct_vexpr vexpr) {
    return const_cast<constant_type&>(const_cast<dense_dbm_base_ const&>(*this).at(vexpr));
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(octdiff_vexpr vexpr) {
    return const_cast<constant_type&>(const_cast<dense_dbm_base_ const&>(*this).at(vexpr));
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(oct_var xi) const {
    using cons_type_ = oct_cons<constant_type>;
    using vexpr_type_ = typename cons_type_::vexpr_type;
    auto cons = cons_type_::make_upper_limit(vexpr_type_::make_unit(xi), constant_type());
    auto diff_vexpr = cons.split().di().to_vexpr();
    return at(diff_vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(octdiff_var xi, octdiff_var xj) const {
    if (!xi.valid() || !xj.valid()) throw std::logic_error("Invalid variables passed as indexes to DBM.");
    auto real_xi = major_ == dbm_major::row ? xi : xj;
    auto real_xj = major_ == dbm_major::row ? xj : xi;
    std::size_t var_size = this->as_subclass_().size();
    std::size_t index = real_xi.to_index() * var_size + real_xj.to_index();
    return this->as_subclass_().constant_(index);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(oct_vexpr vexpr) const {
    using cons_type_ = oct_cons<constant_type>;
    using vexpr_type_ = typename cons_type_::vexpr_type;
    auto cons = cons_type_::make_upper_limit(vexpr, constant_type());
    auto diff_vexpr = cons.split().di().to_vexpr();
    return at(diff_vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::at(octdiff_vexpr vexpr) const {
    return at(vexpr.xi(), vexpr.xj());
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](octdiff_vexpr vexpr) const {
    return const_cast<dense_dbm_base_&>(*this).operator[](vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](oct_vexpr vexpr) const {
    return const_cast<dense_dbm_base_&>(*this).operator[](vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](oct_var var) const {
    return const_cast<dense_dbm_base_&>(*this).operator[](var);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type const&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](std::size_t index) const {
    return this->as_subclass_().constant_(index);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](octdiff_vexpr vexpr) {
    return at(vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
inline typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](oct_vexpr vexpr) {
    return at(vexpr);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](oct_var var) {
    return at(var);
};

template <typename SubClass, typename ConstantType, typename ValueLimits>
typename dense_dbm_base_<SubClass, ConstantType, ValueLimits>::constant_type&
dense_dbm_base_<SubClass, ConstantType, ValueLimits>::operator[](std::size_t index) {
    return this->as_subclass_().constant_(index);
};

} // namespace oct
adl_END_ROOT_MODULE


#endif //adl__oct__dbm__dense_dbm_base___hpp__
