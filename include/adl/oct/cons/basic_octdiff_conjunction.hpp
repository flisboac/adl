// $flavio.lisboa @ 2017-09-04.
//
/*
 * @file basic_octdiff_conjunction.hpp
 */


#ifndef adl__oct__cons__basic_octdiff_conjunction__hpp__
#define adl__oct__cons__basic_octdiff_conjunction__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>
#include <stdexcept>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ConstantType, typename VarType>
class basic_octdiff_conjunction {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using value_type = ConstantType;
    using var_traits = typename var_type::var_traits;
    using counterpart_var_type = typename var_traits::counterpart_var_type;
    using identity_var_type = typename var_traits::identity_var_type;
    using var_id_type = typename var_traits::var_id_type;

    // Vexpr types
    using vexpr_type = typename var_traits::vexpr_type;
    using counterpart_vexpr_type = typename var_traits::counterpart_vexpr_type;
    using identity_vexpr_type = typename var_traits::identity_vexpr_type;

    // Cons types
    using cons_type = basic_octdiff_cons<value_type, var_type>;
    using counterpart_cons_type = basic_oct_cons<value_type, counterpart_var_type>;
    using identity_cons_type = basic_oct_cons<value_type, identity_var_type>;

    struct less {
        constexpr bool operator()(basic_octdiff_conjunction const& lhs, basic_octdiff_conjunction const& rhs) const noexcept;
    };

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    static_assert(space == domain_space::octdiff, "Wrong domain space.");

    //
    // CONSTRUCTION
    //

    //!@defgroup Defaulted/deleted Constructors, destructor and assignment operators
    //!@{
    constexpr basic_octdiff_conjunction() = default;
    constexpr basic_octdiff_conjunction(basic_octdiff_conjunction const&) = default;
    constexpr basic_octdiff_conjunction(basic_octdiff_conjunction &&) noexcept = default;
    basic_octdiff_conjunction& operator=(basic_octdiff_conjunction const&) = default;
    basic_octdiff_conjunction& operator=(basic_octdiff_conjunction &&) noexcept = default;
    //!@}

    //!@defgroup Instance construction
    //!@{
    constexpr basic_octdiff_conjunction(cons_type di, cons_type dj) noexcept;
    template <
            typename ConstantType_,
            typename VarType_,
            typename = std::enable_if_t<
                common_var<VarType_>::is_oct_space
                && !std::is_same<VarType_, var_type>::value
                && std::is_convertible<ConstantType_, value_type>::value>>
    constexpr basic_octdiff_conjunction(basic_oct_cons<ConstantType_, VarType_> cons) noexcept;

    // static construction functions
    constexpr static basic_octdiff_conjunction invalid() noexcept;
    constexpr static basic_octdiff_conjunction from_cons(cons_type di) noexcept;
    constexpr static basic_octdiff_conjunction from_cons(cons_type di, cons_type dj) noexcept;
    //!@}

    //
    // PROPERTIES
    //

