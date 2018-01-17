// $flavio.lisboa @ 2018-01-15.
//
/*
 * @file is_int_consistent_oper.hpp
 */


#ifndef adl__oct__cpu__is_int_consistent_oper__hpp__
#define adl__oct__cpu__is_int_consistent_oper__hpp__

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

adl_BEGIN_MAIN_MODULE(oct)
namespace cpu {

template <typename DbmType, typename ContextType>
class is_int_consistent_oper : public detail_::oper_base_<cpu::is_int_consistent_oper<DbmType, ContextType>, DbmType, ContextType, bool> {
    using superclass_ = detail_::oper_base_<is_int_consistent_oper, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using value_limits = typename dbm_type::value_limits;

    is_int_consistent_oper() = delete;
    is_int_consistent_oper(is_int_consistent_oper const&) = delete;
    is_int_consistent_oper(is_int_consistent_oper &&) noexcept = default;
    is_int_consistent_oper& operator=(is_int_consistent_oper const&) = delete;
    is_int_consistent_oper& operator=(is_int_consistent_oper &&) noexcept = default;

    explicit is_int_consistent_oper(dbm_type const& dbm) : superclass_(), dbm_(&dbm) {}

    bool on_execute_() {
        using namespace adl::operators;
        auto const& dbm = *dbm_;
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const v_iI = dbm.at(i, -i);
            auto const v_Ii = dbm.at(-i, i);
            if (v_iI + v_Ii < 0) {
                return false;
            }
        }
        return true;
    }

private:
    dbm_type const* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__is_int_consistent_oper__hpp__
