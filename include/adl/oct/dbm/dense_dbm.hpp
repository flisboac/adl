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
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::value_limits;
    using typename superclass_::identity_cons_type;

    using typename superclass_::no_index;

    dense_dbm_base_(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_(dense_dbm_base_ &&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ const&) noexcept = default;
    dense_dbm_base_& operator=(dense_dbm_base_ &&) noexcept = default;

protected:
    explicit dense_dbm_base_(dbm_major major) noexcept;

public:
    template <typename DbmType_, typename = dbm_t<DbmType_>>
        dense_dbm_base_(DbmType_ const& rhs);
    template <typename ValueType_,
            typename ValueLimits_,
            typename = std::enable_if_t<std::is_convertible<ValueType, ValueType_>::value> >
        dense_dbm_base_(octdiff_system<ValueType_, ValueLimits_> const& rhs);

    bool dense() const noexcept;
    bool autocoherent() const noexcept;
    bool empty() const noexcept;

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
    void resize(std::size_t new_size, constant_type value = value_limits::top());
    template <typename VarType_, typename = common_var_t<VarType_>>
        void resize(VarType_ new_last_var, constant_type value = value_limits::top());

private:
    constant_type& constant_(std::size_t index);
    constant_type const& constant_(std::size_t index) const;

private:
    container_type_ data_;
};

} // namespace oct
adl_END_ROOT_MODULE


template <typename CharType,
        typename CharTraits,
        typename DbmType,
        typename = adl::oct::dbm_t<DbmType>>
std::basic_ostream<CharType, CharTraits>& operator<<(
        std::basic_ostream<CharType, CharTraits>& os,
        adl::oct::dense_dbm_base_<DbmType, typename DbmType::value_type, typename DbmType::value_limits> const& dbm);


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {



} // namespace oct
adl_END_ROOT_MODULE


#endif //adl__oct__dbm__dense_dbm__hpp__
