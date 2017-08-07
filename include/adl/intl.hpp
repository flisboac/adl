// $flisboac 2017-03-11
/**
 * @file intl.hpp
 */
#ifndef adl__intl__hpp__
#define adl__intl__hpp__


#include <cstddef>

#include "adl.cfg.hpp"


adl_BEGIN_ROOT_MODULE

namespace literals {
    // For internationalization later on. Non-inline on purpose, because it's an internal detail.
    namespace {
        namespace intl_ {
            char const* operator ""_intl(char const* message, std::size_t size) {
                return message;
            }
        }
    }
}

adl_END_ROOT_MODULE

#endif //adl__intl__hpp__
