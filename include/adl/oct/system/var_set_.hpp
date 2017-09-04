// $flisboac 2017-07-10
/**
 * @file var_set_.hpp
 */
#ifndef adl__oct__system__var_set___hpp__
#define adl__oct__system__var_set___hpp__


#include <map>
#include <iterator>
#include <cassert>

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
            explicit value_type(VarType var);

        bool valid() const noexcept;
        std::string const& name() const noexcept;
        std::size_t count() const noexcept;
        std::size_t positive_count() const noexcept;
        std::size_t negative_count() const noexcept;
        var_type const& normalized_var() const noexcept;

        bool equals(value_type const& rhs) const noexcept;
        int compare(value_type const& rhs) const noexcept;

        bool operator<(value_type const& rhs) const noexcept;
        bool operator<=(value_type const& rhs) const noexcept;
        bool operator==(value_type const& rhs) const noexcept;
        bool operator!=(value_type const& rhs) const noexcept;
        bool operator>=(value_type const& rhs) const noexcept;
        bool operator>(value_type const& rhs) const noexcept;

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
    using container_type_ = std::set<value_type>; // normalized (positive-only)

public:
    using const_iterator = typename container_type_::const_iterator;
    using iterator = typename container_type_::iterator;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator rbegin() const;
    const_iterator rend() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    var_set& clear();
    var_set& reset();
    void setup(var_type var);
    std::pair<iterator, bool> insert(var_type var);
    void erase_one(var_type var);
    var_type last_var() const;

    std::size_t count(var_type var) const; // unnormalized occurences
    std::size_t count_all(var_type var) const; // considers all occurrences, positive + negative
    const_iterator find(var_type var) const;
    value_type const& get(var_type var) const;
    value_type const& operator[](var_type var) const; // returns invalid if the var is not included

private:
    iterator find(var_type var);
    std::pair<iterator, bool> find_or_insert_(var_type var);

private:
    container_type_ data_;
    value_type const dummy_data_;
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
    reset();
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
adl_IMPL typename var_set<Domain>::var_type const& var_set<Domain>::value_type::normalized_var() const noexcept {
    return normalized_var_;
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::equals(value_type const& rhs) const noexcept {
    return normalized_var_.equals(rhs.normalized_var_);
}

template <domain_space Domain>
adl_IMPL int var_set<Domain>::value_type::compare(value_type const& rhs) const noexcept {
    return normalized_var_.compare(rhs.normalized_var_);
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator<(value_type const& rhs) const noexcept {
    return compare(rhs) < 0;
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator<=(value_type const& rhs) const noexcept {
    return compare(rhs) <= 0;
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator==(value_type const& rhs) const noexcept {
    return equals(rhs);
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator!=(value_type const& rhs) const noexcept {
    return !equals(rhs);
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator>=(value_type const& rhs) const noexcept {
    return compare(rhs) >= 0;
}

template <domain_space Domain>
adl_IMPL bool var_set<Domain>::value_type::operator>(value_type const& rhs) const noexcept {
    return compare(rhs) > 0;
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
    return normalized_var();
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
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::begin() const {
    return data_.begin();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::end() const {
    return data_.end();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::rbegin() const {
    return data_.rbegin();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::rend() const {
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
adl_IMPL var_set<Domain>& var_set<Domain>::reset() {
    for (auto iter = data_.begin(); iter != data_.end(); ++iter) iter->reset();
    return *this;
}

template <domain_space Domain>
adl_IMPL void var_set<Domain>::setup(var_type var) {
    auto results = find_or_insert_(var);
    if (!results.second) throw std::logic_error("Variable already set up.");
}

template <domain_space Domain>
adl_IMPL std::pair<typename var_set<Domain>::iterator, bool> var_set<Domain>::insert(var_type var) {
    auto results = find_or_insert_(var);
    auto iter = results.first;
    bool inserted = results.second;
    auto& var_data = const_cast<typename iterator::value_type&>(*iter);
    bool add_result = var_data.add(var);
    assert(add_result); // Being paranoid here.
    return { iter, inserted };
}

template <domain_space Domain>
adl_IMPL void var_set<Domain>::erase_one(var_type var) {
    auto iter = find(var);
    if (iter != end()) {
        iter->subtract(var);
        //if (!iter->valid()) data_.erase(iter);
    }
}

template <domain_space Domain>
typename var_set<Domain>::var_type var_set<Domain>::last_var() const {
    return data_.rbegin()->normalized_var();
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::count(var_type var) const {
    std::size_t total = 0;
    auto iter = find(var);
    if (iter != end()) {
        if (var.positive()) total = iter->positive_count();
        if (var.negative()) total = iter->negative_count();
    }
    return total;
}

template <domain_space Domain>
adl_IMPL std::size_t var_set<Domain>::count_all(var_type var) const {
    std::size_t total = 0;
    auto iter = find(var);
    if (iter != end()) total = iter->count();
    return total;
};

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::const_iterator var_set<Domain>::find(var_type var) const {
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) return data_.find(value_type(idx_var));
    return data_.end();
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::iterator var_set<Domain>::find(var_type var) {
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) return data_.find(value_type(idx_var));
    return data_.end();
}

template <domain_space Domain>
adl_IMPL std::pair<typename var_set<Domain>::iterator, bool> var_set<Domain>::find_or_insert_(var_type var) {
    auto idx_var = var.to_identity().to_normalized();
    if (idx_var.valid()) {
        value_type value(idx_var);
        iterator iter = data_.find(value);
        if (iter == data_.end()) {
            return data_.insert(value);
        }
        return { iter, false };
    }
    throw std::logic_error("Invalid variable.");
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::value_type const& var_set<Domain>::get(var_type var) const {
    auto iter = find(var);
    if (iter != end()) return *iter;
    return dummy_data_;
}

template <domain_space Domain>
adl_IMPL typename var_set<Domain>::value_type const& var_set<Domain>::operator[](var_type var) const {
    return get(var);
}


} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__system__var_set___hpp__
