// $flisboac 2017-02-05
/**
 * @file adl.cfg.hpp
 */
#ifndef adl__cfg__hpp__
#define adl__cfg__hpp__

//
// [[ CONFIGURABLE DEFINITIONS ]]
//
// Must remember to always change the CmakeLists.txt AND the below preprocessor definitions to have correct
// version information! In the future, generate this file from CMake before building the library (but then find a way
// for IDE's to not get confused).
//

#define adl_CONFIG_NAME adl
#define adl_CONFIG_VERSION_MAJOR 0
#define adl_CONFIG_VERSION_MINOR 2
#define adl_CONFIG_VERSION_PATCH 0
#define adl_CONFIG_VERSION_ABI 0 // Increased at each breaking change, major bump certainly bumps ABI version, minor bump should not (but may?) lead to abi bump


#ifndef adl_BUILDING_DOCS

//
// [[ PREPROCESSOR FLAGS ]]
//


#if defined(adl_NO_EXCEPTIONS)
    #define adl_CONFIG_IS_USING_EXCEPTIONS 0
    #ifdef CL_HPP_ENABLE_EXCEPTIONS
        #pragma warning("OpenCL has exceptions enabled, but ADL does not. Surprises may occur.")
    #endif
#elif defined(CL_HPP_ENABLE_EXCEPTIONS)
    #define adl_CONFIG_IS_USING_EXCEPTIONS 1
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
    #define adl_CONFIG_IS_STRIPPED_BINARY !adl_CONFIG_IS_DEBUG
#endif


//
// [[ FEATURE DETECTION ]]
//


// COMPILERS

#define adl_CONFIG_COMPILER_GCC   1
#define adl_CONFIG_COMPILER_CLANG 2
#define adl_CONFIG_COMPILER_MSVC  3

#define adl_CONFIG_COMPILER_IS_MSVC (adl_CONFIG_COMPILER_DETECTED == adl_CONFIG_COMPILER_MSVC)
#define adl_CONFIG_COMPILER_IS_GCC (adl_CONFIG_COMPILER_DETECTED == adl_CONFIG_COMPILER_GCC)
#define adl_CONFIG_COMPILER_IS_CLANG (adl_CONFIG_COMPILER_DETECTED == adl_CONFIG_COMPILER_CLANG)

#if defined(_MSC_VER)
    #define adl_CONFIG_COMPILER_DETECTED adl_CONFIG_COMPILER_MSVC
    #if !(_MSC_VER >= 1910) // MSVC++ 2017, relaxed constexpr rules
        #error Unsupported MSVC version.
    #endif
#elif defined(__clang__)
    #define adl_CONFIG_COMPILER_DETECTED adl_CONFIG_COMPILER_CLANG
    #if !(__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 4))
        #error Unsupported Clang C++ version.
    #endif
#elif defined(__GNUC__)
    #define adl_CONFIG_COMPILER_DETECTED adl_CONFIG_COMPILER_GCC
    #if __GNUC__ < 5
        #error Unsupported G++ version.
    #endif
#else
    #define adl_CONFIG_COMPILER_DETECTED 0
#endif

#if adl_CONFIG_COMPILER_IS_CLANG || adl_CONFIG_COMPILER_IS_GCC
    #define adl_CONFIG_COMPILER_IS_GCC_LIKE 1
#else
    #define adl_CONFIG_COMPILER_IS_GCC_LIKE 0
#endif

#if !adl_CONFIG_COMPILER_DETECTED
    #pragma message("What compiler is this? The world is so vast...")
#endif

// OPERATING SYSTEMS

#define adl_CONFIG_OS_LINUX   1
#define adl_CONFIG_OS_MACOS   2
#define adl_CONFIG_OS_WINDOWS 3

#define adl_CONFIG_OS_IS_LINUX (adl_CONFIG_OS_DETECTED == adl_CONFIG_OS_IS_LINUX)
#define adl_CONFIG_OS_IS_MACOS (adl_CONFIG_OS_DETECTED == adl_CONFIG_OS_IS_MACOS)
#define adl_CONFIG_OS_IS_WINDOWS (adl_CONFIG_OS_DETECTED == adl_CONFIG_OS_IS_WINDOWS)

#if defined(_WIN32) || defined(_WIN64)
    #define adl_CONFIG_OS_DETECTED adl_CONFIG_OS_WINDOWS
#elif defined(__MAC__)
    #define adl_CONFIG_OS_DETECTED adl_CONFIG_OS_MACOS
#elif defined(__linux__)
    #define adl_CONFIG_OS_DETECTED adl_CONFIG_OS_LINUX
#else
    #define adl_CONFIG_OS_DETECTED 0
#endif

#if !adl_CONFIG_OS_DETECTED
    #pragma message("What OS is this? The world is so vast...")
#endif

// LANGUAGE STANDARDS

#define adl_CONFIG_LANG_CPP14 1
#define adl_CONFIG_LANG_CPP17 2

