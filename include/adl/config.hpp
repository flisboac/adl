// $flisboac 2017-03-05
/**
 * @file config.hpp
 */
#ifndef adl__config__hpp__
#define adl__config__hpp__

#include "adl.cfg.hpp"

adl_BEGIN_ROOT_MODULE

namespace config {

    constexpr char const* const name = adl_CONFIG_NAME_STR;
    namespace version {
        constexpr char const* const name = adl_CONFIG_VERSION_STR;
        constexpr int const major = adl_CONFIG_VERSION_MAJOR;
        constexpr int const minor = adl_CONFIG_VERSION_MINOR;
        constexpr int const patch = adl_CONFIG_VERSION_PATCH;
        constexpr int const abi = adl_CONFIG_VERSION_ABI;
    }
}

adl_END_ROOT_MODULE

#endif //adl__config__hpp__
