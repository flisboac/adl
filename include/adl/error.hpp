// $flisboac 2017-03-01
/**
 * @file error.hpp
 */
#pragma once
#ifndef adl__error__hpp__
#define adl__error__hpp__

#include "adl.cfg.hpp"

#include <string>
#include <type_traits>
#include <exception>
#include <system_error>

#include "adl/config.hpp"
#include "adl.fwd.hpp"


namespace adl {
namespace adl_ABI_NAMESPACE_NAME {


enum class error_kind_id : error_id_type {

    warning = -2,
    ok = 0,
    error
};


enum class error_id : error_id_type {

    warn = -2,
    end = -1,
    ok = 0, // Everything is fine :) .
    error = 1, // An unknown error, generally originated from this library.
    external_error = 2, // An unknown error on a dependent library or service.
    invalid_error_id = 3
};


library_error_kind_category const& error_kind_category();
library_error_category const& library_category();
external_error_category const& external_category();
template <typename T>
    error_context make_error_context(std::enable_if_t<std::is_error_condition_enum<T>::value, T> enum_value);


class library_error_category : public error_category {
public:
    using error_category::error_category;
    virtual ~library_error_category() {}

    virtual const char* name() const noexcept;
    virtual std::string message(error_id_type value) const;
    virtual bool equivalent(error_id_type value, error_condition const& einfo) const noexcept;
};


class library_error_kind_category : public library_error_category {};


class external_error_category : public error_category {
public:
    using error_category::error_category;
    virtual ~external_error_category() {}

    virtual const char* name() const noexcept;
    virtual std::string message(error_id_type value) const;
    virtual bool equivalent(error_id_type value, error_condition const& einfo) const noexcept;
};


class error_context {
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
    error_id_type value() const noexcept;
    const error_category& category() const noexcept;
    std::string message() const;
    bool will_throw() const noexcept;
    bool ok() const noexcept;
    bool warning() const noexcept;
    bool error() const noexcept;

    // Operations
    void assign(int value, error_category const& category) const noexcept;
    void clear() noexcept;
    void throw_if() const noexcept(false);
    const error_context& done() const noexcept(false); // throw_if(); return *this;

    // Conversion
    bool to_bool() const noexcept;
    error_condition to_error_condition() const noexcept;
    operator bool() const noexcept;
    operator error_condition() const noexcept;

private:
    error_condition condition_;
    //std::vector<std::unique_ptr<error_info&>> info_;
};


class exception : public std::exception {
public:
    exception(const exception&) = default;
    virtual exception& operator=(const exception& rhs) = default;
    virtual ~exception();

    exception(const char* what, error_context context);
    exception(error_context context);

    const error_context& context() const;
    virtual error_id id() const;
    virtual error_kind_id kind() const;
    virtual const char* what() const noexcept;
    virtual std::string message() const;

private:
    error_context context_;
};


} // adl_ABI_NAMESPACE_NAME
} // adl

namespace std {

template <> struct is_error_condition_enum<adl::error_id> : std::true_type {};
template <> struct is_error_condition_enum<adl::error_kind_id> : std::true_type {};


template <>
struct hash<adl::error_context> {
    using argument_type = adl::error_context;
    using result_type = std::size_t;
    result_type operator()(argument_type const& ctx) const;
};


inline std::error_condition make_error_condition(adl::error_id id) {
    return std::error_condition(static_cast<adl::error_id_type>(id), adl::library_category());
}

inline std::error_condition make_error_condition(adl::error_kind_id id) {
    return std::error_condition(static_cast<adl::error_id_type>(id), adl::error_kind_category());
}

}

#endif // adl__error_info__hpp__
