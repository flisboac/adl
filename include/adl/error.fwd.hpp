// $flisboac 2017-03-19
/**
 * @file error.fwd.hpp
 */
#ifndef adl__error__fwd__hpp__
#define adl__error__fwd__hpp__


#include <type_traits>
#include <system_error>

#include "adl.cfg.hpp"


adl_BEGIN_ROOT_MODULE

using error_id_type = int;

enum class errk;       // error_condition, used to throw exceptions when needed
enum class err; // error_code (usage of std::errc is temporary! I hope...)

using error_kind = std::error_condition; // Probably won't change
using error_id = std::error_code; // Probably won't change
using error_info = error_id; // Complements error_id, cannot be made for error_kind, to be extended in the future

using error_category = std::error_category;
    class library_error_code_category;
    class external_error_code_category;     // external_code_category()
    class library_error_kind_category; // kind_category()

class exception;
    class client_exception;
    class internal_exception;

// Utility typedefs

template <typename T> using is_error_id_enum = std::is_error_code_enum<T>;
template <typename T> using is_error_id_enum_t = std::enable_if_t<is_error_id_enum<T>::value, T>;
template <typename T> using is_error_enum_t = std::enable_if_t<
    std::is_error_condition_enum<T>::value || std::is_error_code_enum<T>::value || is_error_id_enum<T>::value, T>;

adl_END_ROOT_MODULE

#endif //adl__error__fwd__hpp__
