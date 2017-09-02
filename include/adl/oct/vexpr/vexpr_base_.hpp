// $flavio.lisboa @ 2017-09-01.
//
/*
 * @file vexpr_base_.hpp
 */
#ifndef adl__oct__vexpr__vexpr_base___hpp__
#define adl__oct__vexpr__vexpr_base___hpp__

#include <type_traits>
#include <string>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

enum class vexpr_oper {
    unit,
    add,
    sub
};

template <typename VarType>
class vexpr_base_ {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using var_traits = typename var_type::var_traits;
    using counterpart_var_type = typename var_traits::counterpart_var_type;
    using identity_var_type = typename var_traits::identity_var_type;
    using var_id_type = typename var_traits::var_id_type;

    // Vexpr types
    using vexpr_type = typename var_traits::vexpr_type;
    using counterpart_vexpr_type = typename var_traits::counterpart_vexpr_type;
    using identity_vexpr_type = typename var_traits::identity_vexpr_type;

    struct less {
        constexpr bool operator()(vexpr_type const& lhs, vexpr_type const& rhs) const noexcept;
    };

    struct hash {
        constexpr std::size_t operator()(vexpr_type const& lhs) const noexcept;
    };

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_traits::space;
    constexpr static const domain_space counterpart_space = var_traits::counterpart_space;
    constexpr static const bool has_unit_vexpr = space == domain_space::oct;
    constexpr static const bool has_add_vexpr = space == domain_space::oct;

private:
    using subclass_ = vexpr_type;

public:
    //
    // CONSTRUCTION
    //

    ///@name Defaulted or deleted members
    ///@{
    constexpr vexpr_base_() = default;
    constexpr vexpr_base_(vexpr_base_ const&) = default;
    constexpr vexpr_base_(vexpr_base_&&) noexcept = default;
    vexpr_base_& operator=(vexpr_base_ const&) = default;
    vexpr_base_& operator=(vexpr_base_&&) noexcept = default;
    ///@}

    ///@name Instance construction
    ///@{
    constexpr vexpr_base_(var_type xi, var_type xj) noexcept;

    constexpr static vexpr_type invalid() noexcept;
    constexpr static vexpr_type make_sub(var_type xi, var_type xj) noexcept;
    ///@}

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t end_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept; // Refers to a single octagonal var. If true, an eventual constraint will have the format +-xi <= c (or its difference format)
    constexpr bool duplicated_var() const noexcept; // xi.valid() && xi.equals(xj); must be always false for octdiff space
    constexpr vexpr_oper operation() const noexcept; // Always vexpr_oper::sub for octdiff space

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(vexpr_type const& rhs) const noexcept;
    constexpr int compare(vexpr_type const& rhs) const noexcept;

    // Operations
    constexpr vexpr_type& invalidate() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr vexpr_type& ensure_valid();
    constexpr vexpr_type const& ensure_valid() const;
    constexpr vexpr_type& as_valid() noexcept;
    constexpr vexpr_type to_valid() const noexcept;
    std::string to_string() const;
    constexpr identity_vexpr_type to_identity() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

protected:
    var_type xi_;
    var_type xj_;
};

template <typename FirstVarType, typename SecondVarType, bool Specialized = common_var<FirstVarType, SecondVarType>::valid>
struct common_vexpr_ {
    constexpr static const bool valid = Specialized;
};

template <typename FirstVarType, typename SecondVarType>
struct common_vexpr_<FirstVarType, SecondVarType, true> {
    constexpr static const bool valid = true;

private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    using type = typename var_traits::vexpr_type;
    constexpr static const domain_space space = common_var<FirstVarType, SecondVarType>::space;
    constexpr static const domain_space counterpart_space = common_var<FirstVarType, SecondVarType>::counterpart_space;
    constexpr static const bool is_oct_space = common_var<FirstVarType, SecondVarType>::is_oct_space;
    constexpr static const bool is_octdiff_space = common_var<FirstVarType, SecondVarType>::is_octdiff_space;
};


template <typename FirstVarType, typename SecondVarType>
struct common_vexpr : public common_vexpr_<FirstVarType, SecondVarType> {};

} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__vexpr__vexpr_base___hpp__
