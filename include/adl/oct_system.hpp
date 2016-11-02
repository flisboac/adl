#ifndef ADT__OCT_SYSTEM_HPP__
#define ADT__OCT_SYSTEM_HPP__

#include <set>
#include <map>

namespace adl {

template <typename C>
class oct_system_base_ {
private:
    using thisclass_ = oct_system_base_<C>;

public:
    using cons_type = C;
    using value_type = typename cons_type::value_type;
    using var_type = typename cons_type::var_type;
    using vexpr_type = typename cons_type::vexpr_type;

private: using collection_type = std::set<cons_type>;
public:  using const_iterator = typename collection_type::const_iterator;
    using iterator = typename collection_type::iterator;

    using var_count_type = size_t;
private: using var_collection_type = std::map<var_type, var_count_type>;
public:  using var_iterator = typename var_collection_type::iterator;
    using const_var_iterator = typename var_collection_type::const_iterator;

    inline bool empty() const;
    inline bool valid() const;
    inline size_t size() const;

    inline const_iterator begin() const;
    inline iterator begin();
    inline const_iterator cbegin() const;
    inline const_iterator end() const;
    inline iterator end();
    inline const_iterator cend() const;

    inline void clear() const;
    inline bool insert(const cons_type& cons);
    template <typename I> inline size_t insert(I begin, I end);
    template <typename I> inline size_t insert(std::initializer_list<cons_type> values);
    inline bool erase(const cons_type& cons);
    template <typename I> inline size_t erase(I begin, I end);
    template <typename I> inline size_t erase(std::initializer_list<cons_type> values);

    inline thisclass_& operator,(const cons_type& cons);

protected:
    collection_type _constraints;
};


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

}

#endif /* ADT__OCT_SYSTEM_HPP__ */
