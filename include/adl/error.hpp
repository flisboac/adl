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
#include "adl/std/optional.hpp"

#include "adl/error.fwd.hpp"

#include "adl/config.hpp"
#include "adl/intl.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

//
// Enumerations
//

enum class errk : error_id_type {
    warning = -2,
    ok = 0,
    client_error,
    error
};

enum class err : error_id_type {
    warning = -2,
    ok = 0,
    error
};

//
// Functions
//

adl_API library_error_code_category const& library_code_category() noexcept;
adl_API external_error_code_category const& external_code_category() noexcept;
adl_API library_error_kind_category const& library_kind_category() noexcept;

adl_API error_kind make_error_kind(errk value) noexcept;
adl_API error_id make_error_id(err value) noexcept;
template <typename T, typename = is_error_id_enum_t<T>> adl_API error_info make_error_info(T value) noexcept;
template <typename T, typename = is_error_enum_t<T>> constexpr error_id_type error_enum_to_int(T value) noexcept;

adl_API void finished(error_info const& info) noexcept(false);
template <typename T> adl_API optional<T> safe_result(error_info const& info, T value);
template <typename T> adl_API T result(error_info const& info, T value) noexcept(false);

//
// Classes
//

class adl_CLASS library_error_code_category : public error_category {
private:
    using superclass_ = error_category;
public:
    virtual ~library_error_code_category() {}
    char const* name() const noexcept;
    std::string message(int value) const;
    virtual std::error_condition default_error_condition( int code ) const noexcept;
};

class adl_CLASS library_error_kind_category : public error_category {
private:
    using superclass_ = error_category;
public:
    virtual ~library_error_kind_category() {}
    char const* name() const noexcept;
    std::string message(int value) const;
    bool equivalent(std::error_code const& code, int value) const noexcept;
};

class adl_CLASS external_error_code_category : public error_category {
private:
    using superclass_ = error_category;
public:
    virtual ~external_error_code_category() {}
    char const* name() const noexcept;
    std::string message(int value) const;
    virtual std::error_condition default_error_condition( int code ) const noexcept;
};

class adl_CLASS exception : public std::runtime_error {
private:
    using superclass_ = std::runtime_error;
public:
    exception(exception const& rhs) noexcept = default;
    exception& operator=(exception const&) noexcept = default;

    exception(char const* what) noexcept;
    exception(std::string const& what) noexcept;
    exception(error_info info);
    exception(error_info info, char const* what) noexcept;
    exception(error_info info, std::string const& what) noexcept;

    error_id code() const noexcept;
    error_info const& info() const noexcept;

private:
    error_info info_;
};

class adl_CLASS client_exception : public exception {
private:
    using superclass_ = exception;
public:
    using exception::exception;
};

class adl_CLASS internal_exception : public exception {
private:
    using superclass_ = exception;
public:
    using exception::exception;
};

adl_END_ROOT_MODULE


//
// [[ STD SPECIALIZATIONS ]]
//
namespace std {
    template <> struct is_error_condition_enum<adl::errk> : public std::true_type {};
    std::error_condition make_error_condition(adl::errk value) noexcept;

    template <> struct is_error_code_enum<adl::err> : public std::true_type {};
    std::error_code make_error_code(adl::err value) noexcept;
}

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

    template <typename T, typename>
    adl_IMPL error_info make_error_info(T value) noexcept {
        return error_info(make_error_id(value));
    }

    template <typename T, typename>
    constexpr error_id_type error_enum_to_int(T value) noexcept {
        return static_cast<error_id_type>(value);
    }

    template <typename T>
    adl_IMPL optional<T> safe_result(error_info const& info, T value) {
        if (info == make_error_kind(errk::ok)) return value;
        return {};
    }

    template <typename T>
    adl_IMPL T result(error_info const& info, T value) noexcept(false) {
        finished(info);
        return value;
    }
adl_END_ROOT_MODULE