    constexpr cons_type di() const noexcept;
    constexpr cons_type first_cons() const noexcept;
    constexpr cons_type dj() const noexcept;
    constexpr cons_type second_cons() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t end_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(basic_octdiff_conjunction const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(basic_octdiff_conjunction const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr basic_octdiff_conjunction& invalidate() noexcept;
    template <typename CharTraits = std::char_traits<char>> void print(std::basic_ostream<char, CharTraits>& os) const;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr basic_octdiff_conjunction& ensure_valid();
    constexpr basic_octdiff_conjunction const& ensure_valid() const;
    constexpr basic_octdiff_conjunction& as_valid() noexcept;
    constexpr basic_octdiff_conjunction to_valid() const noexcept;
    std::string to_string() const;
    constexpr counterpart_cons_type to_oct() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;

private:
    constexpr static cons_type init_di_(cons_type di, cons_type dj) noexcept;
    constexpr static cons_type init_dj_(cons_type di, cons_type dj) noexcept;

private:
    cons_type di_;
    cons_type dj_;
};

} // namespace oct

namespace operators {
    inline namespace oct {
        inline namespace cons {
            inline namespace comparison {
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) < 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<=(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) < 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator==(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return lhs.equals(rhs); }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator!=(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return !lhs.equals(rhs); }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>=(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) >= 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>(adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) > 0; }
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename ConstantType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::basic_octdiff_conjunction<ConstantType, VarType> const& cons
) {
    cons.print(os);
    return os;
};

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
#include "adl/oct/cons/basic_oct_cons.hpp"
#include "adl/oct/cons/basic_octdiff_cons.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

//
// adl::oct::basic_octdiff_conjunction
//

template <typename ConstantType, typename VarType>
constexpr bool basic_octdiff_conjunction<ConstantType, VarType>::less::operator()(
        basic_octdiff_conjunction const& lhs,
        basic_octdiff_conjunction const& rhs
) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>::basic_octdiff_conjunction(cons_type di, cons_type dj) noexcept :
        di_(init_di_(di, dj)), dj_(init_dj_(di, dj)) {};

template <typename ConstantType, typename VarType>
template <typename ConstantType_, typename VarType_, typename>
constexpr basic_octdiff_conjunction<ConstantType, VarType>::basic_octdiff_conjunction(
    basic_oct_cons<ConstantType_, VarType_> cons
) noexcept : basic_octdiff_conjunction(cons.split()) {};

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>
basic_octdiff_conjunction<ConstantType, VarType>::invalid() noexcept {
    return basic_octdiff_conjunction();
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>
basic_octdiff_conjunction<ConstantType, VarType>::from_cons(cons_type di) noexcept {
    return basic_octdiff_conjunction(di, cons_type::invalid()).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>
basic_octdiff_conjunction<ConstantType, VarType>::from_cons(cons_type di, cons_type dj) noexcept {
    return basic_octdiff_conjunction(di, dj).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::di() const noexcept {
    return di_;
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::first_cons() const noexcept {
    return di_;
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::dj() const noexcept {
    return dj_;
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::second_cons() const noexcept {
    return dj_;
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::var_type
basic_octdiff_conjunction<ConstantType, VarType>::last_var() const noexcept {
    auto di_last = di_.last_var();
    auto dj_last = dj_.last_var();
    return di_last >= dj_last ? di_last : dj_last;
}

template <typename ConstantType, typename VarType>
constexpr std::size_t basic_octdiff_conjunction<ConstantType, VarType>::end_var_index() const noexcept {
    auto var = last_var();
    return var.valid() ? last_var().increment().to_index() : 0;
}

template <typename ConstantType, typename VarType>
constexpr bool basic_octdiff_conjunction<ConstantType, VarType>::valid() const noexcept {
    return di_.valid() && (
            di_.unit()
            ? dj_.equals(cons_type::invalid())
            : di_.to_commuted().equals(dj_)
    );
}

template <typename ConstantType, typename VarType>
constexpr bool basic_octdiff_conjunction<ConstantType, VarType>::unit() const noexcept {
    return di_.valid() && di_.unit();
}

template <typename ConstantType, typename VarType>
constexpr bool basic_octdiff_conjunction<ConstantType, VarType>::equals(basic_octdiff_conjunction const& rhs) const noexcept {
    return valid() && rhs.valid() && (
            di_.equals(rhs.di_)
            ? dj_.equals(rhs.dj_)
            : di_.equals(rhs.dj_)
              ? dj_.equals(rhs.di_)
              : false
    );
}

template <typename ConstantType, typename VarType>
constexpr int basic_octdiff_conjunction<ConstantType, VarType>::compare(basic_octdiff_conjunction const& rhs) const noexcept {
    if (!valid() || !rhs.valid()) return 0;
    int di_cmp = di_.compare(rhs.di_);
    return di_cmp != 0 ? di_cmp : dj_.compare(rhs.dj_);
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>&
basic_octdiff_conjunction<ConstantType, VarType>::invalidate() noexcept {
    di_.invalidate();
    dj_.invalidate();
    return *this;
}

template <typename ConstantType, typename VarType>
template <typename CharTraits>
inline void basic_octdiff_conjunction<ConstantType, VarType>::print(std::basic_ostream<char, CharTraits>& os) const {
    os << this->to_string();
};

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>&
basic_octdiff_conjunction<ConstantType, VarType>::ensure_valid() {
    return valid() ? *this : throw std::logic_error("Invalid octdiff-constraint conjunction");
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType> const&
basic_octdiff_conjunction<ConstantType, VarType>::ensure_valid() const {
    return valid() ? *this : throw std::logic_error("Invalid octdiff-constraint conjunction");
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>&
basic_octdiff_conjunction<ConstantType, VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return *this;
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>
basic_octdiff_conjunction<ConstantType, VarType>::to_valid() const noexcept {
    return basic_octdiff_conjunction<ConstantType, VarType>(*this).as_valid();
}

template <typename ConstantType, typename VarType>
inline std::string basic_octdiff_conjunction<ConstantType, VarType>::to_string() const {
    return std::string("(") + (unit() ? di_.to_string() : di_.to_string() + " && " + dj_.to_string()) + ")";
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::counterpart_cons_type
basic_octdiff_conjunction<ConstantType, VarType>::to_oct() const noexcept {
    return this->valid()
           ? unit()
             ? counterpart_cons_type(counterpart_vexpr_type::make_unit(di().xi().to_counterpart()), di().c() / 2)
             : counterpart_cons_type(counterpart_vexpr_type(di().xi().to_counterpart(), di().xj().to_counterpart()), di().c())
           : counterpart_cons_type::invalid();
}

template <typename ConstantType, typename VarType>
constexpr bool basic_octdiff_conjunction<ConstantType, VarType>::operator!() const noexcept {
    return !valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>::operator bool() const noexcept {
    return valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_conjunction<ConstantType, VarType>::operator std::string() const {
    return to_string();
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::init_di_(cons_type di, cons_type dj) noexcept {
    if (!di.valid()
        || !dj.valid()
        || di.compare(dj) <= 0)
        return di;
    return dj;
}

template <typename ConstantType, typename VarType>
constexpr typename basic_octdiff_conjunction<ConstantType, VarType>::cons_type
basic_octdiff_conjunction<ConstantType, VarType>::init_dj_(cons_type di, cons_type dj) noexcept {
    if (di.equals(dj)) return cons_type::invalid(); // for unit oct-constraints of type xi <= c or xi >= c
    if (!di.valid()
        || !dj.valid()
        || dj.compare(di) >= 0)
        return dj;
    return di;
}

} // namespace oct
adl_END_ROOT_MODULE

#endif // adl__oct__cons__basic_octdiff_conjunction__hpp__
