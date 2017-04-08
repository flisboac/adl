// $flisboac 2017-03-20
/**
 * @file char_helper.hpp
 */
#ifndef adl__char_helper__hpp__
#define adl__char_helper__hpp__

#include <cwchar>
#include <string>
#include <limits>

#include "adl.cfg.hpp"
#include "adl/char_helper.fwd.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

enum class char_digit_base {
    decimal = 10
};

template <typename Char>
struct adl_CLASS char_definitions {
    constexpr static bool valid = false;
    using char_type = typename std::char_traits<Char>::char_type;
    using int_type = typename std::char_traits<Char>::int_type;
    using off_type = typename std::char_traits<Char>::off_type;
    using pos_type = typename std::char_traits<Char>::pos_type;
    using state_type = typename std::char_traits<Char>::state_type;
};

template <>
struct adl_CLASS char_definitions<char> {
    constexpr static bool valid = true;
    using char_type = typename std::char_traits<char>::char_type;
    using int_type = typename std::char_traits<char_type>::int_type;
    using off_type = typename std::char_traits<char_type>::off_type;
    using pos_type = typename std::char_traits<char_type>::pos_type;
    using state_type = typename std::char_traits<char_type>::state_type;
    using c_str_type = const char_type*;
    constexpr static c_str_type const empty_c_str = "";
    constexpr static int_type character_null = '\0';
    constexpr static int_type character_zero = '0';
    constexpr static int_type character_nine = '9';
    constexpr static int_type character_negated = '-';
    constexpr static int_type character_format = '%';
    constexpr static int_type character_digit_sep = '\'';
};

template <>
struct adl_CLASS char_definitions<wchar_t> {
    constexpr static bool valid = true;
    using char_type = typename std::char_traits<wchar_t>::char_type;
    using int_type = typename std::char_traits<char_type>::int_type;
    using off_type = typename std::char_traits<char_type>::off_type;
    using pos_type = typename std::char_traits<char_type>::pos_type;
    using state_type = typename std::char_traits<char_type>::state_type;
    using c_str_type = const char_type*;
    constexpr static c_str_type const empty_c_str = L"";
    constexpr static const int_type character_null = L'\0';
    constexpr static const int_type character_zero = L'0';
    constexpr static const int_type character_nine = L'9';
    constexpr static const int_type character_negated = L'-';
    constexpr static const int_type character_format = L'%';
    constexpr static const int_type character_digit_sep = L'\'';
};

template <>
struct adl_CLASS char_definitions<char16_t> {
    constexpr static bool valid = true;
    using char_type = typename std::char_traits<char16_t>::char_type;
    using int_type = typename std::char_traits<char_type>::int_type;
    using off_type = typename std::char_traits<char_type>::off_type;
    using pos_type = typename std::char_traits<char_type>::pos_type;
    using state_type = typename std::char_traits<char_type>::state_type;
    using c_str_type = const char_type*;
    constexpr static c_str_type const empty_c_str = u"";
    constexpr static const int_type character_null = u'\0';
    constexpr static const int_type character_zero = u'0';
    constexpr static const int_type character_nine = u'9';
    constexpr static const int_type character_negated = u'-';
    constexpr static const int_type character_format = u'%';
    constexpr static const int_type character_digit_sep = u'\'';
};

template <>
struct adl_CLASS char_definitions<char32_t> {
    constexpr static bool valid = true;
    using char_type = typename std::char_traits<char32_t>::char_type;
    using int_type = typename std::char_traits<char_type>::int_type;
    using off_type = typename std::char_traits<char_type>::off_type;
    using pos_type = typename std::char_traits<char_type>::pos_type;
    using state_type = typename std::char_traits<char_type>::state_type;
    using c_str_type = const char_type*;
    constexpr static c_str_type const empty_c_str = U"";
    constexpr static const int_type character_null = U'\0';
    constexpr static const int_type character_zero = U'0';
    constexpr static const int_type character_nine = U'9';
    constexpr static const int_type character_negated = U'-';
    constexpr static const int_type character_format = U'%';
    constexpr static const int_type character_digit_sep = U'\'';
};

template <typename Char>
struct adl_CLASS char_helper : public char_definitions<Char> {
private:
    using std_char_traits_ = std::char_traits<Char>;
    using superclass_ = char_definitions<Char>;

