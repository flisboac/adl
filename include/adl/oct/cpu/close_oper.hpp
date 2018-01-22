// $flavio.lisboa @ 2017-11-23.
//
/*
 * @file close_oper.hpp
 */


#ifndef adl__oct__cpu__shortest_path_oper__hpp__
#define adl__oct__cpu__shortest_path_oper__hpp__

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
class close_oper : public detail_::oper_base_<cpu::close_oper<DbmType, ContextType>, DbmType, ContextType, void> {
    using superclass_ = detail_::oper_base_<close_oper, DbmType, ContextType, void>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using queue_type = typename context_type::queue_type;
    using constant_limits = typename dbm_type::constant_limits;

    close_oper() = delete;
    close_oper(close_oper const&) = delete;
    close_oper(close_oper &&) noexcept = default;
    close_oper& operator=(close_oper const&) = delete;
    close_oper& operator=(close_oper &&) noexcept = default;

    close_oper(queue_type&, dbm_type& dbm) : superclass_(), dbm_(&dbm) {}

    void on_execute_() {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        for (auto k = dbm.first_var(); k < dbm.end_var(); k.increment_oct()) {
            auto const K = -k;
            auto const v_kK = dbm.at(k, K);
            auto const v_Kk = dbm.at(K, k);

            for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
                auto const v_ik = dbm.at(i, k);
                auto const v_iK = dbm.at(i, K);

                for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                    auto const v_ij = dbm.at(i, j);
                    auto const v_kj = dbm.at(k, j);
                    auto const v_Kj = dbm.at(K, j);
                    auto const val = constant_limits::min({
                        v_ij,
                        v_ik + v_kj,
                        v_iK + v_Kj,
                        v_ik + v_kK + v_Kj,
                        v_iK + v_Kk + v_kj
                    });
                    dbm.at(i, j) = val;
                }
            }
        }
    }

private:
    dbm_type* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__shortest_path_oper__hpp__
