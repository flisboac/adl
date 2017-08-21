// $flisboac 2017-08-13
/**
 * @file dense_dbm.hpp.hpp
 */
#ifndef adl__oct__dbm__dense_dbm__hpp__
#define adl__oct__dbm__dense_dbm__hpp__

#include <vector>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/dbm/dbm_base_.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {


template <typename Subclass, typename ValueType, typename ValueLimits>
class dense_dbm_base_ : public dbm_base_<Subclass, ValueType, ValueLimits> {
private:
    using superclass_ = dbm_base_<Subclass, ValueType, ValueLimits>;

protected:
    using traits_ = dbm_traits<Subclass>;

public:
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;

    dense_dbm_base_() noexcept = default;
    dense_dbm_base_(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_(dense_dbm_base_ &&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ &&) noexcept = default;

    dense_dbm_base_(dbm_major major) noexcept;

    //dbm_major major() const noexcept;
    identity_var_type last_var() const noexcept;
    bool dense() const noexcept;
    bool autocoherent() const noexcept;

    std::size_t first_xi(std::size_t xj = 0) const;
    std::size_t first_xj(std::size_t xi = 0) const;
    std::size_t end_xi(std::size_t xj = 0) const;
    std::size_t end_xj(std::size_t xi = 0) const;
    template <typename VarType_ = identity_var_type, typename = common_var_t<VarType_>>
        VarType_ first_xi(VarType_ xj = VarType_::invalid()) const;
    template <typename VarType_ = identity_var_type, typename = common_var_t<VarType_>>
        VarType_ first_xj(VarType_ xi = VarType_::invalid()) const;
    template <typename VarType_ = identity_var_type, typename = common_var_t<VarType_>>
        VarType_ end_xi(VarType_ xj = VarType_::invalid()) const;
    template <typename VarType_ = identity_var_type, typename = common_var_t<VarType_>>
        VarType_ end_xj(VarType_ xi = VarType_::invalid()) const;

    constant_type const& at(std::size_t i, std::size_t j) const;
    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        constant_type const& at(VarTypeA_ xi, VarTypeB_ xj) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type const& at(octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type const& at(oct_vexpr<VarType_> vexpr) const;

    constant_type& at(std::size_t i, std::size_t j);
    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        constant_type& at(VarTypeA_ xi, VarTypeB_ xj);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type& at(octdiff_vexpr<VarType_> vexpr);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type& at(oct_vexpr<VarType_> vexpr);

    constant_type& set(std::size_t i, std::size_t j, constant_type value);
    template <typename VarTypeA_, typename VarTypeB_, typename = std::enable_if<
        common_var<VarTypeA_>::valid && common_var<VarTypeB_>::valid>>
        constant_type& set(VarTypeA_ xi, VarTypeB_ xj, constant_type value);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type& set(octdiff_vexpr<VarType_> vexpr, constant_type value);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type& set(oct_vexpr<VarType_> vexpr, constant_type value);

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type const& operator[](octdiff_vexpr<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type const& operator[](oct_vexpr<VarType_> vexpr) const;

    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_octdiff_space>>
        constant_type& operator[](octdiff_vexpr<VarType_> vexpr);
    template <typename VarType_, typename = std::enable_if<common_var<VarType_>::is_oct_space>>
        constant_type& operator[](oct_vexpr<VarType_> vexpr);

protected:
    dbm_major const major_ = traits_::default_major;
};

template <typename ValueType, typename ValueLimits, typename Allocator>
class dense_dbm : public dense_dbm_base_<dense_dbm<ValueType, ValueLimits>, ValueType, ValueLimits> {
private:
    using superclass_ = dense_dbm_base_<dense_dbm<ValueType, ValueLimits>, ValueType, ValueLimits>;
    using container_type_ = std::vector<ValueType, Allocator>;

public:
    using typename superclass_::traits_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;

    using allocator_type = Allocator;

    dense_dbm() = default;
    dense_dbm(dense_dbm const&) = default;
    dense_dbm(dense_dbm &&) noexcept = default;
    dense_dbm& operator=(dense_dbm const&) = default;
    dense_dbm& operator=(dense_dbm &&) noexcept = default;

    dense_dbm(std::size_t max_vars, constant_type value, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        dense_dbm(VarType_ last_var, constant_type value, dbm_major major = traits_::default_major);

    static dense_dbm top(std::size_t max_vars, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        static dense_dbm top(VarType_ last_var, dbm_major major = traits_::default_major);
    static dense_dbm bottom(std::size_t max_vars, dbm_major major = traits_::default_major);
    template <typename VarType_, typename = common_var_t<VarType_>>
        static dense_dbm bottom(VarType_ last_var, dbm_major major = traits_::default_major);

    std::size_t var_count() const noexcept;
    allocator_type get_allocator() const;

    void initialize(constant_type value);
    void resize(std::size_t max_vars);
    void resize(std::size_t max_vars, constant_type value);
    template <typename VarType_, typename = common_var_t<VarType_>>
        void resize(VarType_ last_var);
    template <typename VarType_, typename = common_var_t<VarType_>>
        void resize(VarType_ last_var, constant_type value);

private:
    constant_type& value_(std::size_t index);
    constant_type const& value_(std::size_t index) const;
    constant_type& value_(std::size_t index, constant_type value);

private:
    container_type_ data_;
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__dense_dbm__hpp__
