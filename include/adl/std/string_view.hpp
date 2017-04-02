// $flisboac 2017-04-01
/**
 * @file string_view.hpp
 */
#ifndef adl__std__string_view__hpp__
#define adl__std__string_view__hpp__

#if adl_CONFIG_LANG_IS_CPP17
    #include <string_view>
#else
    #include "adl/char_helper.hpp"
#endif

#include <string>
#include <iterator>
#include <type_traits>
#include <stdexcept>

#include "adl.cfg.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

#if adl_CONFIG_LANG_IS_CPP17

    template <typename Char, typename Traits = std::char_traits<Char>>
        using basic_string_view = std::basic_string_view<Char, Traits>;
    using string_view = std::string_view;
    using wstring_view = std::wstring_view;
    template <typename Char> using char_traits = std::char_traits<Char>;

#else

    template <typename Char> using char_traits = char_helper<Char>;
    template <typename Char, typename Traits = char_traits<Char>> class basic_string_view;
    using string_view = basic_string_view<char>;
    using wstring_view = basic_string_view<wchar_t>;

    template <typename Char, typename Traits>
    class basic_string_view {
    private:
        using helper_ = char_helper<Char>;

    public:
        using traits_type = Traits;
        using value_type = Char;
        using pointer = Char*;
        using const_pointer = const Char*;
        using reference = Char&;
        using const_reference = const Char&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        constexpr static const size_type npos = (size_type) -1;

        static_assert(std::is_same<value_type, typename traits_type::char_type>::value,
            "Character types must be the same.");

        class const_iterator {
        public:
            constexpr const_iterator() noexcept = default;
            constexpr const_iterator(const const_iterator&) = default;
            constexpr const_iterator(const Char* value, size_type size, size_type pos, bool reverse) noexcept;

            constexpr bool operator==(const_iterator rhs) const noexcept;
            constexpr bool operator!=(const_iterator rhs) const noexcept;
            constexpr bool operator>=(const_iterator rhs) const noexcept;
            constexpr bool operator> (const_iterator rhs) const noexcept;
            constexpr bool operator<=(const_iterator rhs) const noexcept;
            constexpr bool operator< (const_iterator rhs) const noexcept;
            constexpr const_iterator operator+(size_type rhs) const;
            constexpr const_iterator operator-(size_type rhs) const;
            constexpr const_iterator& operator+=(size_type rhs);
            constexpr const_iterator& operator-=(size_type rhs);
            constexpr const Char& operator*() const;
            constexpr const Char& operator[](size_type rhs) const;
            constexpr const_iterator& operator++();
            constexpr const_iterator operator++(int);
            constexpr const_iterator& operator--();
            constexpr const_iterator operator--(int);

        private:
            bool reverse_ = false;
            size_type size_ { 0 };
            size_type pos_ { 0 };
            const_pointer value_ { helper_::empty_c_str };
        };

        using iterator = const_iterator;
        using const_reverse_iterator = iterator;
        using reverse_iterator = const_reverse_iterator;

        // Constructors and assignments
        constexpr basic_string_view() noexcept = default;
        constexpr basic_string_view(const basic_string_view& rhs) noexcept = default;
        constexpr basic_string_view(const Char* s, size_type count) noexcept;
        constexpr basic_string_view(const Char* s);
        basic_string_view& operator=(const basic_string_view& rhs) noexcept = default;

        // Iterators
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cend() const noexcept;
        constexpr const_iterator rbegin() const noexcept;
        constexpr const_iterator crbegin() const noexcept;
        constexpr const_iterator rend() const noexcept;
        constexpr const_iterator crend() const noexcept;

        // Accessors and properties
        constexpr const_reference at(size_type pos) const;
        constexpr const_reference front() const;
        constexpr const_reference back() const;
        constexpr const_pointer data() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type length() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr bool empty() const noexcept;

        // Operations
        inline size_type copy(Char* dest, size_type count, size_type pos = 0) const;
        constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const;
        constexpr int compare(basic_string_view rhs) const;
        constexpr int compare(size_type pos, size_type count, basic_string_view rhs) const;
        constexpr int compare(
            size_type pos1,
            size_type count1,
            basic_string_view rhs,
            size_type pos2,
            size_type count2
        ) const;
        constexpr int compare(const Char* s) const;
        constexpr int compare(size_type pos1, size_type count1, const Char* s) const;
        constexpr int compare(size_type pos1, size_type count1, const Char* s, size_type count2) const;

        // Operators
        constexpr const_reference operator[](size_type pos) const;
        constexpr bool operator==(basic_string_view rhs) const;
        constexpr bool operator!=(basic_string_view rhs) const;
        constexpr bool operator< (basic_string_view rhs) const;
        constexpr bool operator<=(basic_string_view rhs) const;
        constexpr bool operator> (basic_string_view rhs) const;
        constexpr bool operator>=(basic_string_view rhs) const;

    private:
        constexpr int compare_result_(int cmp, size_t rhs_size) const;

    private:
        size_type size_ { 0 };
        const_pointer value_ { helper_::empty_c_str };
    };

