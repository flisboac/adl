// $flisboac 2017-02-05
/**
 * @file adl.cfg.hpp
 */
#ifndef adl__cfg__hpp__
#define adl__cfg__hpp__


#define adl_STRQT(s__) #s__
#define adl_STRFY(s__) adl_STRQT(s__)

#define adl_NAME adl
#define adl_VERSION_MAJOR 0
#define adl_VERSION_MINOR 1
#define adl_VERSION_PATCH 0

#define adl_VERSION_ABI 0 // Increased at each breaking change, major bump certainly bumps ABI version, minor bump should not (but may?) lead to abi bump

#define adl_ROOT_NAMESPACE_NAME adl
#define adl_ABI_NAMESPACE_NAME abi_##adl_VERSION_ABI##_

#if __cplusplus > 201402L
    #define adl_CONFIG_IS_CPP17 1
#else
    #define adl_CONFIG_IS_CPP17 0
#endif

#if defined(adl_USE_NO_HEADERONLY)
    #define adl_CONFIG_IS_HEADER_ONLY 0
#else
    #define adl_CONFIG_IS_HEADER_ONLY 1
#endif

#if defined(adl_NO_EXCEPTIONS)
    #define adl_CONFIG_IS_USING_EXCEPTIONS 0
#else
    #define adl_CONFIG_IS_USING_EXCEPTIONS 1
#endif

#if defined(adl_DEBUG)
    #define adl_CONFIG_IS_DEBUG 1
#else
    #define adl_CONFIG_IS_DEBUG 0
#endif

#if defined(adl_STRIPPED)
    #define adl_CONFIG_IS_STRIPPED_BINARY 1
#else
    #define adl_CONFIG_IS_STRIPPED_BINARY adl_CONFIG_IS_DEBUG
#endif

#endif // adl__cfg__hpp__
