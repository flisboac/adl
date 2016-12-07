#ifndef ADL__OCT__OCT_CONS_BASE_HPP__
#define ADL__OCT__OCT_CONS_BASE_HPP__


namespace adl {
namespace oct {


template <typename V, typename T>
class oct_cons_base_ {
private:
    using thisclass_ = oct_cons_base_<V, T>;

public:
    typedef T value_type;
    using vexpr_type = V;
    using var_type = typename vexpr_type::var_type;

    constexpr oct_cons_base_() = default;
    constexpr oct_cons_base_(var_type xi, var_type xj, T c) : _e(vexpr_type(xi, xj)), _c(c) {}
    constexpr oct_cons_base_(var_type x, T c) : _e(vexpr_type(x)), _c(c) {}
    constexpr oct_cons_base_(vexpr_type e, T c) : _e(e), _c(c) {}
    constexpr oct_cons_base_(const thisclass_& rhs) = default;
    constexpr oct_cons_base_(thisclass_&& rhs) = default;
    inline thisclass_& operator=(const thisclass_& rhs) = default;
    inline thisclass_& operator=(thisclass_&& rhs) = default;
    constexpr explicit inline operator bool() const
        { return valid(); }
    constexpr inline bool operator !() const
        { return !valid(); }
    constexpr inline bool valid() const {
        return _e.valid();
    }
    constexpr inline bool operator==(const thisclass_& rhs) const
        { return _e == rhs._e; }
    constexpr inline bool operator!=(const thisclass_& rhs) const
        { return !(*this == rhs); }
    constexpr inline bool operator<(const thisclass_& rhs) const
        { return _e < rhs._e; }
    inline thisclass_& invalidate() {
        return (_e.invalidate(), *this);
    }
    constexpr inline const var_type& xi() const
        { return _e.xi(); }
    inline thisclass_& xi(const var_type& v)
        { return (_e.xi(v), *this); }
    constexpr inline const var_type& xj() const
        { return _e.xj(); }
    inline thisclass_& xj(const var_type& v)
        { return (_e.xj(v), *this); }
    constexpr inline const T& c() const
        { return _c; }
    inline thisclass_& c(T v)
        { return (_c = v, *this); }
    constexpr inline vexpr_type to_vexpr() const
        { return _e; }
    constexpr inline operator vexpr_type() const
        { return to_vexpr(); }

protected:
    vexpr_type _e;
    T _c = T();
};


}}


#endif /* ADL__OCT__OCT_CONS_BASE_HPP__ */