    static_assert(superclass_::valid, "Helper cannot operate on the specified character type.");

public:
    using char_type = typename superclass_::char_type;
    using int_type = typename superclass_::int_type;
    using off_type = typename superclass_::off_type;
    using pos_type = typename superclass_::pos_type;
    using state_type = typename superclass_::state_type;
    using superclass_::empty_c_str;
    using superclass_::character_null;
    using superclass_::character_zero;
    using superclass_::character_nine;
    using superclass_::character_negated;
    using superclass_::character_format;
    using superclass_::character_digit_sep;

    // std::char_traits<T> functions

    constexpr static void assign(char_type& r, const char_type& a) noexcept; //std
    static char_type* assign(char_type* p, std::size_t count, char_type a); //std
    constexpr static bool eq(char_type a, char_type b) noexcept; //std
    constexpr static bool lt(char_type a, char_type b) noexcept; //std
    static char_type* move(char_type* dest, const char_type* src, std::size_t count); //std
    static char_type* copy(char_type* dest, const char_type* src, std::size_t count); //std
    constexpr static int compare(const char_type* a, const char_type* b, size_t count = 0); //std
    constexpr static size_t length(const char_type* a, size_t a_size = 0); //std
    constexpr static const char_type* find(const char_type* p, std::size_t count, const char_type& ch); //std
    constexpr static char_type to_char_type(int_type c) noexcept; //std
    constexpr static int_type to_int_type(char_type c) noexcept; //std
    constexpr static bool eq_int_type(int_type c1, int_type c2) noexcept; //std
    constexpr static int_type eof() noexcept; //std
    constexpr static int_type not_eof(int_type e) noexcept; //std

    // ------

    constexpr static int compare(const char_type* a, const char_type* b, size_t a_size, size_t b_size);
    constexpr static const char* ensure_empty_string(const char_type* s) noexcept;
    template <template <typename> class StringType = std::basic_string, class String = StringType<Char>>
        constexpr static String ensure_empty_string(String const& s)
        noexcept(noexcept(String().empty()) && noexcept(String("")));
    constexpr static const char_type* rfind(const char_type* p, std::size_t count, const char_type& ch);
    constexpr static bool is_digit(const char_type& c) noexcept;
    template <typename T = int> constexpr inline T digit_to_integer(const char_type& c) noexcept;
    template <typename T> constexpr static T to_integer(T fallback_value, const char_type* c, size_t size = 0);

private:
    constexpr static bool is_finished_(const char_type* s, size_t s_size, size_t s_count);
    template <typename T> constexpr static T choose_valid_(std::pair<T, bool> a, T b);
    constexpr static int compare_(const Char* a, size_t a_size, const Char* b, size_t b_size, size_t count);
    constexpr static size_t length_(const char_type* c, size_t c_size, size_t c_count);
    constexpr static const char_type* find_(const char_type* str, size_t size, size_t counter, const char_type& ch);;
    constexpr static const char_type* rfind_(const char_type* str, size_t counter, const char_type& ch);;
    template <typename T> constexpr static std::pair<T, bool> to_integer_(
        const char_type* c,
        size_t c_size,
        size_t c_count = 0,
        T value = 0,
        bool found = false,
        bool negative = false,
        char_digit_base base = char_digit_base::decimal
    );
};

template <typename... Args> adl_API int string_printf(std::string& buffer, const char* fmt, Args... args);
template <typename... Args> adl_API int string_printf(std::string& buffer, const std::string& fmt, Args... args);;
template <typename... Args> adl_API int string_printf(std::wstring& buffer, const wchar_t* fmt, Args... args);;
template <typename... Args> adl_API int string_printf(std::wstring& buffer, const std::wstring& fmt, Args... args);;

namespace {
    template <typename T> adl_IAPI T string_printf_basic_arg_(T value) noexcept;
    template <typename T> adl_IAPI T const* string_printf_basic_arg_(std::basic_string<T> const& value) noexcept;
}

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

//
// functions
//

template <typename... Args>
adl_IMPL int string_printf(std::wstring& buffer, const wchar_t* fmt, Args... args) {
    int chars_written = 0;
    const auto nchars = std::swprintf(nullptr, 0, fmt, string_printf_basic_arg_(args)...) + 1;
    if (nchars > 0 && nchars <= std::numeric_limits<std::size_t>::max()) {
        const size_t size = (size_t) nchars;
        buffer.resize((std::size_t) nchars);
        chars_written = std::swprintf(&buffer[0], buffer.length(), fmt, string_printf_basic_arg_(args)...);
        buffer.resize(buffer.size() - 1);
    }
    return chars_written;
};

