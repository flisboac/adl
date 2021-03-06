// $flavio.lisboa @ 2018-01-15.
//
/*
 * @file is_consistent.hpp
 */


#ifndef adl__oct__cpu__is_consistent_oper__hpp__
#define adl__oct__cpu__is_consistent_oper__hpp__

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
class is_consistent_oper : public detail_::oper_base_<cpu::is_consistent_oper<DbmType, ContextType>, DbmType, ContextType, bool> {
    using superclass_ = detail_::oper_base_<is_consistent_oper, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using queue_type = typename context_type::queue_type;
    using constant_limits = typename dbm_type::constant_limits;

    is_consistent_oper() = delete;
    is_consistent_oper(is_consistent_oper const&) = delete;
    is_consistent_oper(is_consistent_oper &&) noexcept = default;
    is_consistent_oper& operator=(is_consistent_oper const&) = delete;
    is_consistent_oper& operator=(is_consistent_oper &&) noexcept = default;

    explicit is_consistent_oper(queue_type const&, dbm_type const& dbm) : superclass_(), const_dbm_(&dbm), dbm_(nullptr) {}
    explicit is_consistent_oper(queue_type&, dbm_type& dbm) : superclass_(), const_dbm_(&dbm), dbm_(&dbm) {}

    bool on_execute_() {
        using namespace adl::operators;
        auto const& dbm = *const_dbm_;
        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const v_ii = dbm.at(i, i);
            if (v_ii < 0) {
                return false;
            }
            if (dbm_) dbm_->at(i, i) = 0;
        }
        return true;
    }

private:
    dbm_type const* const_dbm_ = nullptr;
    dbm_type* dbm_ = nullptr;
};

}
adl_END_MAIN_MODULE

#endif // adl__oct__cpu__is_consistent_oper__hpp__
