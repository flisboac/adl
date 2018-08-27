// $flisboac 2017-03-11
/**
 * @file intl.hpp
 */
#ifndef adl__intl__hpp__
#define adl__intl__hpp__


#include <cstddef>

#include "adl.cfg.hpp"


adl_BEGIN_ROOT_MODULE

template <typename CharT, typename CharTraits = std::char_traits<CharT>>
class basic_tstring {
public:
    constexpr static std::size_t const nosz = (std::size_t) -1;

    constexpr basic_tstring() = default;
    constexpr basic_tstring(basic_tstring const&) = default;
    constexpr basic_tstring(basic_tstring &&) noexcept = default;
    constexpr basic_tstring& operator=(basic_tstring const&) = default;
    constexpr basic_tstring& operator=(basic_tstring &&) noexcept = default;

    constexpr explicit basic_tstring(CharT const* value) : value_(value), size_(CharTraits::length(value)) {}
    constexpr basic_tstring(CharT const* value, std::size_t size) : value_(value), size_(size) {}

    operator bool() const noexcept { return value_ != nullptr; }
    operator CharT const*() const noexcept { return value_; }
    operator std::basic_string<CharT, CharTraits>() const { return {value_, size_}; }

private:
    CharT const* value_ = nullptr;
    std::size_t size_ = nosz;
};

using tstring = basic_tstring<char>;
using twstring = basic_tstring<wchar_t>;

namespace literals {
    // For internationalization later on. Non-inline on purpose, because it's an internal detail.
    namespace {
        namespace intl_ {
            adl::tstring operator ""_tstr(char const* message, std::size_t size) {
                return tstring(message, size);
            }
            adl::twstring operator ""_tstr(wchar_t const* message, std::size_t size) {
                return twstring(message, size);
            }
        }
    }
}

adl_END_ROOT_MODULE

#endif //adl__intl__hpp__