template <typename... Args>
adl_IMPL int string_printf(std::string& buffer, const char* fmt, Args... args) {
    int chars_written = 0;
    const auto nchars = std::snprintf(nullptr, 0, fmt, string_printf_basic_arg_(args)...) + 1;
    if (nchars > 0 && nchars <= std::numeric_limits<std::size_t>::max()) {
        buffer.resize((std::size_t) nchars);
        chars_written = std::snprintf(&buffer[0], buffer.length(), fmt, string_printf_basic_arg_(args)...);
        buffer.resize(buffer.size() - 1);
    }
    return chars_written;
}

template <typename... Args>
adl_IMPL int string_printf(std::string& buffer, const std::string& fmt, Args... args) {
    return string_printf(buffer, fmt.c_str(), args...);
}

template <typename... Args>
adl_IMPL int string_printf(std::wstring& buffer, const std::wstring& fmt, Args... args) {
    return string_printf(buffer, fmt.c_str(), args...);
}

template <typename T>
adl_IMPL T string_printf_basic_arg_(T value) {
    return value;
}

template <typename T>
adl_IMPL T const* string_printf_basic_arg_(std::basic_string<T> const& value) {
    return value.c_str();
}

//
// char_helper<Char>
//

template <typename Char>
constexpr void char_helper<Char>::assign(char_type& r, const char_type& a) noexcept { // std
    return (r = a, (void)0);
}

template <typename Char>
adl_IMPL typename char_helper<Char>::char_type* char_helper<Char>::assign(char_type* p, std::size_t count, char_type a) { // std
    return std_char_traits_::assign(p, count, a);
}

template <typename Char>
constexpr bool char_helper<Char>::eq(char_type a, char_type b) noexcept { // std
    return std_char_traits_::eq(a, b);
}

template <typename Char>
constexpr bool char_helper<Char>::lt(char_type a, char_type b) noexcept { // std
    return std_char_traits_::lt(a, b);
}

template <typename Char>
adl_IMPL typename char_helper<Char>::char_type*
char_helper<Char>::move(char_type* dest, const char_type* src, std::size_t count) { // std
    return std_char_traits_::move(dest, src, count);
}

template <typename Char>
adl_IMPL typename char_helper<Char>::char_type*
char_helper<Char>::copy(char_type* dest, const char_type* src, std::size_t count) { // std
    return std_char_traits_::copy(dest, src, count);
}

template <typename Char>
constexpr int
char_helper<Char>::compare(const char_type* a, const char_type* b, std::size_t a_size, std::size_t b_size) {
    return compare_(a, a_size, b, b_size, 0);
}

template <typename Char>
constexpr int char_helper<Char>::compare(const char_type* a, const char_type* b, std::size_t count) { // std
    return compare(a, b, count, count);
}

template <typename Char>
constexpr std::size_t char_helper<Char>::length(const char_type* a, std::size_t a_size) { // std
    return length_(a, a_size, 0);
}

template <typename Char>
constexpr const typename char_helper<Char>::char_type*
char_helper<Char>::find(const char_type* p, std::size_t count, const char_type& ch) { // std
    return find_(p, count, 0, ch);
}

template <typename Char>
constexpr typename char_helper<Char>::char_type char_helper<Char>::to_char_type(int_type c) noexcept { // std
    return std_char_traits_::to_char_type(c);
}

template <typename Char>
constexpr typename char_helper<Char>::int_type char_helper<Char>::to_int_type(char_type c) noexcept { // std
    return std_char_traits_::to_int_type(c);
}

template <typename Char>
constexpr bool char_helper<Char>::eq_int_type(int_type c1, int_type c2) noexcept { // std
    return std_char_traits_::eq_int_type(c1, c2);
}

template <typename Char>
constexpr typename char_helper<Char>::int_type char_helper<Char>::eof() noexcept { // std
    return std_char_traits_::eof();
}

template <typename Char>
constexpr typename char_helper<Char>::int_type char_helper<Char>::not_eof(int_type e) noexcept { // std
    return std_char_traits_::not_eof(e);
}

