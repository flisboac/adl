#ifndef ADL__OCT__OCTDIFF_VAR_HPP__
#define ADL__OCT__OCTDIFF_VAR_HPP__

#include <string>

#include "adl/adl.hpp"
#include "adl/oct/oct.hpp"
#include "adl/oct/oct_var_base_.hpp"

namespace adl {
namespace oct {

constexpr const char *const default_posvar_suffix = "__pos";
constexpr const char *const default_negvar_suffix = "__neg";

class oct_var;

class octdiff_var : public oct_var_base_<octdiff_var> {
public:
    friend class oct_var_base_<octdiff_var>;
    constexpr octdiff_var() {}
    explicit constexpr octdiff_var(int value) : _value(!is_neg(value) ? value : 0) {}
    constexpr octdiff_var(const octdiff_var& rhs) : _value(rhs._value) {}
    constexpr octdiff_var(octdiff_var&& rhs) : _value(rhs._value) {}
    inline octdiff_var& operator=(const octdiff_var& rhs) { return (_value=(rhs.to_int()), *this); }
    inline octdiff_var& operator=(octdiff_var&& rhs) { return (_value=(rhs.to_int()), *this); }
    inline octdiff_var& operator=(int rhs) { return (_value = !is_neg(rhs) ? rhs : 0, *this); }

    constexpr inline operator oct_var() const;
    constexpr inline bool same_oct_var(octdiff_var rhs) const;
    constexpr inline bool negative() const
        { return _value % 2 == 0; }
    constexpr inline bool positive() const
        { return !negative(); }
    constexpr inline octdiff_var swap() const {
        return negative()
            ? octdiff_var(_value-1)
            : octdiff_var(_value+1);
    }
    inline octdiff_var& operator++()
        { return ((*this)++, (*this)); }
    inline octdiff_var operator++(int) {
        return adl::oct::octdiff_var(valid() ? _value++ : 0);
    }
    inline octdiff_var& operator--()
        { return ((*this)--, (*this)); }
    inline octdiff_var operator--(int){
        return adl::oct::octdiff_var(valid() ? _value-- : 0);
    }
    constexpr inline size_t index() const
        { return valid() ? to_int() - 1 : (size_t) -1; }
    constexpr inline oct_var to_oct() const;

protected:
    int _value { 0 };
};


constexpr static octdiff_var make_octdiff_var(int var = 0) {
    return octdiff_var(var);
}

template <>
inline std::string var_name<octdiff_var>(octdiff_var var, const std::string& base) {
    return var.positive()
        ? base + std::to_string(var.to_oct().normalize().to_int()) + std::string(default_posvar_suffix)
        : base + std::to_string(var.to_oct().normalize().to_int()) + std::string(default_negvar_suffix);
}
template <>
inline std::string var_name<octdiff_var>(octdiff_var var) {
    std::string base = default_var_name;
    return var_name(var, base);
}


namespace literals {

    constexpr inline octdiff_var operator "" _dv(unsigned long long int varId) {
        return octdiff_var(varId);
    }
}


}}


//------------------------------------------------------------------------------
// [ IMPLEMENTATION ]
//------------------------------------------------------------------------------


#include "adl/oct/oct_var.hpp"
//#include "adl/oct/octdiff_var.inc.hpp"

constexpr inline bool adl::oct::octdiff_var::same_oct_var(adl::oct::octdiff_var rhs) const {
    return to_oct().same_var(rhs.to_oct());
}

constexpr inline adl::oct::oct_var adl::oct::octdiff_var::to_oct() const {
    using namespace adl::oct;

    return valid()
        ? negative()
            ? oct_var(-((to_int() - 2) / 2 + 1))
            : oct_var(  (to_int() - 1) / 2 + 1 )
        : oct_var::invalid();
}

constexpr inline adl::oct::octdiff_var::operator adl::oct::oct_var() const
    { return to_oct(); }


#endif /* ADL__OCT__OCTDIFF_VAR_HPP__ */

