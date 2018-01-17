// $flavio.lisboa @ 2018-01-15.
//
/*
 * @file tighten_oper.hpp
 */


#ifndef adl__oct__cpu__tighten_oper__hpp__
#define adl__oct__cpu__tighten_oper__hpp__

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
class tighten_oper : public detail_::oper_base_<cpu::tighten_oper<DbmType, ContextType>, DbmType, ContextType, void> {
    using superclass_ = detail_::oper_base_<tighten_oper, DbmType, ContextType, void>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using value_limits = typename dbm_type::value_limits;

    tighten_oper() = delete;
    tighten_oper(tighten_oper const&) = delete;
    tighten_oper(tighten_oper &&) noexcept = default;
    tighten_oper& operator=(tighten_oper const&) = delete;
    tighten_oper& operator=(tighten_oper &&) noexcept = default;

    explicit tighten_oper(dbm_type& dbm) : superclass_(), dbm_(&dbm) {}

    void on_execute_() {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const v_iI = dbm.at(i, -i);
            dbm.at(i, -i) = 2 * value_limits::floor(v_iI / 2);
        }
    }

private:
    dbm_type* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__tighten_oper__hpp__
