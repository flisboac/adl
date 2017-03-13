// $flisboac 2017-03-01
/**
 * @file error.hpp
 */
#pragma once
#ifndef adl__error__hpp__
#define adl__error__hpp__


#include <string>
#include <type_traits>
#include <exception>
#include <system_error>
#include <memory>

#include "adl.fwd.hpp"

#include "adl/config.hpp"
#include "adl/intl.hpp"


adl_BEGIN_ROOT_MODULE

//
// error.hpp
//

template <typename T> using enable_if_enum_t = std::enable_if_t<std::is_enum<T>::value, T>;

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
    class client_exception; // errors that have origins on user-provided data or intervention (e.g. invalid arguments, etc)
    class internal_exception; // error originated from the system, may be recoverable but it's not caused by users of the library
    class internal_error; // Denotes that an unrecoverable error condition ocurred. All unrecoverable conditions ends in error.
    // Some ideas:
    //class validation_exception; // Arises from invalid inputs
    //class security_exception; // authentication/authorization problems
    //class arithmetic_exception; // Arithmetic-related errors, e.g. overflow, underflow
    //class out_of_bounds_exception; // Array indexing errors, domain errors, etc
    //class system_exception; // originates from the operating system or any of its subsystems/modules
    //class io_exception; // io-related errors, standard or not, includes networking

/**
 * Enumerates all possible types of errors.
 *
 * The library uses this enumeration for determining when a specific error condition (or error code) value is subject
 * to special treatment. The library uses `std::error_category` machinery to convert codes from different places into
 * something it can work with.
 */
enum class error_kind_id : error_id_type {

    /**
     * The code for abnormal situations that can be ignored or recovered from. In other words, it is intended to
     * represent operations that are not entirely successful, and is used to notify both the user and the library's
     * implementation of that fact. The state of the program should continue to be valid and usable after raising a
     * warning; if that's not the case, an error must be raised instead. Also, because of the said reasoning, warning
     * conditions should not be eligible for exception throwing, except in cases where the user forces warnings to
     * be treated as errors.
     *
     * If warnings are treated as errors, its default exception class should be `adl::internal_exception`.
     */
    warning = -2,

    /**
     * The code for normal situations or completely ignored conditions.
     */
    ok = 0,

    /**
     * A client error is a recoverable condition caused by misuse of the library, to some extent. Cases such as
     * missing parameters and invalid inputs can be considered client errors, and their recovery stays entirely at the
     * client's discretion. The library's state should still be valid and usable after a client error occurs, so as to
     * allow the client to correct the faulty conditions.
     *
     * The default exception class for client errors is `adl::client_exception`.
     *
     * @see adl::client_exception
     */
    client_error,

    /**
     * The default code for faulty conditions. These may be recoverable or not, and that depends solely
     * on domain knowledge (e.g. read the operation's description, it should give a clue on the after-state of the
     * library). When confronted with an error-kind code, the library will most likely raise an exception or
     * cancel the operation.
     *
     * The default exception class for errors is `adl::internal_exception`.
     *
     * @see adl::internal_exception
     */
    error,

    /**
     * The code for faulty conditions that are critical and, generally, not recoverable. The client may assume that
     * the state of the library is not valid or usable after a critical error occurs. Also, when a critical error
     * occurs, only destructors and finalizers alike should be used by clients of the library.
     *
     * The default exception class for errors is `adl::internal_error`.
     *
     * @see adl::internal_error
     */
    critical_error
};

/**
 * Enumerates all the common library's error codes.
 */
enum class error_id : error_id_type {

    warning = -2, //< A generic, unknown, warning code.
    end = -1, //< Indicates the end of an algorithm or sequence. Not an error or warning, per se.
    ok = 0, //< Everything is fine :) .
    error = 1, //< An unknown error, generally originated from this library.
    external_error = 2, //< An unknown error on a dependent library or service.
    invalid_error_id = 3 //< Error code used for invalid code conversions between different error categories.
};

template <typename T>
    adl_API constexpr error_id_type enum_to_int(enable_if_enum_t<T> elem) noexcept;
adl_API library_error_kind_category const& error_kind_category() noexcept;
adl_API library_error_category const& library_category() noexcept;
adl_API external_error_category const& external_category() noexcept;
template <typename T, typename = enable_if_enum_t<T>>
    adl_API error_context make_error_context(T enum_value) noexcept;


class adl_CLASS library_error_kind_category : public error_category {
private:
    using superclass_ = error_category;
public:
    using error_category::error_category;
    virtual ~library_error_kind_category() {}

