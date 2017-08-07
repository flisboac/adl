// $flisboac 2017-03-19
/**
 * @file optional.hpp
 */
#pragma once
#ifndef adl__std__optional__hpp__
#define adl__std__optional__hpp__

#include "adl.cfg.hpp"

#if adl_CONFIG_LANG_IS_CPP17
    #include <optional>
    adl_BEGIN_ROOT_MODULE
        using nullopt_t = std::nullopt_t;
        template <typename T> using optional = std::optional<T>;
        template <typename T> optional<T> make_optional(T value) { return std::make_optional(value); }
    adl_END_ROOT_MODULE
#else
    #include "adl/std/impl/optional.hpp"
    adl_BEGIN_ROOT_MODULE
        using nullopt_t = std::experimental::nullopt_t;
        template <typename T> using optional = std::experimental::optional<T>;
        template <typename T> optional<T> make_optional(T value) { return std::experimental::make_optional(value); }
    adl_END_ROOT_MODULE
#endif



#endif //adl__std__optional__hpp__
