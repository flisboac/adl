// $flavio.lisboa @ 2018-02-26.
//
/*
 * @file error.hpp
 */


#ifndef adl__cm__error__hpp__
#define adl__cm__error__hpp__

#include <system_error>

#include "adl.cfg.hpp"
#include "adl/cm.fwd.hpp"


adl_BEGIN_ROOT_MODULE

namespace cm {

std::error_code make_error_code(errc e);

struct adl_CLASS error : public std::system_error {
using std::system_error::system_error;
};

struct adl_CLASS error_category : public std::error_category {
private:
error_category() noexcept = default;

public:
static error_category const& instance() noexcept;

const char* name() const noexcept override;
std::string message(int ev) const override;
};


} // namespace cm

adl_END_ROOT_MODULE

namespace std {

template <> struct adl_CLASS is_error_code_enum<adl::cm::errc> : true_type {};

} // namespace std


/*
 * [[ IMPLEMENTATION ]]
 */
adl_BEGIN_ROOT_MODULE
namespace cm {

adl_IMPL std::error_code make_error_code(errc e) {
    return { static_cast<int>(e), error_category::instance() };
}

adl_IMPL char const* error_category::name() const noexcept {
    return "cm";
}

adl_IMPL error_category const& error_category::instance() noexcept {
    static error_category category;
    return category;
}

adl_IMPL std::string error_category::message(int ev) const {
    auto err = static_cast<errc>(ev);
    switch (err) {
        case errc::ok: return "success";
        case errc::not_ready: return "not ready";
        case errc::error: return "unknown error";
    }
    return std::string("invalid/unknown error code (") + std::to_string(ev) + ")";
}

} // namespace opencl
adl_END_ROOT_MODULE

#endif // adl__cm__error__hpp__