    virtual const char* name() const noexcept;
    virtual std::string message(error_id_type value) const;
    virtual bool equivalent(error_id_type value, error_condition const& einfo) const noexcept;
};


class adl_CLASS library_error_category : public error_category {
private:
    using superclass_ = error_category;
public:
    using error_category::error_category;
    virtual ~library_error_category() {}

    virtual const char* name() const noexcept;
    virtual std::string message(error_id_type value) const;
    virtual bool equivalent(error_id_type value, error_condition const& einfo) const noexcept;
};


class adl_CLASS external_error_category : public error_category {
private:
    using superclass_ = error_category;
public:
    using error_category::error_category;
    virtual ~external_error_category() {}

    virtual const char* name() const noexcept;
    virtual std::string message(error_id_type value) const;
    virtual bool equivalent(error_id_type value, error_condition const& einfo) const noexcept;
};


class adl_CLASS expected {
public:
    expected() = default;
    expected(expected const&) = default;
    expected(expected&&) = default;
    expected& operator=(expected const&) = default;
    expected& operator=(expected&&) = default;
};

class adl_CLASS error_context {
public:
    // Defaultable members
    error_context() noexcept = default;
    error_context(error_context const&) noexcept = default;
    error_context(error_context&&) noexcept = default;
    error_context& operator=(error_context const&) noexcept = default;
    error_context& operator=(error_context&&) noexcept = default;

    // Construction and Assignment
    error_context(error_condition condition) noexcept;
    explicit error_context(int value) noexcept;
    error_context(int value, const error_category& category) noexcept;
    template <typename T>
        error_context(std::enable_if_t<std::is_error_condition_enum<T>::value, T> enum_value) noexcept;
    template <typename T>
        error_context& operator=(std::enable_if_t<std::is_error_condition_enum<T>::value, T> enum_value) const noexcept;

    // Properties
    const std::unique_ptr<error_context>& cause() const noexcept;
    error_id_type value() const noexcept;
    const error_category& category() const noexcept;
    std::string message() const;
    bool will_throw() const noexcept;
    bool ok() const noexcept;
    bool warning() const noexcept;
    bool error() const noexcept;

    // Operations
    error_context& caused_by(error_context const& real_cause);
    void assign(int value, error_category const& category) const noexcept;
    void assign(error_condition condition) const noexcept;
    void clear() noexcept;
    const error_context& done() const noexcept(false);

    // Conversion
    bool to_bool() const noexcept;
    error_condition to_error_condition() const noexcept;
    operator bool() const noexcept;
    operator error_condition() const noexcept;

private:
    error_condition condition_;
    std::unique_ptr<error_context> cause_;
    //std::vector<std::unique_ptr<error_info&>> info_;
};


class adl_CLASS exception : public std::runtime_error {
public:
    exception(const exception&) = default;
    virtual exception& operator=(const exception& rhs) = default;
    virtual ~exception();

    exception(error_context context);
    exception(error_context context, const char* what);
    exception(error_context context, std::string const& what);

    const error_context& context() const;
    virtual error_id id() const;
    virtual error_kind_id kind() const;
    virtual const char* what() const noexcept;
    virtual std::string message() const;

private:
    error_context context_;
};

class adl_CLASS client_exception : public exception {
public:
    using exception::exception;
};

class adl_CLASS internal_exception : public exception {
public:
    using exception::exception;
};

class adl_CLASS internal_error : public exception {
public:
    using exception::exception;
};


adl_END_ROOT_MODULE

namespace std {

template <> struct is_error_condition_enum<adl::error_id> : std::true_type {};
template <> struct is_error_condition_enum<adl::error_kind_id> : std::true_type {};


template <>
struct hash<adl::error_context> {
    using argument_type = adl::error_context;
    using result_type = std::size_t;
    result_type operator()(argument_type const& arg) const {
        return hash<adl::error_id_type>()(static_cast<adl::error_id_type const&>(arg));
    }
};


adl_API std::error_condition make_error_condition(adl::error_id id) {
    return std::error_condition(static_cast<adl::error_id_type>(id), adl::library_category());
}

adl_API std::error_condition make_error_condition(adl::error_kind_id id) {
    return std::error_condition(static_cast<adl::error_id_type>(id), adl::error_kind_category());
}

}


//
// [[ TEMPLATE IMPLEMENTATION ]]
//

adl_BEGIN_ROOT_MODULE

template <typename T>
constexpr error_context make_error_context(T enum_value) noexcept {
    return error_context(error_condition(enum_value));
}

