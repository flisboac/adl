// $flisboac 2017-07-10
/**
 * @file var_set_.hpp
 */
#ifndef adl__oct__system__var_set___hpp__
#define adl__oct__system__var_set___hpp__


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
        var_set& erase_one(VarType var);

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


} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__system__var_set___hpp__
