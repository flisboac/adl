// $flisboac 2018-02-14
/**
 * @file void_t.hpp
 */
#ifndef adl__std__void_t__hpp__
#define adl__std__void_t__hpp__

#include <type_traits>

#include "adl.cfg.hpp"

adl_BEGIN_ROOT_MODULE
#if adl_CONFIG_LANG_IS_CPP17
    template<typename... Ts> using void_t = std::void_t<Ts...>;
#else
    template<typename... Ts> struct make_void_ { typedef void type;};
    template<typename... Ts> using void_t = typename make_void_<Ts...>::type;
#endif
adl_END_ROOT_MODULE

#endif //adl__std__void_t__hpp__
