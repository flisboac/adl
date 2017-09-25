// $flisboac 2017-09-25
/**
 * @file any.hpp
 */
#ifndef adl__std__any__hpp__
#define adl__std__any__hpp__

#include "adl.cfg.hpp"

#if adl_CONFIG_LANG_IS_CPP17
    #include <any>
#else
    #include "adl/std/impl/any.hpp"
#endif


adl_BEGIN_ROOT_MODULE

#if adl_CONFIG_LANG_IS_CPP17
    using any = std::any;
    using std::any_cast;
    using std::make_any;
    using std::bad_any_cast;
#else
    using any = linb::any;
    using linb::any_cast;
    using linb::bad_any_cast;
    template< class T, class... Args > any make_any( Args&&... args ) {
        return any( T(std::forward<Args>(args)...) );
    };
    template< class T, class U, class... Args >
    any make_any( std::initializer_list<U> il, Args&&... args ) {
        return any( T(il, std::forward<Args>(args)...) );
    };
#endif

adl_END_ROOT_MODULE

#endif //adl__std__any__hpp__
