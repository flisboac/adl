// $flisboac 2017-09-14
/**
 * @file traits.hpp.hpp
 */
#pragma once
#ifndef adl__oct__context__traits__hpp__
#define adl__oct__context__traits__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename ContextType>
struct context_traits {
    constexpr static const bool valid = false;
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__context__traits__hpp__
