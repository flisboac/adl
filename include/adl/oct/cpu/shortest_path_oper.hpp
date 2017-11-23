// $flavio.lisboa @ 2017-11-23.
//
/*
 * @file shortest_path_oper.hpp
 */


#ifndef adl__oct__cpu__shortest_path_oper__hpp__
#define adl__oct__cpu__shortest_path_oper__hpp__

#include "adl.cfg.hpp"
#include "adl/timer.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/dbm/traits.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/var.hpp"

adl_BEGIN_MAIN_MODULE(oct)
namespace cpu {

template <typename DbmType, typename ContextType>
class shortest_path_oper : public oper_base_<shortest_path_oper, DbmType, ContextType, void> {
    using superclass_ = oper_base_<shortest_path_oper, DbmType, ContextType, void>;

public:
    shortest_path_oper() = delete;
    shortest_path_oper(shortest_path_oper const&) = delete;
    shortest_path_oper(shortest_path_oper &&) noexcept = default;
    shortest_path_oper& operator=(shortest_path_oper const&) = delete;
    shortest_path_oper& operator=(shortest_path_oper &&) noexcept = default;

    shortest_path_oper(dbm_type& dbm) : dbm_(&dbm) {}

    void on_execute_() {
        using namespace adl::operators;
        using namespace adl::dsl;
        auto dbm = *dbm_;
        for (auto k = dbm.first_var(); k < dbm.end_var(); k++) {
            for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
                for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                    dbm.at(i, j) = std::min(dbm.at(i, k), dbm.at(k, j));
                }
            }
        }
    }

private:
    dbm_type* dbm_;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__shortest_path_oper__hpp__
