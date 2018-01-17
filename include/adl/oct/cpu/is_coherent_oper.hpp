// $flavio.lisboa @ 2018-01-15.
//
/*
 * @file is_coherent_oper.hpp
 */


#ifndef adl__oct__cpu__is_coherent_oper__hpp__
#define adl__oct__cpu__is_coherent_oper__hpp__

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
class is_coherent_oper : public detail_::oper_base_<cpu::is_coherent_oper<DbmType, ContextType>, DbmType, ContextType, bool> {
    using superclass_ = detail_::oper_base_<is_coherent_oper, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using constant_limits = typename dbm_type::constant_limits;

    is_coherent_oper() = delete;
    is_coherent_oper(is_coherent_oper const&) = delete;
    is_coherent_oper(is_coherent_oper &&) noexcept = default;
    is_coherent_oper& operator=(is_coherent_oper const&) = delete;
    is_coherent_oper& operator=(is_coherent_oper &&) noexcept = default;

    explicit is_coherent_oper(dbm_type const& dbm) : superclass_(), dbm_(&dbm) {}

    bool on_execute_() {
        using namespace adl::operators;
        auto const& dbm = *dbm_;
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                auto const v_ij = dbm.at(i, j);
                auto const v_JI = dbm.at(-j, -i);
                if (v_ij != v_JI) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    dbm_type const* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__is_coherent_oper__hpp__
