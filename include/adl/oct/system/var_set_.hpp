// $flisboac 2017-07-10
/**
 * @file var_set_.hpp
 */
#ifndef adl__oct__system__var_set___hpp__
#define adl__oct__system__var_set___hpp__


#include <map>
#include <iterator>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"


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

        template <typename VarType, typename = common_var_t<VarType>>
            value_type(VarType var);

        bool valid() const noexcept;
        std::string const& name() const noexcept;
        std::size_t count() const noexcept;
        std::size_t positive_count() const noexcept;
        std::size_t negative_count() const noexcept;
        var_type const& var() const noexcept;

        void reset();
        template <typename VarType, typename = common_var_t<VarType>>
            bool add(VarType var);
        template <typename VarType, typename = common_var_t<VarType>>
            bool subtract(VarType var);

        operator var_type const&() const noexcept;
        explicit operator bool() const noexcept;
        bool operator !() const noexcept;

    private:
        literal_var_type normalized_var_;
        std::string name_;
        std::size_t positive_count_ = 0;
        std::size_t negative_count_ = 0;
    };

private:
    using container_type_ = std::map<identity_var_type, value_type>; // normalized (positive-only)

public:
    using const_iterator = typename container_type_::const_iterator;
    using iterator = typename container_type_::iterator;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator rbegin() const;
    const_iterator rend() const;

    bool empty() const;
    std::size_t size() const;

    var_set& clear();
    template <typename VarType, typename = common_var_t<VarType>>
        std::pair<const_iterator, bool> insert(VarType var);
    template <typename VarType, typename = common_var_t<VarType>>
        void erase_one(VarType var);

    template <typename VarType, typename = common_var_t<VarType>>
        std::size_t count(VarType var) const; // unnormalized occurences
    template <typename VarType, typename = common_var_t<VarType>>
        std::size_t count_all(VarType var) const; // considers all occurrences, positive + negative
    template <typename VarType, typename = common_var_t<VarType>>
        const_iterator find(VarType var) const;
    template <typename VarType, typename = common_var_t<VarType>>
        value_type const& get(VarType var) const noexcept(false);
    template <typename VarType, typename = common_var_t<VarType>>
        value_type const& operator[](VarType var) const; // returns invalid if the var is not included

private:
    template <typename VarType, typename = common_var_t<VarType>>
        iterator find(VarType var);
    template <typename VarType, typename = common_var_t<VarType>>
        iterator find_or_insert_(VarType var);

private:
    container_type_ data_;
};


//
// [[ TEMPLATE IMPLEMENTATION ]]
//

//
// var_set::var_data
//

template <domain_space Domain>
template <typename VarType, typename>
var_set<Domain>::value_type::value_type(VarType var) {
    auto normalized_arg_var = var.to_identity().to_normalized();
    if (normalized_arg_var.valid()) {
        name_ = adl::to_string(var.name());
        normalized_var_ = literal_var_type(normalized_arg_var.id(), string_view(name_.c_str(), name_.size()));
    } else {
        name_ = "";
        normalized_var_ = literal_var_type::invalid();
    }
    positive_count_ = 0;
    negative_count_ = 0;
}

template <domain_space Domain>
adl_IMPL void var_set<Domain>::value_type::reset() {
    positive_count_ = 0;
    negative_count_ = 0;
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::value_type::count() const noexcept {
    return positive_count_ + negative_count_;
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::value_type::positive_count() const noexcept {
    return positive_count_;
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::value_type::negative_count() const noexcept {
    return negative_count_;
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::var_type const& var_set<Domain>::value_type::var() const noexcept {
    return normalized_var_;
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::valid() const noexcept {
    return normalized_var_.valid() && count() > 0;
}

template <domain_space Domain>
adl_IMPL std::string const& var_set<Domain>::value_type::name() const noexcept {
    return name_;
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL bool var_set<Domain>::value_type::add(VarType var) {
    auto normalized_arg_var = var.to_identity().to_normalized();
    bool inserted = normalized_var_.valid() && normalized_var_.equals(normalized_arg_var);
    if (inserted) {
        if (var.positive()) positive_count_++;
        if (var.negative()) negative_count_++;
    }
    return inserted;
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL bool var_set<Domain>::value_type::subtract(VarType var) {
    bool valid_instance_var = normalized_var_.valid();
    auto normalized_arg_var = var.to_identity().to_normalized();
    bool removed = false;
    if (valid_instance_var && normalized_var_.equals(normalized_arg_var)) {
        if (var.positive() && positive_count_ > 0) {
            positive_count_--;
            removed = true;
        }
        if (var.negative() && negative_count_ > 0) {
            negative_count_--;
            removed = true;
        }
    }
    return removed;
}

template <domain_space Domain>
adl_IMPL var_set<Domain>::value_type::operator var_type const&() const noexcept {
    return var();
}

template <domain_space Domain>
adl_IMPL var_set<Domain>::value_type::operator bool() const noexcept {
    return valid();
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator !() const noexcept {
    return !valid();
}

//
// var_set
//

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::begin() const noexcept {
    return data_.begin();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::end() const noexcept {
    return data_.end();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::rbegin() const noexcept {
    return data_.rbegin();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::rend() const noexcept {
    return data_.rend();
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::empty() const noexcept {
    return data_.empty();
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::size() const noexcept {
    return data_.size();
}

template <domain_space Domain>
adl_IMPL var_set<Domain>& var_set<Domain>::clear() {
    data_.clear();
    return *this;
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL std::pair<typename var_set<Domain>::const_iterator, bool> var_set<Domain>::insert(VarType var) {
    auto iter = end();
    bool inserted = false;
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) {
        auto insertion_results = data_.insert(std::make_pair(idx_var, value_type(var)));
        auto& var_data = *insertion_results.first;
        inserted = insertion_results.second;
        if (!var_data.add(var) && inserted) {
            // Being paranoid here.
            data_.erase(insertion_results.first);
        } else {
            iter = const_iterator(insertion_results.first);
        }
    }
    return { iter, inserted };
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL void var_set<Domain>::erase_one(VarType var) {
    auto iter = find(var);
    if (iter != end()) {
        iter->subtract(var);
        //if (!iter->valid()) data_.erase(iter);
    }
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL std::size_t var_set<Domain>::count(VarType var) const {
    std::size_t total = 0;
    auto iter = find(var);
    if (iter != end()) {
        if (var.positive()) total = iter->positive_count();
        if (var.negative()) total = iter->negative_count();
    }
    return total;
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL std::size_t var_set<Domain>::count_all(VarType var) const {
    std::size_t total = 0;
    auto iter = find(var);
    if (iter != end()) total = iter->count();
    return total;
};

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::find(VarType var) const {
    return const_cast<var_set&>(*this).find(var);
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL typename var_set<Domain>::iterator var_set<Domain>::find(VarType var) {
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) return data_.find(idx_var);
    return data_.end();
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL typename var_set<Domain>::iterator var_set<Domain>::find_or_insert_(VarType var) {
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) {
        auto iter = data_.find(idx_var);
        if (iter == data_.end()) return data_.insert(std::make_pair(idx_var, value_type(var))).first;
        return iter;
    }
    return data_.end();
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL typename var_set<Domain>::value_type const& var_set<Domain>::get(VarType var) const noexcept(false) {
    auto iter = find(var);
    if (iter != end()) return iter->var();
    return var_type::invalid();
}

template <domain_space Domain>
template <typename VarType, typename>
adl_IMPL typename var_set<Domain>::value_type const& var_set<Domain>::operator[](VarType var) const {
    return get(var);
}


} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__system__var_set___hpp__
