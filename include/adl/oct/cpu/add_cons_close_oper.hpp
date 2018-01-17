// $flisboac 2018-01-16
/**
 * @file add_cons_oper.hpp
 */
#ifndef adl__oct__cpu__add_cons_close_oper__hpp__
#define adl__oct__cpu__add_cons_close_oper__hpp__

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

template <typename DbmType, typename ContextType, bool UsingIntegerAlgorithm = DbmType::constant_limits::integer>
class add_cons_close_oper_impl_ {};

/*
 * REAL ALGORITHM IMPLEMENTATION
 */
template <typename DbmType, typename ContextType>
class add_cons_close_oper_impl_<DbmType, ContextType, false> :
    public detail_::oper_base_<cpu::add_cons_close_oper<DbmType, ContextType>, DbmType, ContextType, bool>
{
    using superclass_ = detail_::oper_base_<cpu::add_cons_close_oper<DbmType, ContextType>, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using queue_type = typename context_type::queue_type;
    using constant_limits = typename dbm_type::constant_limits;
    using counterpart_cons_type = typename dbm_type::counterpart_identity_cons_type;
    using cons_type = typename dbm_type::identity_cons_type;
    using octdiff_conjunction_type = typename dbm_type::octdiff_conjunction_type;

    add_cons_close_oper_impl_() = delete;
    add_cons_close_oper_impl_(add_cons_close_oper_impl_ const&) = delete;
    add_cons_close_oper_impl_(add_cons_close_oper_impl_ &&) noexcept = default;
    add_cons_close_oper_impl_& operator=(add_cons_close_oper_impl_ const&) = delete;
    add_cons_close_oper_impl_& operator=(add_cons_close_oper_impl_ &&) noexcept = default;

    add_cons_close_oper_impl_(queue_type&, dbm_type& dbm, cons_type di, cons_type dj) : superclass_(), dbm_(&dbm), di_(di), dj_(dj) {}
    add_cons_close_oper_impl_(queue_type&, dbm_type& dbm, cons_type cons) : add_cons_close_oper_impl_(dbm, cons, cons_type::invalid()) {}
    add_cons_close_oper_impl_(queue_type&, dbm_type& dbm, counterpart_cons_type oct_cons) : add_cons_close_oper_impl_(dbm, oct_cons.split()) {}
    add_cons_close_oper_impl_(queue_type&, dbm_type& dbm, octdiff_conjunction_type conj) : add_cons_close_oper_impl_(dbm, conj.di(), conj.dj()) {}

    bool on_execute_() {
        return add_cons_(di_) && (dj_.valid() && add_cons_(dj_));
    }

private:
    bool add_cons_(cons_type cons) {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        auto const d = cons.constant();
        auto const a = cons.xi();
        auto const A = -a;
        auto const b = cons.xj();
        auto const B = -b;

        auto const v_Aa = dbm.at(A, a);
        auto const v_bB = dbm.at(b, B);

        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const I = -i;
            auto const v_iI = dbm.at(i, I);

            auto const v_ia = dbm.at(i, a);
            auto const v_iB = dbm.at(i, B);
            auto const v_AI = dbm.at(A, I);
            auto const v_bI = dbm.at(b, I);

            auto const val = constant_limits::min({
                v_iI,
                v_ia + d + v_bI,
                v_iB + d + v_Aa + d + v_bI,
                v_ia + d + v_bB + d + v_AI
            });

            dbm.at(i, I) = val;
        }

        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const I = -i;
            auto const v_iI = dbm.at(i, I);

            for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                if (j != I) {
                    auto const J = -j;
                    auto const v_Jj = dbm.at(J, j);
                    auto const v_ij = dbm.at(i, j);

                    auto const v_ia = dbm.at(i, a);
                    auto const v_bj = dbm.at(b, j);
                    auto const v_iB = dbm.at(i, B);
                    auto const v_Aj = dbm.at(A, j);

                    auto const val = constant_limits::min({
                        v_ij,
                        v_ia + d + v_bj,
                        v_iB + d + v_Aj,
                        v_iB + d + v_Aa + d + v_bj,
                        v_ia + d + v_bB + d + v_Aj,
                        (v_iI + v_Jj) / 2
                    });

                    dbm.at(i, j) = val;
                }
            }

            if (dbm.at(i, i) < 0) return false;
        }

        return true;
    }

private:
    dbm_type * dbm_;
    cons_type di_;
    cons_type dj_;
};

/*
 * INTEGER ALGORITHM IMPLEMENTATION
 */
