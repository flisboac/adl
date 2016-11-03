#ifndef ADL__OCT__OCT_SYSTEM_BASE_HPP__
#define ADL__OCT__OCT_SYSTEM_BASE_HPP__

#include <cstddef>
#include <map>
#include <initializer_list>

namespace adl {
namespace oct {


template <typename C>
class oct_system_base_ {
private:
    using thisclass_ = oct_system_base_<C>;

public:
    using cons_type = C;
    using value_type = typename cons_type::value_type;
    using var_type = typename cons_type::var_type;
    using vexpr_type = typename cons_type::vexpr_type;

private: using collection_type = std::map<vexpr_type, value_type>;
public:  using const_iterator = typename collection_type::const_iterator;
    using iterator = typename collection_type::iterator;

    inline bool empty() const {
        return _constraints.empty();
    }

    inline bool valid() const {
        bool is_valid = true;
        for (const auto& entry : _constraints) {
            auto cons = get(entry);
            is_valid = is_valid && cons.valid();
            if (!is_valid) break;
        }
        return is_valid;
    }

    inline size_t size() const {
        return _constraints.size();
    }

    inline const_iterator find(var_type var) const {
        return _constraints.find(vexpr_type(var));
    }
    inline const_iterator find(vexpr_type vexpr) const {
        return _constraints.find(vexpr);
    }
    inline iterator find(var_type var) {
        return _constraints.find(vexpr_type(var));
    }
    inline iterator find(vexpr_type vexpr) {
        return _constraints.find(vexpr);
    }

    inline bool contains(var_type var) const {
        return find(var) != _constraints.end();
    }
    inline bool contains(vexpr_type vexpr) const {
        return find(vexpr) != _constraints.end();
    }

    inline const value_type& operator[](var_type var) const {
        return find(var)->second;
    }
    inline const value_type& operator[](vexpr_type vexpr) const {
        return find(vexpr)->second;
    }
    inline const value_type& operator[](const_iterator iter) const {
        return iter->second;
    }
    inline value_type& operator[](var_type var) {
        return find(var)->second;
    }
    inline value_type& operator[](vexpr_type vexpr) {
        return find(vexpr)->second;
    }
    inline value_type& operator[](iterator iter) {
        return iter->second;
    }

    inline const_iterator begin() const {
        return _constraints.begin();
    }
    inline iterator begin() {
        return _constraints.begin();
    }
    inline const_iterator cbegin() const {
        return _constraints.cbegin();
    }
    inline const_iterator end() const {
        return _constraints.end();
    }
    inline iterator end() {
        return _constraints.end();
    }
    inline const_iterator cend() const {
        return _constraints.cend();
    }

    inline void clear() const {
        return _constraints.clear();
    }
    inline cons_type get(const_iterator iter, cons_type default_cons = cons_type::invalid) const {
        return iter != _constraints.end()
            ? cons_type(iter->first.xi(), iter->first.xj(), iter->second)
            : default_cons;
    }
    inline cons_type get(iterator iter, cons_type default_cons = cons_type::invalid) const {
        return iter != _constraints.end()
            ? cons_type(iter->first.xi(), iter->first.xj(), iter->second)
            : default_cons;
    }
    inline cons_type get(vexpr_type vexpr, cons_type default_cons = cons_type::invalid) const {
        return at(find(vexpr), default_cons);
    }
    inline cons_type get(var_type var, cons_type default_cons = cons_type::invalid) const {
        return at(find(var), default_cons);
    }

    inline size_t insert(iterator iter, value_type c) {
        size_t assigned = 0;
        if (iter != _constraints.end()) {
            iter->second = c;
            assigned++;
        }
        return assigned;
    }
    inline size_t insert(cons_type cons) {
        size_t assigned = 0;
        if (cons.valid()) {
            auto iter = find(cons);
            if (iter != _constraints.end()) {
                assigned = insert(iter, cons.c());
            }
        }
        return assigned;
    }
    template <typename I>
    inline size_t insert(I begin, I end) {
        size_t count = 0;
        auto hint = _constraints.begin();
        for (; begin != end; ++begin) {
            cons_type cons = *begin;
            auto value = std::make_pair(cons.to_vexpr(), cons.c());
            auto pair = _constraints.insert(hint, value);
            auto inserted = pair.second;
            if (inserted) {
                count++;
                hint = pair.first;
            }
        }
        return count;
    }
    template <typename I> inline size_t insert(std::initializer_list<cons_type> values) {
        return insert(values.begin(), values.end());
    }
    inline void erase(iterator iter) {
        _constraints.erase(iter);
    }
    inline void erase(var_type var) {
        auto iter = find(var);
        if (iter != _constraints.end()) erase(iter);
    }
    inline void erase(vexpr_type vexpr) {
        auto iter = find(vexpr);
        if (iter != _constraints.end()) erase(iter);
    }
    template <typename I> inline void erase(I begin, I end) {
        for (; begin != end; ++begin) {
            vexpr_type vexpr = *begin;
            erase(vexpr);
        }
    }
    template <typename I> inline void erase(std::initializer_list<cons_type> values) {
        erase(values.begin(), values.end());
    }

    inline thisclass_& operator,(cons_type cons) {
        insert(cons);
        return *this;
    }

protected:
    collection_type _constraints;
};


}}

#endif /* ADL__OCT__OCT_SYSTEM_BASE_HPP__ */