template <typename T, typename = enable_if_enum_t<T>>
constexpr error_id_type enum_to_int(T elem) noexcept {
    return static_cast<error_id_type>(elem);
};


adl_END_ROOT_MODULE

//
// [[ NON-TEMPLATE IMPLEMENTATION ]]
//

adl_BEGIN_ROOT_MODULE

// functions

adl_IMPL const library_error_kind_category& error_kind_category() noexcept {
    static library_error_kind_category category;
    return category;
}

adl_IMPL const library_error_category& library_category() noexcept {
    static library_error_category category;
    return category;
}

adl_IMPL const external_error_category& external_category() noexcept {
    static external_error_category category;
    return category;
}

// library_error_kind_category

adl_IMPL const char* library_error_kind_category::name() const noexcept {
    return "adl::library_category";
}

adl_IMPL std::string library_error_kind_category::message(error_id_type value) const {
    #if !adl_CONFIG_IS_STRIPPED_BINARY
        using namespace literals::intl_;
        switch (value) {
        case enum_to_int(error_kind_id::warning): return "A warning has been raised."_intl;
        case enum_to_int(error_kind_id::ok): return "";
        case enum_to_int(error_kind_id::client_error): return "A recoverable error ocurred."_intl;
        case enum_to_int(error_kind_id::error): return "An error ocurred."_intl;
        case enum_to_int(error_kind_id::critical_error): return "An unrecoverable error ocurred."_intl;
        }
        return "Unknown error kind.";
    #else
        return "";
    #endif
}

adl_IMPL bool library_error_kind_category::equivalent(error_id_type value, error_condition const& einfo) const noexcept {
    if (einfo.category() == error_kind_category()) return superclass_::equivalent(value, einfo);
    return einfo.category().equivalent(einfo.value(), default_error_condition(value));
}

// library_error_category

adl_IMPL const char* library_error_category::name() const noexcept {
    return "adl::library_category";
}

adl_IMPL std::string library_error_category::message(error_id_type value) const {
    #if !adl_CONFIG_IS_STRIPPED_BINARY
        using namespace literals::intl_;
        switch(value) {
        case enum_to_int(error_id::end): return "End of operation."_intl;
        case enum_to_int(error_id::ok): return "Success."_intl;
        case enum_to_int(error_id::external_error): return "An out-of-library (external) error ocurred."_intl;
        case enum_to_int(error_id::invalid_error_id): return "Invalid error condition ID."_intl;
        }
        if (value <= enum_to_int(error_id::warning)) return "An unknown warning has been raised."_intl;
        if (value >= enum_to_int(error_id::error)) return "An unknown error ocurred."_intl;
        return "Unknown error condition ID."_intl;
    #else
        return "";
    #endif
}

adl_IMPL bool library_error_category::equivalent(error_id_type value, error_condition const& einfo) const noexcept {
    if (einfo.category() == error_kind_category()) {
        if (value <= enum_to_int(error_id::warning)) {
            return error_kind_category().equivalent(einfo.value(), std::make_error_condition(error_kind_id::warning));
        } else if (value >= enum_to_int(error_id::error)) {
            return error_kind_category().equivalent(einfo.value(), std::make_error_condition(error_kind_id::error));
        } else {
            return error_kind_category().equivalent(einfo.value(), std::make_error_condition(error_kind_id::ok));
        }
    }
    return superclass_::equivalent(value, einfo);
}

// external_error_category

adl_IMPL const char* external_error_category::name() const noexcept{
    return "adl::external_category";
}

adl_IMPL std::string external_error_category::message(error_id_type value) const {
    #if !adl_CONFIG_IS_STRIPPED_BINARY
        using namespace literals::intl_;
        if (value != 0) return "An error or warning ocurred."_intl;
        return "Success."_intl;
    #else
        return "";
    #endif
}

adl_IMPL bool external_error_category::equivalent(error_id_type value, error_condition const& einfo) const noexcept {
    if (einfo.category() == error_kind_category()) {
        if (value != 0) {
            return error_kind_category().equivalent(einfo.value(), std::make_error_condition(error_kind_id::error));
        } else {
            return error_kind_category().equivalent(einfo.value(), std::make_error_condition(error_kind_id::ok));
        }
        // NOTE Critical and client error conditions are to be included by subclasses of external_error_category.
    }
    return superclass_::equivalent(value, einfo);
}

adl_END_ROOT_MODULE


#endif // adl__error_info__hpp__
