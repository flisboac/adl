// $flisboac 2017-03-19
/**
 * @file error.fwd.hpp
 */
#ifndef adl__error__fwd__hpp__
#define adl__error__fwd__hpp__


#include <type_traits>
#include <tuple>
#include <system_error>

#include "adl.cfg.hpp"
#include "adl/std/any.hpp"

adl_BEGIN_ROOT_MODULE

using std::error_code;
using std::error_condition;
using std::make_error_code;
using std::make_error_condition;
using std::is_error_condition_enum;
using std::is_error_code_enum;

enum class error_stack_mode;

using error_id_type = int;

enum class errk; // conceptually, this is an error_condition, but called error_kind instead to indicate a common ground

// No more adl::err. Or at least not a "universal" one.
// Each module will have its own error_code enum, or something similar.

using error_category = std::error_category;
    class library_error_kind_category; // kind_category()

class exception_root;
    class exception;  // conceptually similar to std::logic_exception, or java.lang.Exception
    class error;      // Conceptually similar to std::runtime_exception, or java.lang.Error

template <typename SubClass, typename EnumType, typename BaseExceptionType = std::system_error> struct error_traits_base;
template <typename EnumType> struct error_traits;

template <typename EnumType> using base_exception_t = typename error_traits<EnumType>::base_exception_type;
template <typename EnumType, EnumType ErrorCode> using exception_t = typename error_traits<EnumType>::template code_traits<ErrorCode>::exception_type;

class error_context;

class error_entry;
class error_stack;
    template <typename Allocator = std::allocator<error_entry>> class basic_dynamic_error_stack;
    using dynamic_error_stack = basic_dynamic_error_stack<>;
    class single_error_stack;
    class throwing_error_stack;

adl_END_ROOT_MODULE

#endif //adl__error__fwd__hpp__
