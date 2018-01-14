// $flavio.lisboa @ 2017-11-23.
//
/*
 * @file shortest_path_oper.hpp
 */


#ifndef adl__oct__cpu__shortest_path_oper__hpp__
#define adl__oct__cpu__shortest_path_oper__hpp__

#include <fstream>

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
class shortest_path_oper : public detail_::oper_base_<cpu::shortest_path_oper<DbmType, ContextType>, DbmType, ContextType, void> {
    using superclass_ = detail_::oper_base_<shortest_path_oper, DbmType, ContextType, void>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;

    shortest_path_oper() = delete;
    shortest_path_oper(shortest_path_oper const&) = delete;
    shortest_path_oper(shortest_path_oper &&) noexcept = default;
    shortest_path_oper& operator=(shortest_path_oper const&) = delete;
    shortest_path_oper& operator=(shortest_path_oper &&) noexcept = default;

    explicit shortest_path_oper(dbm_type& dbm) : superclass_(), dbm_(&dbm) {}

    void on_execute_() {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        for (auto k = dbm.first_var(); k < dbm.end_var(); k++) {
            for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
                auto v_ik = dbm.at(i, k);

                for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                    auto v_ij = dbm.at(i, j);
                    auto v_kj = dbm.at(k, j);
                    auto val = std::min(v_ij, v_ik + v_kj);
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
