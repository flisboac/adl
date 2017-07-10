// $flisboac 2017-07-10
/**
 * @file system_base_.hpp
 */
#ifndef adl__oct__system__system_base___hpp__
#define adl__oct__system__system_base___hpp__

#include <map>
#include <set>
#include <iterator>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/system/var_set_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {


template <domain_space Domain, typename ValueType, typename ValueLimits>
class system_base_ {
    //
    // Types
    //
    using domain_space_traits = adl::oct::domain_space_traits<Domain>;

private:
    using subclass_ = typename domain_space_traits::template system_type<ValueType, ValueLimits>;

public:
    using identity_var_type = typename domain_space_traits::identity_var_type;
    using literal_var_type = typename domain_space_traits::literal_var_type;
    using counterpart_identity_var_type = typename domain_space_traits::counterpart_identity_var_type;
    using counterpart_literal_var_type = typename domain_space_traits::counterpart_literal_var_type;

    using identity_vexpr_type = typename domain_space_traits::identity_vexpr_type;
    using literal_vexpr_type = typename domain_space_traits::literal_vexpr_type;
    using counterpart_identity_vexpr_type = typename domain_space_traits::counterpart_identity_vexpr_type;
    using counterpart_literal_vexpr_type = typename domain_space_traits::counterpart_literal_vexpr_type;

    using identity_cons_type = typename domain_space_traits::template identity_cons_type<ValueType>;
    using literal_cons_type = typename domain_space_traits::template literal_cons_type<ValueType>;
    using counterpart_identity_cons_type = typename domain_space_traits::template counterpart_identity_cons_type<ValueType>;
    using counterpart_literal_cons_type = typename domain_space_traits::template counterpart_literal_cons_type<ValueType>;
    using identity_octdiff_conjunction_type = typename domain_space_traits::template identity_octdiff_conjunction_type <ValueType>;
    using literal_octdiff_conjunction_type = typename domain_space_traits::template literal_octdiff_conjunction_type<ValueType>;

    using system_type = typename domain_space_traits::template system_type<ValueType, ValueLimits>;
    using counterpart_system_type = typename domain_space_traits::template counterpart_system_type<ValueType, ValueLimits>;

    // Constexpr static values
    constexpr static const auto space = domain_space_traits::space;
    constexpr static const auto counterpart_space = domain_space_traits::counterpart_space;

protected:
    using container_value_type_ = std::conditional<space == domain_space::oct, identity_cons_type, identity_octdiff_conjunction_type>::type;
    using container_type_ = std::set<container_value_type_>;
    using container_const_iterator_ = typename container_type_::const_iterator;

public:
    using value_type = std::conditional<space == domain_space::oct, literal_cons_type, literal_octdiff_conjunction_type>::type;
    using var_set_type = var_set<space>;

    class const_iterator {

        // Type definitions
        using difference_type = typename container_const_iterator_::difference_type;
        using value_type = value_type;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::random_access_iterator_tag;

        // Defaulted members
        const_iterator(const_iterator const& rhs) = default;
        const_iterator(const_iterator && rhs) noexcept = default;
        const_iterator& operator=(const_iterator const& rhs) = default;
        const_iterator& operator=(const_iterator && rhs) noexcept = default;

        // Instance construction
        const_iterator(system_base_ const& parent, container_const_iterator_ iter) noexcept;

        bool operator==(const_iterator rhs) noexcept;
        bool operator!=(const_iterator rhs) noexcept;

        // Iterator members
        value_type const& operator*();
        value_type const* operator->();
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

    private:
        system_base_ parent_;
        container_const_iterator_ iter_;
        value_type current_;
    };

    system_base_() noexcept = default;
    system_base_(system_base_ const&) = default;
    system_base_(system_base_ &&) noexcept = default;
    system_base_& operator=(system_base_ const&) = default;
    system_base_& operator=(system_base_ &&) noexcept = default;

    var_set_type const& vars() const;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator rbegin() const;
    const_iterator rend() const;

    bool empty() const;
    std::size_t size() const;

    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        std::size_t count(common_vexpr_t<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        const_iterator find(common_vexpr_t<VarType_> vexpr) const;
    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        value_type at(common_vexpr_t<VarType_> vexpr) const noexcept(false);
    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        value_type operator[](common_vexpr_t<VarType_> vexpr) const;

protected:
    system_base_& clear_();
    template <typename ValueType_, typename VarType_, typename = std::enable_if<
        std::is_convertible<ValueType_, ValueType>::value
        && common_cons<ValueType_, VarType_>::space == space>>
        const_iterator insert_(common_cons_t<ValueType_, VarType_> cons);
    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        const_iterator erase_(common_vexpr_t<VarType_> vexpr);

protected:
    container_type_ constraints_;
    var_set_type variables_;
};

} // namespace oct

adl_END_ROOT_MODULE

#endif //adl__oct__system__system_base___hpp__