//
// [[ NON-TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

    //
    // Functions
    //

    adl_IMPL library_error_code_category const& library_code_category() noexcept {
        static library_error_code_category category;
        return category;
    }

    adl_IMPL external_error_code_category const& external_code_category() noexcept {
        static external_error_code_category category;
        return category;
    }

    adl_IMPL library_error_kind_category const& library_kind_category() noexcept {
        static library_error_kind_category category;
        return category;
    }

    adl_IMPL error_kind make_error_kind(errk value) noexcept {
        return error_kind(error_enum_to_int(value), library_kind_category());
    }

    adl_IMPL error_id make_error_id(err value) noexcept {
        return error_id(error_enum_to_int(value), library_code_category());
    }

    adl_IMPL void finished(error_info const& info) noexcept(false) {
        if (info == make_error_kind(errk::error)) throw internal_exception(info);
        if (info == make_error_kind(errk::client_error)) throw client_exception(info);
    }

    //
    // library_error_code_category
    //

    adl_IMPL char const* library_error_code_category::name() const noexcept {
        return "adl::code";
    }

    adl_IMPL std::string library_error_code_category::message(int value) const {
        #if !adl_CONFIG_IS_STRIPPED_BINARY
            using namespace literals::intl_;
            switch (static_cast<err>(value)) {
            case err::warning: return "A warning has been raised."_intl;
            case err::ok: return "";
            case err::error: return "An unknown error occurred."_intl;
            }
            return "Unknown error code."_intl;
        #else
            return "";
        #endif
    }

    adl_IMPL std::error_condition library_error_code_category::default_error_condition(int code) const noexcept {
        switch(static_cast<err>(code)) {
        case err::warning:
            return make_error_kind(errk::warning);
        case err::ok:
            return make_error_kind(errk::ok);
        case err::error:
            return make_error_kind(errk::error);
        }
        return make_error_kind(errk::error);
    }

    //
    // library_error_kind_category
    //

    adl_IMPL char const* library_error_kind_category::name() const noexcept {
        return "adl::kind";
    }

    adl_IMPL std::string library_error_kind_category::message(int value) const {
        #if !adl_CONFIG_IS_STRIPPED_BINARY
            using namespace literals::intl_;
            switch (static_cast<errk>(value)) {
            case errk::warning: return "A warning has been raised."_intl;
            case errk::ok: return "";
            case errk::client_error: return "A recoverable error ocurred."_intl;
            case errk::error: return "An error ocurred."_intl;
            }
            return "Unknown error kind."_intl;
        #else
            return "";
        #endif
    }

    adl_IMPL bool library_error_kind_category::equivalent(std::error_code const& code, int value) const noexcept {
        switch(value) {
        case error_enum_to_int(errk::ok):
            return !code;
        case error_enum_to_int(errk::error):
            return !!code;
        default:
            return false;
        }
    }

    //
    // external_error_code_category
    //

    adl_IMPL char const* external_error_code_category::name() const noexcept {
        return "adl::external_code";
    }

    adl_IMPL std::string external_error_code_category::message(int value) const {
        #if !adl_CONFIG_IS_STRIPPED_BINARY
            using namespace literals::intl_;
            if (value != 0) return "An external error occurred."_intl;
            return "";
        #else
            return "";
        #endif
    }

    adl_IMPL std::error_condition external_error_code_category::default_error_condition(int code) const noexcept {
        if (code != 0) return make_error_kind(errk::error);
        return make_error_kind(errk::ok);
    }

    //
    // exception
    //

    adl_IMPL exception::exception(char const* what) noexcept :
        superclass_(what) {}

    adl_IMPL exception::exception(std::string const& what) noexcept :
        superclass_(what) {}

    adl_IMPL exception::exception(error_info info) :
        superclass_(info.message()),
        info_(info) {}

    adl_IMPL exception::exception(error_info info, char const* what) noexcept :
        superclass_(what),
        info_(info) {}

    adl_IMPL exception::exception(error_info info, std::string const& what) noexcept :
        superclass_(what),
        info_(info) {}

    adl_IMPL error_id exception::code() const noexcept {
        return info_;
    }

    adl_IMPL error_info const& exception::info() const noexcept {
        return info_;
    }
adl_END_ROOT_MODULE

namespace std {

    std::error_condition make_error_condition(adl::errk value) noexcept {
        return std::error_condition(adl::error_enum_to_int(value), adl::library_kind_category());
    }

    std::error_code make_error_code(adl::err value) noexcept {
        return std::error_code(adl::error_enum_to_int(value), adl::library_code_category());
    }
}

//
// [[[ DOCUMENTATION ]]]
//
adl_BEGIN_ROOT_MODULE
adl_END_ROOT_MODULE

#endif // adl__error_info__hpp__
