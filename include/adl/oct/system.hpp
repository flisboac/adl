// $flisboac 2017-06-26
/**
 * @file system.hpp
 */
#ifndef adl__oct__system__hpp__
#define adl__oct__system__hpp__

#include <map>
#include <set>
#include <iterator>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <domain_space Domain>
class var_set {
public:
    //
    // Types
    //
    using domain_space_traits = adl::oct::domain_space_traits<Domain>;
    using identity_var_type = typename domain_space_traits::identity_var_type;
    using literal_var_type = typename domain_space_traits::literal_var_type;
    using counterpart_identity_var_type = typename domain_space_traits::counterpart_identity_var_type;
    using counterpart_literal_var_type = typename domain_space_traits::counterpart_literal_var_type;

    using var_type = literal_var_type;

    class value_type {
    public:
        value_type() = default;
        value_type(value_type const&) = default;
        value_type(value_type &&) noexcept = default;
        value_type& operator=(value_type const&) = default;
        value_type& operator=(value_type &&) noexcept = default;

        std::string const& name() const noexcept;
        std::string& name() noexcept;
        value_type& name(std::string value);

        std::size_t const& positive_count() const noexcept;
        std::size_t& positive_count() noexcept;
        value_type& positive_count(std::size_t value) noexcept;

        std::size_t const& negative_count() const noexcept;
        std::size_t& negative_count() noexcept;
        value_type& negative_count(std::size_t value) noexcept;

        var_type var() const noexcept;

        operator var_type() const noexcept;

    private:
        identity_var_type normalized_var_;
        std::string name_;
        std::size_t positive_count_;
        std::size_t negative_count_;
    };

private:
    using container_type_ = std::multimap<identity_var_type, value_type>; // normalized (positive-only)

public:
    using const_iterator = typename container_type_::const_iterator;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator rbegin() const;
    const_iterator rend() const;

    bool empty() const;
    std::size_t size() const;

    var_set& clear();
    template <typename VarType, typename = common_var_t<VarType>>
        const_iterator insert(VarType var);
    template <typename VarType, typename = common_var_t<VarType>>
        const_iterator erase_one(VarType var);

    template <typename VarType, typename = common_var_t<VarType>>
        std::size_t count(VarType var) const; // unnormalized occurences
    template <typename VarType, typename = common_var_t<VarType>>
        std::size_t count_all(VarType var) const; // considers all occurrences, positive + negative
    template <typename VarType, typename = common_var_t<VarType>>
        const_iterator find(VarType var) const;
    template <typename VarType, typename = common_var_t<VarType>>
        value_type at(VarType var) const noexcept(false); // throws if the var is not included
    template <typename VarType, typename = common_var_t<VarType>>
        value_type operator[](VarType var) const; // returns invalid if the var is not included

private:
    container_type_ data_;
};

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

private:
    using container_type_ = std::set<identity_cons_type>;
    using container_const_iterator_ = typename container_type_::const_iterator;

public:
    using value_type = literal_cons_type;
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

    system_base_& clear();
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        const_iterator insert(common_cons_t<CT, VT> cons);
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        const_iterator erase(common_cons_t<CT, VT> cons);
    template <typename CT, typename VT, typename = std::enable_if<common_vexpr<CT, VT>::space == space>>
        const_iterator erase(common_vexpr_t<CT, VT> vexpr);
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        std::size_t count(common_cons_t<CT, VT> cons) const;
    template <typename CT, typename VT, typename = std::enable_if<common_vexpr<CT, VT>::space == space>>
        std::size_t count(common_vexpr_t<CT, VT> vexpr) const;
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        const_iterator find(common_cons_t<CT, VT> cons) const;
    template <typename CT, typename VT, typename = std::enable_if<common_vexpr<CT, VT>::space == space>>
        const_iterator find(common_vexpr_t<CT, VT> vexpr) const;
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        value_type at(common_cons_t<CT, VT> cons) const noexcept(false);
    template <typename CT, typename VT, typename = std::enable_if<common_vexpr<CT, VT>::space == space>>
        value_type at(common_vexpr_t<CT, VT> vexpr) const noexcept(false);
    template <typename CT, typename VT, typename = std::enable_if<common_cons<CT, VT>::space == space>>
        value_type operator[](common_cons_t<CT, VT> cons) const;
    template <typename CT, typename VT, typename = std::enable_if<common_vexpr<CT, VT>::space == space>>
        value_type operator[](common_vexpr_t<CT, VT> vexpr) const;

private:
    container_type_ constraints_;
    var_set_type variables_;
};

template <typename ValueType, typename ValueLimits>
class oct_system : public system_base_<domain_space::oct, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::oct, ValueType, ValueLimits>;

public:

};

template <typename ValueType, typename ValueLimits>
class octdiff_system : public system_base_<domain_space::octdiff, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::octdiff, ValueType, ValueLimits>;

public:

};

} // namespace oct

adl_END_ROOT_MODULE


#endif // adl__oct__system__hpp__