template <typename Char>
constexpr const char* char_helper<Char>::ensure_empty_string(const char_type* s) noexcept {
    return s == nullptr ? s : empty_c_str;
}

template <typename Char>
template <template <typename> class StringType, class String>
constexpr String char_helper<Char>::ensure_empty_string(const String& s)
noexcept(noexcept(String().empty()) && noexcept(String(""))) {
    return !s.empty() ? s : String("");
}

template <typename Char>
constexpr const typename char_helper<Char>::char_type*
char_helper<Char>::rfind(const char_type* p, std::size_t count, const char_type& ch) { // std
    return rfind_(p, count > 0 ? count : length(p), ch);
}

template <typename Char>
constexpr bool char_helper<Char>::is_digit(const char_type& c) noexcept {
    return c >= character_zero && c <= character_nine;
}

template <typename Char>
template <typename T>
constexpr T char_helper<Char>::digit_to_integer(const char_type& c) noexcept {
    return static_cast<T>(c - character_zero);
}

template <typename Char>
template <typename T>
constexpr T char_helper<Char>::to_integer(T fallback_value, const char_type* c, size_t size) {
    return choose_valid_(to_integer_(c, size, 0, T(0)), fallback_value);
}

template <typename Char>
constexpr bool char_helper<Char>::is_finished_(const char_type* s, size_t s_size, size_t s_count) {
    using char_definitions_ = char_definitions<Char>;
    return s_size == 0
           ? eq(s[s_count], character_null)
           : s_count >= s_size;
}

template <typename Char>
template <typename T>
constexpr T char_helper<Char>::choose_valid_(std::pair<T, bool> a, T b) {
    return a.second ? a.first : b;
}

template <typename Char>
constexpr int char_helper<Char>::compare_(const Char* a, size_t a_size, const Char* b, size_t b_size, size_t count) {
    return !is_finished_(a, a_size, count)
           ? !is_finished_(b, b_size, count)
             ? eq(a[count], b[count])
               ? compare_(a, a_size, b, b_size, count + 1) // none of the strings have finished, continue
               : lt(b[count], a[count]) - lt(a[count], b[count]) // calculate the difference between the two (different) values
             : 1    // b is finished but not a, a is bigger than b, therefore a > b
           : !is_finished_(b, b_size, count)
             ? -1   // b is bigger than a, therefore b > a => a < b
             : 0;   // b is the same length than a and has exactly the same values as a
}

template <typename Char>
constexpr size_t char_helper<Char>::length_(const char_type* c, size_t c_size, size_t c_count) {
    return eq(c[c_count], character_null)
           ? c_count
           : c_size == 0 || c_count < c_size
             ? length_(c, c_size, c_count + 1)
             : c_count;
}

template <typename Char>
constexpr const typename char_helper<Char>::char_type*
char_helper<Char>::find_(const char_type* str, size_t size, size_t counter, const char_type& ch) {
    return eq(str[counter], ch)
           ? str + counter
           : !is_finished_(str, size, counter)
             ? find_(str, size, counter + 1, ch)
             : nullptr;
}

template <typename Char>
constexpr const typename char_helper<Char>::char_type*
char_helper<Char>::rfind_(const char_type* str, size_t counter, const char_type& ch) {
    return eq(str[counter], ch)
           ? str + counter
           : counter > 0
             ? rfind_(str, counter - 1, ch)
             : nullptr;
}

template <typename Char>
template <typename T>
constexpr std::pair<T, bool> char_helper<Char>::to_integer_(
    const char_helper::char_type* c,
    size_t c_size,
    size_t c_count,
    T value,
    bool found,
    bool negative,
    char_digit_base base
) {
    return !is_finished_(c, c_size, c_count)
           ? is_digit(c[c_count])
             ? to_integer_(
                c, c_size, c_count + 1,
                value * ((T)base) + T(negative ? -1 : 1) * digit_to_integer<T>(c[c_count]),
                true,
                negative,
                base)
             : !found
               ? to_integer_(c, c_size, c_count + 1, value, found, !found && eq(c[c_count], character_negated) ? !negative : negative, base)
               : eq(c[c_count], character_digit_sep)
                 ? to_integer_(c, c_size, c_count + 1, value, found, negative, base)
                 : std::make_pair(value, found)
           : std::make_pair(value, found);
}

adl_END_ROOT_MODULE

#endif //adl__char_helper__hpp__
