// $flavio.lisboa @ 2018-01-26.
//
/*
 * @file context.hpp
 */

#ifndef adl__oct__cl__context__hpp__
#define adl__oct__cl__context__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

class adl_CLASS context {
public:
    context() = delete;
    context(context const&) = delete;
    context(context &&) noexcept = default;
    context& operator=(context const&) = delete;
    context& operator=(context &&) noexcept = default;

    explicit context(::cl::Context& context);

    static context make();

private:
    ::cl::Context context_;
};

} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__context__hpp__
