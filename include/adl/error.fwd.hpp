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

//
// error.hpp
//

template <typename T> using enable_if_error_context_enum_t = std::enable_if_t<
    std::is_error_condition_enum<T>::value, T>;
template <typename T> using enable_if_error_enum_t = std::enable_if_t<
    std::is_error_condition_enum<T>::value || std::is_error_code_enum<T>::value, T>;

using error_id_type = int; // will always be int, for compatibility with <system_error> and everything else that's an error code
using error_condition = std::error_condition; // will not change, but for library use we have error_info
using error_code = std::error_code; // will not change

enum class error_kind_id;
using error_id = std::errc; // may be substituted in the future (and by that I mean before v1.0) by a custom enum class, but keeping the enum values already in use.

using error_category = std::error_category;
    class library_error_kind_category;
    class external_error_category;
    using library_error_category = decltype(std::generic_category()); // when we get a custom error_id, library_category will have its own class as well
using error_info = error_condition; // In the future, error_info will be its own class, with the same interface as in error_condition and possibly additional contextual information (a la boost::exception).

class exception; // everything is a runtime_exception unless stated otherwise, language errors are not encapsulated (e.g. std::bad_*)
    class client_exception; // errors that have origins on user-provided data or intervention (e.g. invalid arguments, etc)
    class internal_exception; // error originated from the system, may be recoverable but it's not caused by users of the library

adl_END_ROOT_MODULE

#endif //adl__error__fwd__hpp__
