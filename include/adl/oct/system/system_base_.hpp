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
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

#include "adl/oct/system/var_set_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {


template <domain_space Domain, typename ConstantType, typename ValueLimits>
class system_base_ {
    //
    // Types
    //
    using domain_space_traits = adl::oct::domain_space_traits<Domain>;

private:
    using subclass_ = typename domain_space_traits::template system_type<ConstantType, ValueLimits>;

public:
    using constant_type = ConstantType;

    using identity_var_type = typename domain_space_traits::identity_var_type;
    using literal_var_type = typename domain_space_traits::literal_var_type;
    using counterpart_identity_var_type = typename domain_space_traits::counterpart_identity_var_type;
    using counterpart_literal_var_type = typename domain_space_traits::counterpart_literal_var_type;

    using identity_vexpr_type = typename domain_space_traits::identity_vexpr_type;
    using literal_vexpr_type = typename domain_space_traits::literal_vexpr_type;
    using counterpart_identity_vexpr_type = typename domain_space_traits::counterpart_identity_vexpr_type;
    using counterpart_literal_vexpr_type = typename domain_space_traits::counterpart_literal_vexpr_type;

    using identity_cons_type = typename domain_space_traits::template identity_cons_type<ConstantType>;
    using literal_cons_type = typename domain_space_traits::template literal_cons_type<ConstantType>;
    using counterpart_identity_cons_type = typename domain_space_traits::template counterpart_identity_cons_type<ConstantType>;
    using counterpart_literal_cons_type = typename domain_space_traits::template counterpart_literal_cons_type<ConstantType>;
    using identity_octdiff_conjunction_type = typename domain_space_traits::template identity_octdiff_conjunction_type <ConstantType>;
    using literal_octdiff_conjunction_type = typename domain_space_traits::template literal_octdiff_conjunction_type<ConstantType>;

    using system_type = typename domain_space_traits::template system_type<ConstantType, ValueLimits>;
    using counterpart_system_type = typename domain_space_traits::template counterpart_system_type<ConstantType, ValueLimits>;

    // Constexpr static values
    constexpr static const auto space = domain_space_traits::space;
    constexpr static const auto counterpart_space = domain_space_traits::counterpart_space;

protected:
    using container_constant_type_ = literal_cons_type; //typename std::conditional<space == domain_space::oct, literal_cons_type, literal_octdiff_conjunction_type>::type;
    using container_type_ = std::set<container_constant_type_, typename container_constant_type_::less>;
    using container_const_iterator_ = typename container_type_::const_iterator;
    using container_iterator_ = typename container_type_::iterator;
    using container_reverse_iterator_ = typename container_type_::reverse_iterator;
    using container_const_reverse_iterator_ = typename container_type_::const_reverse_iterator;

public:
    using var_set_type = var_set<space>;
    using value_type = container_constant_type_;
    using key_type = value_type;
    using const_iterator = container_const_iterator_;
    using iterator = container_iterator_;
    using reverse_iterator = container_reverse_iterator_;
    using const_reverse_iterator = container_const_reverse_iterator_;

    system_base_() = default;
    system_base_(system_base_ const&) = default;
    system_base_(system_base_ &&) noexcept = default;
    system_base_& operator=(system_base_ const&) = default;
    system_base_& operator=(system_base_ &&) noexcept = default;

    var_set_type const& vars() const;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    bool empty() const;
    std::size_t size() const;

    std::string to_string() const;

protected:
    container_type_ constraints_;
    var_set_type variables_;
};

} // namespace oct

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//

//
// system_base_
//

adl_BEGIN_ROOT_MODULE
namespace oct {

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL typename system_base_<Domain, ConstantType, ValueLimits>::var_set_type const&
system_base_<Domain, ConstantType, ValueLimits>::vars() const {
    return variables_;
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL typename system_base_<Domain, ConstantType, ValueLimits>::const_iterator
system_base_<Domain, ConstantType, ValueLimits>::begin() const {
    return constraints_.begin();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL typename system_base_<Domain, ConstantType, ValueLimits>::const_iterator
system_base_<Domain, ConstantType, ValueLimits>::end() const {
    return constraints_.end();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL typename system_base_<Domain, ConstantType, ValueLimits>::const_reverse_iterator
system_base_<Domain, ConstantType, ValueLimits>::rbegin() const {
    return constraints_.rbegin();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL typename system_base_<Domain, ConstantType, ValueLimits>::const_reverse_iterator
system_base_<Domain, ConstantType, ValueLimits>::rend() const {
    return constraints_.rend();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL bool system_base_<Domain, ConstantType, ValueLimits>::empty() const {
    return constraints_.empty();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL std::size_t system_base_<Domain, ConstantType, ValueLimits>::size() const {
    return constraints_.size();
}

template <domain_space Domain, typename ConstantType, typename ValueLimits>
adl_IMPL std::string system_base_<Domain, ConstantType, ValueLimits>::to_string() const {
    std::string repr = "{";
    char const* sep = "";
    for (auto& constraint : this->constraints_) {
        repr += sep;
        repr += constraint.to_string();
        sep = ", ";
    }
    repr += "}";
    return repr;
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__system__system_base___hpp__
