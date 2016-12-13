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

    // TODO custom iterator class that creates *_cons objects
protected: using collection_type = std::map<vexpr_type, value_type>;
public: using const_iterator = typename collection_type::const_iterator;
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

    inline bool contains(var_type var) const {
        return find(var) != _constraints.end();
    }
    inline bool contains(vexpr_type vexpr) const {
        return find(vexpr) != _constraints.end();
    }

    inline const value_type& operator[](var_type var) const {
        return operator[](find(var));
    }
    inline const value_type& operator[](vexpr_type vexpr) const {
        return operator[](find(vexpr));
    }
    inline const value_type& operator[](const_iterator iter) const {
        return iter->second;
    }

    inline const_iterator begin() const {
        return _constraints.begin();
    }
    inline const_iterator cbegin() const {
        return _constraints.cbegin();
    }
    inline const_iterator end() const {
        return _constraints.end();
    }
    inline const_iterator cend() const {
        return _constraints.cend();
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

protected:
    collection_type _constraints;
};


}}

#endif /* ADL__OCT__OCT_SYSTEM_BASE_HPP__ */