template <typename DbmType, typename ContextType>
class add_cons_close_oper_impl_<DbmType, ContextType, true> :
    public detail_::oper_base_<cpu::add_cons_close_oper<DbmType, ContextType>, DbmType, ContextType, bool>
{
    using superclass_ = detail_::oper_base_<add_cons_close_oper<DbmType, ContextType>, DbmType, ContextType, bool>;

public:
    using dbm_type = DbmType; //typename superclass_::dbm_type;
    using context_type = ContextType; //typename superclass_::context_type;
    using constant_limits = typename dbm_type::constant_limits;
    using counterpart_cons_type = typename dbm_type::counterpart_identity_cons_type;
    using cons_type = typename dbm_type::identity_cons_type;
    using octdiff_conjunction_type = typename dbm_type::octdiff_conjunction_type;

    add_cons_close_oper_impl_() = delete;
    add_cons_close_oper_impl_(add_cons_close_oper_impl_ const&) = delete;
    add_cons_close_oper_impl_(add_cons_close_oper_impl_ &&) noexcept = default;
    add_cons_close_oper_impl_& operator=(add_cons_close_oper_impl_ const&) = delete;
    add_cons_close_oper_impl_& operator=(add_cons_close_oper_impl_ &&) noexcept = default;

    add_cons_close_oper_impl_(dbm_type& dbm, cons_type di, cons_type dj) : superclass_(), dbm_(&dbm), di_(di), dj_(dj) {}
    add_cons_close_oper_impl_(dbm_type& dbm, cons_type cons) : add_cons_close_oper_impl_(dbm, cons, cons_type::invalid()) {}
    add_cons_close_oper_impl_(dbm_type& dbm, counterpart_cons_type oct_cons) : add_cons_close_oper_impl_(dbm, oct_cons.split()) {}
    add_cons_close_oper_impl_(dbm_type& dbm, octdiff_conjunction_type conj) : add_cons_close_oper_impl_(dbm, conj.di(), conj.dj()) {}

    bool on_execute_() {
        return add_cons_(di_) && (dj_.valid() && add_cons_(dj_));
    }

private:
    bool add_cons_(cons_type cons) {
        using namespace adl::operators;
        auto &dbm = *dbm_;
        auto const d = cons.constant();
        auto const a = cons.xi();
        auto const A = -a;
        auto const b = cons.xj();
        auto const B = -b;

        auto const v_Aa = dbm.at(A, a);
        auto const v_bB = dbm.at(b, B);

        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const I = -i;
            auto const v_iI = dbm.at(i, I);

            auto const v_ia = dbm.at(i, a);
            auto const v_iB = dbm.at(i, B);
            auto const v_AI = dbm.at(A, I);
            auto const v_bI = dbm.at(b, I);

            auto const val = constant_limits::min({
                v_iI,
                v_ia + d + v_bI,
                v_iB + d + v_AI,
                v_iB + d + v_Aa + d + v_bI,
                v_ia + d + v_bB + d + v_AI
            });

            dbm.at(i, I) = constant_limits::floor(val / 2) * 2;
        }

        for (auto i = dbm.first_var(); i < dbm.end_var(); i++) {
            auto const I = -i;
            auto const v_iI = dbm.at(i, I);

            for (auto j = dbm.first_var(); j < dbm.end_var(); j++) {
                if (j != I) {
                    auto const J = -j;
                    auto const v_Jj = dbm.at(J, j);
                    auto const v_ij = dbm.at(i, j);

                    auto const v_ia = dbm.at(i, a);
                    auto const v_bj = dbm.at(b, j);
                    auto const v_iB = dbm.at(i, B);
                    auto const v_Aj = dbm.at(A, j);

                    auto const val = constant_limits::min({
                        v_ij,
                        v_ia + d + v_bj,
                        v_iB + d + v_Aj,
                        v_iB + d + v_Aa + d + v_bj,
                        v_ia + d + v_bB + d + v_Aj,
                        (v_iI + v_Jj) / 2
                    });

                    dbm.at(i, j) = val;
                }
            }

            if (dbm.at(i, i) < 0) return false;
        }

        return true;
    }

private:
    dbm_type * dbm_;
    cons_type di_;
    cons_type dj_;
};

template <typename DbmType, typename ContextType>
class add_cons_close_oper : public add_cons_close_oper_impl_<DbmType, ContextType> {
public:
    using add_cons_close_oper_impl_<DbmType, ContextType>::add_cons_close_oper_impl_;
};

}
adl_END_MAIN_MODULE

#endif //adl__oct__cpu__add_cons_close_oper__hpp__
