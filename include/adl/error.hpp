// $flisboac 2017-03-01
/**
 * @file error.hpp
 */
#ifndef adl__error__hpp__
#define adl__error__hpp__


#include <string>
#include <type_traits>
#include <exception>
#include <system_error>
#include <memory>

#include "adl/error.fwd.hpp"

#include "adl/config.hpp"
#include "adl/intl.hpp"


adl_BEGIN_ROOT_MODULE

enum class error_kind_id : error_id_type {
    warning = -2,
    ok = 0,
    client_error,
    error
    // No critical_error for now
};


template <typename T, typename = enable_if_error_enum_t<T>>
    constexpr error_id_type error_enum_to_int(T elem) noexcept;
template <typename T, typename = enable_if_error_context_enum_t<T>>
    error_info make_error_info(T enum_value);
adl_API library_error_category const& library_category() noexcept;
adl_API library_error_kind_category const& error_kind_category() noexcept;
adl_API external_error_category const& external_category() noexcept;
adl_API error_info const& done(error_info const& error);

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


class adl_CLASS exception : public std::runtime_error {
private:
    using superclass_ = std::runtime_error;
public:
    exception(const exception&) = default;
    virtual exception& operator=(const exception& rhs) = default;
    virtual ~exception() {}

    exception(error_info context);
    exception(error_info context, const char* what) noexcept;
    exception(error_info context, std::string const& what) noexcept;

    const error_info& error() const noexcept;
    virtual std::string message() const;

private:
    error_info error_;
};

class adl_CLASS client_exception : public exception {
public:
    using exception::exception;
};

class adl_CLASS internal_exception : public exception {
public:
    using exception::exception;
};


adl_END_ROOT_MODULE

namespace std {

template <> struct is_error_condition_enum<adl::error_kind_id> : std::true_type {};

adl_API std::error_condition make_error_condition(adl::error_kind_id id) {
    return std::error_condition(static_cast<adl::error_id_type>(id), adl::error_kind_category());
}

}


//
// [[ TEMPLATE IMPLEMENTATION ]]
//


adl_BEGIN_ROOT_MODULE


template <typename T, typename>
constexpr error_id_type error_enum_to_int(T elem) noexcept {
    return static_cast<error_id_type>(elem);
};

template <typename T, typename>
adl_IMPL error_info make_error_info(T enum_value) {
    return std::make_error_condition(enum_value);
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
    return std::generic_category();
}

adl_IMPL const external_error_category& external_category() noexcept {
    static external_error_category category;
    return category;
}

adl_IMPL error_info const& done(error_info const& error) {
    error_info error_kind = make_error_info(error_kind_id::error);
    error_info client_error_kind = make_error_info(error_kind_id::client_error);

    if (error_kind == error) {
        throw internal_exception(error);
    } else if (client_error_kind == error) {
        throw client_exception(error);
    }

    return error;
}

// library_error_kind_category

adl_IMPL const char* library_error_kind_category::name() const noexcept {
    return "adl::error_kind";
}

adl_IMPL std::string library_error_kind_category::message(error_id_type value) const {
    #if !adl_CONFIG_IS_STRIPPED_BINARY
        using namespace literals::intl_;
        switch (value) {
        case error_enum_to_int(error_kind_id::warning): return "A warning has been raised."_intl;
        case error_enum_to_int(error_kind_id::ok): return "";
        case error_enum_to_int(error_kind_id::client_error): return "A recoverable error ocurred."_intl;
        case error_enum_to_int(error_kind_id::error): return "An error ocurred."_intl;
        }
        return "Unknown error kind.";
    #else
        return "";
    #endif
}

adl_IMPL bool library_error_kind_category::equivalent(error_id_type value, error_condition const& einfo) const noexcept {
    bool equivalent = false;
    if (einfo.category() == error_kind_category()) {
        equivalent = superclass_::equivalent(value, einfo);
    } else if (einfo.category() == library_category()) {
        // NOTE Only valid as long as there's no custom class for library_category.
        switch (value) {
        case error_enum_to_int(error_kind_id::ok):
            equivalent = !einfo;
            break;
        case error_enum_to_int(error_kind_id::error):
            equivalent = !!einfo;
            break;
        }
    } else {
        equivalent = (einfo == default_error_condition(value));
    }
    return equivalent;
}

// external_error_category

adl_IMPL const char* external_error_category::name() const noexcept{
    return "adl::external_category";
}

adl_IMPL std::string external_error_category::message(error_id_type value) const {
    #if !adl_CONFIG_IS_STRIPPED_BINARY
        using namespace literals::intl_;
        std::string msg;
        if (value != 0) {
            msg = "An error or warning ocurred."_intl;
        } else {
            msg = "Success."_intl;
        }
        return msg;
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

// exception

adl_IMPL exception::exception(error_info context) :
    superclass_(context.message()), error_(context)
{}

adl_IMPL exception::exception(error_info context, const char* what) noexcept :
    superclass_(what), error_(context)
{}

adl_IMPL exception::exception(error_info context, std::string const& what) noexcept :
    superclass_(what), error_(context)
{}

adl_IMPL const error_info& exception::error() const noexcept {
    return error_;
}

adl_IMPL std::string exception::message() const {
    return error_.message();
}

adl_END_ROOT_MODULE


//
// [[[ DOCUMENTATION ]]]
//

adl_BEGIN_ROOT_MODULE

// error_kind_id

/**
 * @enum error_kind_id
 * Enumerates all types of errors that are relevant to the library.
 *
 * The library uses this enumeration for determining when a specific error condition (or error code) value is subject
 * to special treatment. The library uses `std::error_category` machinery to convert codes from different places into
 * something it can work with.
 */

/**
 * @var error_kind_id::warning
 *
 * The code for abnormal situations that can be ignored or recovered from. In other words, it is intended to
 * represent operations that are not entirely successful, and is used to notify both the user and the library's
 * implementation of that fact. The state of the program should continue to be valid and usable after raising a
 * warning; if that's not the case, an error must be raised instead. Also, because of the said reasoning, warning
 * conditions should not be eligible for exception throwing, except in cases where the user forces warnings to
 * be treated as errors.
 *
 * If warnings are treated as errors, its default exception class should be `adl::internal_exception`.
 */

/**
 * @var error_kind_id::ok
 * The code for normal situations or completely ignored conditions.
 */

/**
 * @var error_kind_id::client_error
 * A client error is a recoverable condition caused by misuse of the library, to some extent. Cases such as
 * missing parameters and invalid inputs can be considered client errors, and their recovery stays entirely at the
 * client's discretion. The library's state should still be valid and usable after a client error occurs, so as to
 * allow the client to correct the faulty conditions.
 *
 * The default exception class for client errors is `adl::client_exception`.
 *
 * @see `adl::client_exception`
 */

/**
 * @var error_kind_id::error
 * The default code for faulty conditions. These may be recoverable or not, and that depends solely
 * on domain knowledge (e.g. read the operation's description, it should give a clue on the after-state of the
 * library). When confronted with an error-kind code, the library will most likely raise an exception or
 * cancel the operation.
 *
 * The default exception class for errors is `adl::internal_exception`.
 *
 * @see adl::internal_exception
 */

adl_END_ROOT_MODULE

#endif // adl__error_info__hpp__
