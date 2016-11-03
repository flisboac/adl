#ifndef ADL__OCT__OCT_SYSTEM_STATS_HPP__
#define ADL__OCT__OCT_SYSTEM_STATS_HPP__

#include <cstddef>
#include <map>

namespace adl {
namespace oct {

template <typename T>
class oct_system_stats {
private:
    using thisclass_ = oct_system_stats<T>;

public:
    using system_type = T;
    using cons_type = typename system_type::cons_type;
    using value_type = typename system_type::value_type;
    using var_type = typename system_type::var_type;
    using vexpr_type = typename system_type::vexpr_type;

private: using collection_type = std::map<var_type, size_t>;
public:  using iterator = typename collection_type::iterator;
    using const_iterator = typename collection_type::const_iterator;

    inline var_type smallest_var() const;
    inline var_type greatest_var() const;
    inline size_t count(const var_type& var) const;
    inline const_iterator find(const var_type& var) const;

    inline const_iterator begin() const;
    inline const_iterator end() const;

private:
    collection_type _var_counts;
};


}}

#endif /* ADL__OCT__OCT_SYSTEM_BASE_HPP__ */
