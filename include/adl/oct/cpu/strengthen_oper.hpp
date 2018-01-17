// $flisboac 2018-01-14
/**
 * @file strengthen.hpp
 */
#pragma once
#ifndef adl__oct__cpu__strengthen_oper__hpp__
#define adl__oct__cpu__strengthen_oper__hpp__


#include <algorithm>

#include "adl.cfg.hpp"
#include "adl/timer.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/dbm/traits.hpp"
#include "adl/oct/cpu/oper_base_.hpp"

adl_BEGIN_MAIN_MODULE(oct)
namespace cpu {

template <typename DbmType, typename ContextType>
class strengthen_oper : public detail_::oper_base_<cpu::strengthen_oper<DbmType, ContextType>, DbmType, ContextType, void> {
    using superclass_ = detail_::oper_base_<strengthen_oper, DbmType, ContextType, void>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using constant_limits = typename dbm_type::constant_limits;

    strengthen_oper() = delete;
    strengthen_oper(strengthen_oper const&) = delete;
    strengthen_oper(strengthen_oper &&) noexcept = default;
    strengthen_oper& operator=(strengthen_oper const&) = delete;
    strengthen_oper& operator=(strengthen_oper &&) noexcept = default;

    explicit strengthen_oper(dbm_type& dbm) : superclass_(), dbm_(&dbm) {}

    void on_execute_() {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                auto const v_ij = dbm.at(i, j);
                auto const v_iI = dbm.at(i, -i);
                auto const v_Jj = dbm.at(-j, j);
                auto const v_alt = (v_iI + v_Jj) / 2;
                auto const val = constant_limits::min(v_ij, v_alt);
                dbm.at(i, j) = val;
            }
        }
    }

private:
    dbm_type* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif //adl__oct__cpu__strengthen_oper__hpp__