#endif

template <typename Char>
std::string to_string(basic_string_view<Char> const& str);;

namespace literals {
    inline namespace string_view {

        constexpr basic_string_view<char> operator "" _sv(const char* str, std::size_t len) noexcept;
        constexpr basic_string_view<wchar_t> operator "" _sv(const wchar_t* str, std::size_t len) noexcept;
        constexpr basic_string_view<char16_t> operator "" _sv(const char16_t* str, std::size_t len) noexcept;
        constexpr basic_string_view<char32_t> operator "" _sv(const char32_t* str, std::size_t len) noexcept;
    }
}


adl_END_ROOT_MODULE

#if !adl_CONFIG_LANG_IS_CPP17
    template <class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(
        std::basic_ostream<CharT, Traits>& os,
        adl::basic_string_view<CharT, Traits> v
    );

    template <typename Char, typename Traits>
    constexpr typename adl::basic_string_view<Char, Traits>::const_iterator operator+(
        typename adl::basic_string_view<Char, Traits>::size_type rhs,
        typename adl::basic_string_view<Char, Traits>::const_iterator iter
    );

    template <typename Char, typename Traits>
    constexpr typename adl::basic_string_view<Char, Traits>::const_iterator operator-(
        typename adl::basic_string_view<Char, Traits>::size_type rhs,
        typename adl::basic_string_view<Char, Traits>::const_iterator iter
    );
adl_BEGIN_ROOT_MODULE

//
// functions
//

template <typename Char>
std::string to_string(basic_string_view<Char> const& str) {
    return std::string(str.data(), str.size());
}

namespace literals {
    inline namespace string_view {

        constexpr basic_string_view<char> operator ""_sv(const char* str, std::size_t len) noexcept {
            return basic_string_view<char>(str, len);
        }

        constexpr basic_string_view<wchar_t> operator ""_sv(const wchar_t* str, std::size_t len) noexcept {
            return basic_string_view<wchar_t>(str, len);
        }

        constexpr basic_string_view<char16_t> operator ""_sv(const char16_t* str, std::size_t len) noexcept {
            return basic_string_view<char16_t>(str, len);
        }

        constexpr basic_string_view<char32_t> operator ""_sv(const char32_t* str, std::size_t len) noexcept {
            return basic_string_view<char32_t>(str, len);
        }
    }
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, adl::basic_string_view<CharT, Traits> v) {
    auto string = adl::to_string(v);
    os << string;
    return os;
}

template <typename Char, typename Traits>
constexpr typename adl::basic_string_view<Char, Traits>::const_iterator operator+(
    typename adl::basic_string_view<Char, Traits>::size_type rhs,
    typename adl::basic_string_view<Char, Traits>::const_iterator iter
) {
    return iter + rhs;
}

template <typename Char, typename Traits>
constexpr typename adl::basic_string_view<Char, Traits>::const_iterator operator-(
    typename adl::basic_string_view<Char, Traits>::size_type rhs,
    typename adl::basic_string_view<Char, Traits>::const_iterator iter
) {
    return iter - rhs;
}

