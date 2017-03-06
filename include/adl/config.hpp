// $flisboac 2017-03-05
/**
 * @file config.hpp
 */
#pragma once
#ifndef adl__config__hpp__
#define adl__config__hpp__

#include "adl.cfg.hpp"

#define adl_ROOT_PATH adl_ROOT_NAMESPACE_NAME::adl_ABI_NAMESPACE_NAME
#define adl_ROOT_MODULE_PATH(m_name__) adl_ROOT_PATH::m_name__

namespace adl {
inline namespace adl_ABI_NAMESPACE_NAME {
namespace config {

constexpr char const* const name = adl_STRFY(adl_NAME);
namespace version {
    constexpr int major = adl_VERSION_MAJOR;
    constexpr int minor = adl_VERSION_MINOR;
    constexpr int patch = adl_VERSION_PATCH;
    constexpr int abi = adl_VERSION_ABI;
}

constexpr bool using_exceptions = adl_CONFIG_IS_USING_EXCEPTIONS;
constexpr bool debug = adl_CONFIG_IS_DEBUG;
constexpr bool stripped_binary = adl_CONFIG_IS_STRIPPED_BINARY;
constexpr bool cpp17 = adl_CONFIG_IS_CPP17;

} // config
} // adl_ABI_NAMESPACE_NAME
} // adl


#endif //adl__config__hpp__
