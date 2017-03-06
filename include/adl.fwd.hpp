// $flisboac 2017-02-28
/**
 * @file adl.fwd.hpp
 */
#pragma once
#ifndef adl__fwd__hpp__
#define adl__fwd__hpp__

#include "adl.cfg.hpp"

#include <system_error>

namespace adl {
inline namespace adl_ABI_NAMESPACE_NAME {

//
// error.hpp
//

using error_id_type = int; // will always be int, for compatibility with <system_error>
using error_condition = std::error_condition;
using error_code = std::error_code;

enum class error_id;
enum class error_kind_id;

using error_category = std::error_category;
    class library_error_category;
    class library_error_kind_category;
    class external_error_category;
class error_context;

class exception; // everything is a runtime_exception unless stated otherwise, language errors are not encapsulated (e.g. std::bad_*)
    //class validation_exception; // Arises from invalid inputs
    //class security_exception; // authentication/authorization problems
    //class arithmetic_exception; // Arithmetic-related errors, e.g. overflow, underflow
    //class out_of_bounds_exception; // Array indexing errors, domain errors, etc
    //class system_exception; // originates from the operating system or any of its subsystems/modules
    //class io_exception; // io-related errors, standard or not, includes networking

} // v1_
} // adl

#endif // adl__fwd__hpp__