#endif // !adl_CONFIG_LANG_IS_CPP17


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
#if !adl_CONFIG_LANG_IS_CPP17
;

//
// basic_string_view
//

template <typename Char, typename Traits>
constexpr basic_string_view<Char, Traits>::basic_string_view(
    const Char* s,
    typename basic_string_view<Char, Traits>::size_type count
) noexcept :
    size_(count),
    value_(s) {}

template <typename Char, typename Traits>
constexpr basic_string_view<Char, Traits>::basic_string_view(const Char* s) :
    size_(helper_::length(s)),
    value_(s) {}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::begin() const noexcept {
    return const_iterator(value_, size_, 0, false);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::cbegin() const noexcept {
    return const_iterator(value_, size_, 0, false);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::end() const noexcept {
    return const_iterator(value_, size_, size_, false);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::cend() const noexcept {
    return const_iterator(value_, size_, size_, false);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::rbegin() const noexcept {
    return const_iterator(value_, size_, 0, true);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::crbegin() const noexcept {
    return const_iterator(value_, size_, 0, true);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::rend() const noexcept {
    return const_iterator(value_, size_, npos, true);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::crend() const noexcept {
    return const_iterator(value_, size_, npos, true);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_reference
basic_string_view<Char, Traits>::operator[](
    typename basic_string_view<Char, Traits>::size_type pos
) const {
    return value_[pos];
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_reference
basic_string_view<Char, Traits>::at(
    basic_string_view::size_type pos
) const {
    return (pos < size_) ? value_[pos] : throw std::out_of_range("Index out of bounds");
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_reference
basic_string_view<Char, Traits>::front() const {
    return this->operator[](0);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_reference
basic_string_view<Char, Traits>::back() const {
    return this->operator[](size_ - 1);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_pointer
basic_string_view<Char, Traits>::data() const noexcept {
    return value_;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::size_type
basic_string_view<Char, Traits>::size() const noexcept {
    return size_;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::size_type
basic_string_view<Char, Traits>::length() const noexcept {
    return size_;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::size_type
basic_string_view<Char, Traits>::max_size() const noexcept {
    return npos - 1;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::empty() const noexcept {
    return size_ == 0;
}


template <typename Char, typename Traits>
typename basic_string_view<Char, Traits>::size_type
basic_string_view<Char, Traits>::copy(
    Char* dest,
    basic_string_view::size_type count,
    basic_string_view::size_type pos
) const {
    size_t nchars = 0;
    const size_t chars_limit = size() - pos;
    const size_t max_chars = count < chars_limit ? count : chars_limit;
    if (pos >= size_) throw std::out_of_range("Index out of bounds");
    for (nchars = 0; nchars < max_chars; ++nchars) dest[nchars] = value_[nchars + pos];
    return nchars;
}

template <typename Char, typename Traits>
constexpr basic_string_view<Char, Traits>
basic_string_view<Char, Traits>::substr(
    basic_string_view::size_type pos,
    basic_string_view::size_type count
) const {
    return (pos < size_)
           ? count < size_ - pos
             ? basic_string_view(value_ + pos, count)
             : basic_string_view(value_ + pos, size_ - pos)
           : throw std::out_of_range("Index out of bounds");
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(basic_string_view rhs) const {
    return compare_result_(Traits::compare(value_, rhs.value_, size_ < rhs.size_ ? size_ : rhs.size_), rhs.size_);
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(
    basic_string_view::size_type pos,
    basic_string_view::size_type count,
    basic_string_view rhs
) const {
    return substr(pos, count).compare(rhs);
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(
    basic_string_view::size_type pos1,
    basic_string_view::size_type count1,
    basic_string_view rhs,
    basic_string_view::size_type pos2,
    basic_string_view::size_type count2
) const {
    return substr(pos1, count1).compare(rhs.substr(pos2, count2));
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(const Char* s) const {
    return compare(basic_string_view(s));
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(
    basic_string_view::size_type pos1,
    basic_string_view::size_type count1,
    const Char* s
) const {
    return substr(pos1, count1).compare(basic_string_view(s));
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare(
    basic_string_view::size_type pos1,
    basic_string_view::size_type count1,
    const Char* s,
    basic_string_view::size_type count2
) const {
    return substr(pos1, count1).compare(basic_string_view(s, count2));
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator==(basic_string_view rhs) const {
    return compare(rhs) == 0;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator!=(basic_string_view rhs) const {
    return compare(rhs) != 0;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator<(basic_string_view rhs) const {
    return compare(rhs) <  0;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator<=(basic_string_view rhs) const {
    return compare(rhs) <= 0;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator>(basic_string_view rhs) const {
    return compare(rhs) >  0;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::operator>=(basic_string_view rhs) const {
    return compare(rhs) >= 0;
}

template <typename Char, typename Traits>
constexpr int basic_string_view<Char, Traits>::compare_result_(int cmp, size_t rhs_size) const {
    return cmp == 0
           ? size_ < rhs_size
             ? -1
             : size_ > rhs_size
               ? 1
               : 0
           : cmp;
}


//
// basic_string_view::const_iterator
//

template <typename Char, typename Traits>
constexpr basic_string_view<Char, Traits>::const_iterator::const_iterator(
    const Char* value,
    basic_string_view::size_type size,
    basic_string_view::size_type pos,
    bool reverse
) noexcept :
    reverse_(reverse),
    size_(size),
    pos_(pos),
    value_(value) {}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator==(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ == rhs.pos_;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator!=(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ != rhs.pos_;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator>=(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ >= rhs.pos_;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator>(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ >  rhs.pos_;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator<=(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ <= rhs.pos_;
}

template <typename Char, typename Traits>
constexpr bool basic_string_view<Char, Traits>::const_iterator::operator<(
    basic_string_view::const_iterator rhs
) const noexcept {
    return pos_ <  rhs.pos_;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::const_iterator::operator+(
    typename basic_string_view<Char, Traits>::size_type rhs
) const {
    return const_iterator(value_, size_, pos_ + rhs, reverse_);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::const_iterator::operator-(
    typename basic_string_view<Char, Traits>::size_type rhs
) const {
    return const_iterator(value_, size_, pos_ - rhs, reverse_);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator&
basic_string_view<Char, Traits>::const_iterator::operator+=(
    typename basic_string_view<Char, Traits>::size_type rhs
) {
    pos_ += rhs;
    return *this;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator&
basic_string_view<Char, Traits>::const_iterator::operator-=(
    typename basic_string_view<Char, Traits>::size_type rhs
) {
    pos_ -= rhs;
    return *this;
}

template <typename Char, typename Traits>
constexpr const Char& basic_string_view<Char, Traits>::const_iterator::operator*() const {
    return value_[pos_];
}

template <typename Char, typename Traits>
constexpr const Char& basic_string_view<Char, Traits>::const_iterator::operator[](
    typename basic_string_view<Char, Traits>::size_type rhs
) const {
    return value_[pos_ + rhs];
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator&
basic_string_view<Char, Traits>::const_iterator::operator++() {
    if (reverse_) {
        --pos_;
    } else {
        ++pos_;
    }
    return *this;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::const_iterator::operator++(int) {
    return const_iterator(value_, size_, reverse_ ? pos_-- : pos_++, reverse_);
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator&
basic_string_view<Char, Traits>::const_iterator::operator--() {
    if (reverse_) {
        ++pos_;
    } else {
        --pos_;
    }
    return *this;
}

template <typename Char, typename Traits>
constexpr typename basic_string_view<Char, Traits>::const_iterator
basic_string_view<Char, Traits>::const_iterator::operator--(int) {
    return const_iterator(value_, size_, reverse_ ? pos_++ : pos_--, reverse_);
}

adl_END_ROOT_MODULE
#endif

#endif // adl__std__string_view__hpp__