#define adl_CONFIG_LANG_IS_CPP14 (adl_CONFIG_LANG_DETECTED == adl_CONFIG_LANG_CPP14)
#define adl_CONFIG_LANG_IS_CPP17 (adl_CONFIG_LANG_DETECTED == adl_CONFIG_LANG_CPP17)

// Sorry dude, not supporting pre-C++14. So many goodies... C++14 allows the library to be what it is! :)
#if __cplusplus < 201402L
    #error Your compiler or toolkit does not properly support C++14.
#endif

#if __cplusplus > 201402L
    #define adl_CONFIG_LANG_DETECTED adl_CONFIG_LANG_CPP17
#else
    #define adl_CONFIG_LANG_DETECTED adl_CONFIG_LANG_CPP14
#endif

// VARIOUS STANDARDS

#if defined(_POSIX_VERSION)
    #define adl_CONFIG_STD_IS_POSIX 1
#else
    #define adl_CONFIG_STD_IS_POSIX 0
#endif


//
// [[ NON-CONFIGURABLE DEFINITIONS ]]
//


#define adl_STRFY_(s__) #s__
#define adl_STRFY(s__) adl_STRFY_(s__)
#define adl_TPASTE3_(a__, b__, c__) a__ ## b__ ## c__
#define adl_TPASTE3(a__, b__, c__) adl_TPASTE3_(a__, b__, c__)

#define adl_CONFIG_NAME_STR adl_STRFY(adl_CONFIG_NAME)
#define adl_CONFIG_VERSION_STR adl_STRFY(adl_CONFIG_VERSION_MAJOR) "." adl_STRFY(adl_CONFIG_VERSION_MINOR) "." adl_STRFY(adl_CONFIG_VERSION_PATCH)

#define adl_ROOT_NAMESPACE_NAME adl
#define adl_ABI_NAMESPACE_NAME adl_TPASTE3(abi_v, adl_CONFIG_VERSION_ABI, _)

#define adl_ROOT_PATH adl_ROOT_NAMESPACE_NAME::adl_ABI_NAMESPACE_NAME
#define adl_ROOT_MODULE_PATH(m_name__) adl_ROOT_PATH::m_name__

#define adl_BEGIN_ROOT_MODULE namespace adl_ROOT_NAMESPACE_NAME { inline namespace adl_ABI_NAMESPACE_NAME {
#define adl_END_ROOT_MODULE }}
#define adl_BEGIN_MAIN_MODULE(name__) namespace adl_ROOT_NAMESPACE_NAME { inline namespace adl_ABI_NAMESPACE_NAME { namespace name__ {
#define adl_END_MAIN_MODULE }}}

// In the future, this will be used to properly build shared binaries in cumbersome systems (e.g. Windows), if the
// library ever transitions from being header-only to being compiled. Also, it allows adding attribute specifiers for
// the entire API with very little effort -- that is, if the need to mass-apply such markers ever become necessary.
#define adl_API
#define adl_IAPI
#define adl_VAR
#define adl_IVAR
#define adl_CLASS
#define adl_ICLASS
#define adl_IMPL   inline


#else

#define adl_API
#define adl_IAPI
#define adl_VAR
#define adl_IVAR
#define adl_CLASS
#define adl_ICLASS
#define adl_IMPL   inline

//
// [[ DOCUMENTATION ]]
//

/**
 * User-provided: If defined, sets adl_CONFIG_IS_USING_EXCEPTIONS to @c false.
 * @sa adl_CONFIG_IS_USING_EXCEPTIONS
 */
#define adl_NO_EXCEPTIONS

/**
 * User-provided: If defined, sets adl_CONFIG_IS_DEBUG to @c true.
 * @sa adl_CONFIG_IS_DEBUG
 */
#define adl_DEBUG

/**
 * User-provided: If defined, sets adl_CONFIG_IS_STRIPPED_BINARY to @c true.
 * @sa adl_CONFIG_IS_STRIPPED_BINARY
 */
#define adl_STRIPPED

/**
 * Determines if the library will default to throw exceptions on errors or not. The API is prepared to handle
 * both cases, and some dependencies (e.g. OpenCL's cl2.hpp) may follow suit automatically.
 */
#define adl_CONFIG_IS_USING_EXCEPTIONS

/**
 * If @c true, enables more runtime information. This flag acts more like an instruction for the implementation to
 * output more information and than to reduce the binary size. For that, adl_STRIPPED should be defined.
 */
#define adl_CONFIG_IS_DEBUG

/**
 * If @c true, enables symbol stripping (at the language and TU level).
 * Stripping removes additional debugging information and additional symbols. The resulting binary is therefore smaller.
 * As of now, it disables error conditions' descriptions.
 */
#define adl_CONFIG_IS_STRIPPED_BINARY

#endif // adl_BUILDING_DOCS
#endif // adl__cfg__hpp__
