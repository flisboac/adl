// $flavio.lisboa @ 2018-01-15.
//
/*
 * @file closure.hpp
 */


#ifndef adl__oct__cpu__closure_oper__hpp__
#define adl__oct__cpu__closure_oper__hpp__

#include <algorithm>

#include "adl.cfg.hpp"
#include "adl/timer.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/dbm/traits.hpp"
#include "adl/oct/cpu/oper_base_.hpp"

#include "adl/oct/cpu/close_oper.hpp"
#include "adl/oct/cpu/is_consistent_oper.hpp"
#include "adl/oct/cpu/tighten_oper.hpp"
#include "adl/oct/cpu/is_int_consistent_oper.hpp"
#include "adl/oct/cpu/strengthen_oper.hpp"

adl_BEGIN_MAIN_MODULE(oct)
namespace cpu {

template <typename DbmType, typename ContextType, bool UsingIntegerAlgorithm = DbmType::value_limits::integer>
class closure_oper_impl_ {};

/*
 * REAL ALGORITHM IMPLEMENTATION
 */
template <typename DbmType, typename ContextType>
class closure_oper_impl_<DbmType, ContextType, false> :
    public detail_::oper_base_<cpu::closure_oper<DbmType, ContextType>, DbmType, ContextType, bool>
{
    using superclass_ = detail_::oper_base_<closure_oper<DbmType, ContextType>, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;

    closure_oper_impl_() = delete;
    closure_oper_impl_(closure_oper_impl_ const&) = delete;
    closure_oper_impl_(closure_oper_impl_ &&) noexcept = default;
    closure_oper_impl_& operator=(closure_oper_impl_ const&) = delete;
    closure_oper_impl_& operator=(closure_oper_impl_ &&) noexcept = default;

    explicit closure_oper_impl_(dbm_type const& dbm) :
        superclass_(), close_(&dbm), is_consistent_(&dbm), strengthen_(&dbm) {}
    explicit closure_oper_impl_(dbm_type& dbm) :
        superclass_(), close_(&dbm), is_consistent_(&dbm), strengthen_(&dbm)  {}

    bool on_execute_() {
        close_.get();
        if (is_consistent_.get()) {
            strengthen_.get();
            return true;
        }
        return false;
    }

private:
    close_oper<DbmType, ContextType> close_;
    is_consistent_oper<DbmType, ContextType> is_consistent_;
    strengthen_oper<DbmType, ContextType> strengthen_;
};

/*
 * INTEGER ALGORITHM IMPLEMENTATION
 */
template <typename DbmType, typename ContextType>
class closure_oper_impl_<DbmType, ContextType, true> :
    public detail_::oper_base_<cpu::closure_oper<DbmType, ContextType>, DbmType, ContextType, bool>
{
    using superclass_ = detail_::oper_base_<closure_oper<DbmType, ContextType>, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using value_limits = typename dbm_type::value_limits;

    closure_oper_impl_() = delete;
    closure_oper_impl_(closure_oper_impl_ const&) = delete;
    closure_oper_impl_(closure_oper_impl_ &&) noexcept = default;
    closure_oper_impl_& operator=(closure_oper_impl_ const&) = delete;
    closure_oper_impl_& operator=(closure_oper_impl_ &&) noexcept = default;

    explicit closure_oper_impl_(dbm_type const& dbm) :
        superclass_(), close_(&dbm), is_consistent_(&dbm), tighten_(&dbm), is_int_consistent_(&dbm), strengthen_(&dbm) {}
    explicit closure_oper_impl_(dbm_type& dbm) :
        superclass_(), close_(&dbm), is_consistent_(&dbm), tighten_(&dbm), is_int_consistent_(&dbm), strengthen_(&dbm) {}

    bool on_execute_() {
        close_.get();
        if (is_consistent_.get()) {
            tighten_.get();
            if (is_int_consistent_.get()) {
                strengthen_.get();
                return true;
            }
        }
        return false;
    }

private:
    close_oper<DbmType, ContextType> close_;
    is_consistent_oper<DbmType, ContextType> is_consistent_;
    tighten_oper<DbmType, ContextType> tighten_;
    is_int_consistent_oper<DbmType, ContextType> is_int_consistent_;
    strengthen_oper<DbmType, ContextType> strengthen_;
};

template <typename DbmType, typename ContextType>
class closure_oper : public closure_oper_impl_<DbmType, ContextType> {
public:
    using closure_oper_impl_<DbmType, ContextType>::closure_oper_impl_;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__closure_oper__hpp__
