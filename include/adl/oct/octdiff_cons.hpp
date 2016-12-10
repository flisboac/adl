#ifndef ADL__OCT__OCTDIFF_CONS_HPP__
#define ADL__OCT__OCTDIFF_CONS_HPP__

#include <array>

#include "adl/oct/oct_cons_base_.hpp"
#include "adl/oct/octdiff_var.hpp"
#include "adl/oct/octdiff_vexpr.hpp"

namespace adl {
namespace oct {

template <typename T> class oct_cons;


template <typename T = float>
class octdiff_cons : public oct_cons_base_<octdiff_vexpr, T> {
private:
    using thisclass_ = octdiff_cons<T>;
    using superclass_ = oct_cons_base_<octdiff_vexpr, T>;

public:
    using value_type = typename superclass_::value_type;
    using vexpr_type = typename superclass_::vexpr_type;
    using var_type = typename superclass_::var_type;

    class pair {
    private:
        using thisclass_ = octdiff_cons<T>::pair;

    public:
        constexpr pair() = default;
        constexpr pair(const thisclass_& rhs) = default;
        constexpr pair(thisclass_&& rhs) = default;
        constexpr pair(octdiff_cons<T> first, octdiff_cons<T> second) : _first(first), _second(second) {}
        inline thisclass_& operator=(const thisclass_& rhs) = default;
        inline thisclass_& operator=(thisclass_&& rhs) = default;

        constexpr inline bool valid() const {
            return _first.valid()                                               // The first constraint must always be provided.
                && (!_second.valid()                                            // In case this conjunction does not represent a single-oct-var diff constraint...
                    || (_first != _second                                       // ... they must be different from each other (represent distinct differences)
                        && _first.xi().to_oct() == _second.xj().swap().to_oct() // ... and consistent (as per the conversion rules).
                        && _first.xj().to_oct() == _second.xi().swap().to_oct()
                    )
                );
        }
        constexpr inline bool single_cons() const {
            return _first.valid() && !_second.valid();
        }
        constexpr inline const octdiff_cons<T>& first() const {
            return _first;
        }
        inline const octdiff_cons<T>& first() {
            return _first;
        }
        constexpr inline const octdiff_cons<T>& second() const {
            return _second;
        }
        inline const octdiff_cons<T>& second() {
            return _second;
        }
        constexpr inline thisclass_ commute() const {
            return thisclass_(_first, _second).commute();
        }
        inline thisclass_& commute() {
            return !single_cons()
                ? *this = thisclass_(_second, _first)
                : *this;
        }

        constexpr inline bool operator==(const thisclass_& rhs) const {
            return (_first == rhs._first && _second == rhs._second)
                || (_first == rhs._second && _second == rhs._first);
        }
        constexpr inline bool operator!=(const thisclass_& rhs) const {
            return !(*this == rhs);
        }
        constexpr explicit inline operator bool() const {
            return valid();
        }
        constexpr inline bool operator !() const {
            return !valid();
        }
        constexpr inline operator oct_cons<T>() const;
        constexpr inline oct_cons<T> to_oct() const;

    private:
        octdiff_cons<T> _first;
        octdiff_cons<T> _second;
    };

    constexpr octdiff_cons() = default;
    constexpr octdiff_cons(var_type xi, var_type xj, T c) : superclass_::oct_cons_base_(xi, xj, c) {}
    constexpr octdiff_cons(var_type x, T c) : superclass_::oct_cons_base_(x, c) {}
    constexpr octdiff_cons(const thisclass_& rhs) = default;
    constexpr octdiff_cons(thisclass_&& rhs) = default;
    inline thisclass_& operator=(const thisclass_& rhs) = default;
    inline thisclass_& operator=(thisclass_&& rhs) = default;

    constexpr inline bool single_oct_var() const
        { return superclass_::_e.single_oct_var(); }
    constexpr inline const var_type xI() const
        { return superclass_::_e.xI(); }
    constexpr inline const var_type xJ() const
        { return superclass_::_e.xJ(); }

    constexpr inline octdiff_cons<T> complement() const {
        return !single_oct_var()
            ? octdiff_cons<T>(superclass_::_e.xj().swap(), superclass_::_e.xi().swap(), superclass_::_c)
            : octdiff_cons<T>::invalid();
    }
    constexpr inline pair conjunction() const {
        return pair(*this, complement());
    }

    constexpr inline operator pair() const {
        return conjunction();
    }

    constexpr static inline thisclass_ invalid()
        { return thisclass_(); }
};


template <typename V>
constexpr inline oct_cons<V> make_octdiff_cons(octdiff_vexpr vexpr, V c) {
    using namespace adl::oct;
    return octdiff_cons<V>(vexpr, c);
}


template <typename V>
constexpr inline oct_cons<V> make_octdiff_cons(octdiff_var xi, octdiff_var xj, V c) {
    using namespace adl::oct;
    return octdiff_cons<V>(xi, xj, c);
}


}}


#include "adl/oct/oct_cons.hpp"

template <typename T>
constexpr inline adl::oct::octdiff_cons<T> operator<=(adl::oct::octdiff_vexpr e, T c) {
    using namespace adl::oct;
    return octdiff_cons<T>(e, c);
}

template <typename T>
constexpr inline adl::oct::oct_cons<T> adl::oct::octdiff_cons<T>::pair::to_oct() const {
    using namespace adl::oct;
    return valid() ?
        !single_cons()
            ? oct_cons<T>(_first.xi().to_oct(), _first.xj().swap().to_oct(), _first.c())
            : oct_cons<T>(_first.xi().to_oct(), _first.c() / 2)
        : oct_cons<T>::invalid();
}

template <typename T>
constexpr inline adl::oct::octdiff_cons<T>::pair::operator adl::oct::oct_cons<T>() const {
    return to_oct();
}


#endif /* ADL__OCT__OCTDIFF_CONS_HPP__ */
