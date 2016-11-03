#ifndef ADL__OCT__OCT_SYSTEM_STATS_HPP__
#define ADL__OCT__OCT_SYSTEM_STATS_HPP__

#include <cstddef>
#include <map>
#include <limits>

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

    oct_system_stats() = default;
    oct_system_stats(const thisclass_& rhs) = default;
    oct_system_stats(thisclass_&& rhs) = default;
    oct_system_stats(const system_type& system) {
        thisclass_::thisclass_(system.begin(), system.end());
    }
    template <typename Iter> oct_system_stats(Iter begin, Iter end) {
        size_t count = 0;
        for (; begin != end; ++begin) {
            const cons_type& cons = *begin;
            if (cons.valid()) {
                count += insert_var(cons.xi());
                if (cons.xj().valid()) count += insert_var(cons.xj());
                if (!_min_cons_c || cons.c() < _min_cons_c.c()) _min_cons_c = cons;
                if (!_max_cons_c || cons.c() > _max_cons_c.c()) _max_cons_c = cons;
            }
        }
        _valid = count > 0;
    }
    inline bool valid() const {
        return _valid;
    }
    inline size_t size() const {
        return _var_counts.size();
    }
    inline cons_type min_cons_c() const {
        return _min_cons_c;
    }
    inline cons_type max_cons_c() const {
        return _max_cons_c;
    }
    inline size_t min_var_index() const {
        return _min_var_index;
    }
    inline size_t max_var_index() const {
        return _max_var_index;
    }
    inline size_t array_size() const {
        return _max_var_index - _min_var_index;
    }
    inline const_iterator find(var_type var) const {
        return _var_counts.find(var);
    }
    inline var_type var(const_iterator iter) const {
        return iter != _var_counts.end() ? iter->first : var_type::invalid();
    }
    inline size_t count(const_iterator iter) const {
        return iter != _var_counts.end() ? iter->second : 0;
    }
    inline size_t count(var_type var) const {
        return count(_var_counts.find(var));
    }
    inline bool contains(const_iterator iter) const {
        return count(iter) > 0;
    }
    inline bool contains(var_type var) const {
        return count(var) > 0;
    }

    inline const_iterator begin() const {
        return _var_counts.begin();
    }
    inline const_iterator end() const {
        return _var_counts.end();
    }

private:
    size_t insert_var(var_type var) {
        size_t inserted = 0;
        size_t var_index = var.index();
        auto found = _var_counts.find(var);
        if (found != _var_counts.end()) {
            *found = std::make_pair(var, found->second + 1);
            inserted++;
        } else {
            if (_var_counts.insert(std::make_pair(var, 1)).second) {
                inserted++;
            }
        }
        if (!_min_var_index || var_index < _min_var_index) _min_var_index = var_index;
        if (!_max_var_index || var_index > _max_var_index) _max_var_index = var_index;
        return inserted;
    }

private:
    bool _valid = false;
    collection_type _var_counts;
    size_t _min_var_index = 0;
    size_t _max_var_index = 0;
    cons_type _min_cons_c;
    cons_type _max_cons_c;
};


}}

#endif /* ADL__OCT__OCT_SYSTEM_BASE_HPP__ */
