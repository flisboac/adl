#ifndef ADL__OCT__OCT_VEXPR_BASE_HPP__
#define ADL__OCT__OCT_VEXPR_BASE_HPP__

#include <cstddef>

namespace adl {
namespace oct {

template <typename T>
class oct_vexpr_base_ {
private:
    using thisclass_ = oct_vexpr_base_<T>;

public:
    using var_type = T;

    constexpr oct_vexpr_base_() = default;
    constexpr oct_vexpr_base_(var_type xi, var_type xj) : _xi(xi), _xj(xj) {}
    constexpr explicit oct_vexpr_base_(var_type x) : _xi(x), _xj() {}
    constexpr oct_vexpr_base_(const thisclass_& rhs) = default;
    constexpr oct_vexpr_base_(thisclass_&& rhs) = default;
    inline thisclass_& operator=(const thisclass_& rhs) = default;
    inline thisclass_& operator=(thisclass_&& rhs) = default;

    constexpr inline bool operator==(const thisclass_& rhs) const
        { return (_xi == rhs._xi && _xj == rhs._xj); }
    constexpr inline bool operator!=(const thisclass_& rhs) const
        { return !(*this == rhs); }
    constexpr inline bool operator<(const thisclass_& rhs) const {
        return _xi < rhs._xi || (!(rhs._xi < _xi) && _xj < rhs._xj );
    }
    constexpr inline const var_type& xi() const
        { return _xi; }
    inline thisclass_& xi(const var_type& v)
        { return (_xi = v, *this); }
    constexpr inline const var_type& xj() const
        { return _xj; }
    inline thisclass_& xj(const var_type& v)
        { return (_xj = v, *this); }

protected:
    inline void _invalidate() {
        _xi = var_type::invalid(), _xj = var_type::invalid();
    }

protected:
    var_type _xi;
    var_type _xj;
};


}}

#endif /* ADL__OCT__OCT_VEXPR_BASE_HPP__ */

