#ifndef ADL__OCT__OCT_SYSTEM_BASE_HPP__
#define ADL__OCT__OCT_SYSTEM_BASE_HPP__

#include <cstddef>
#include <set>

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

private: using collection_type = std::set<cons_type>;
public:  using const_iterator = typename collection_type::const_iterator;
    using iterator = typename collection_type::iterator;

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


}}

#endif /* ADL__OCT__OCT_SYSTEM_BASE_HPP__ */
