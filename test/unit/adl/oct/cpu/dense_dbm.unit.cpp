// $flisboac 2017-08-28
#include "adl_catch.hpp"
#include <sstream>
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/cons.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/cpu/seq_context.hpp"
#include "adl/oct/cpu/async_context.hpp"
#include "adl/oct/cpu/dense_dbm.hpp"

using namespace adl::oct;
using namespace adl::oct::cpu;
using namespace adl::literals;
using namespace adl::operators;
using namespace adl::dsl;

template <typename T>
static std::string dbm_to_string_(T const& dbm) {
    char const* sep = "\t";
    std::stringstream ss;
    ss << std::endl;
    for (auto i = dbm.first_var(); i < dbm.end_var(); ++i) {
        for (auto j = dbm.first_var(); j < dbm.end_var(); ++j) {
            ss << sep << "[" << i.to_index() << "," << j.to_index() << "=" << dbm.at(i, j) << "]";
            sep = " ";
        }
        sep = "\t";
        ss << std::endl;
    }
    return ss.str();
}

template <
    template <typename, typename, typename> class DbmClass,
    typename ContextType,
    typename ValueType,
    typename ValueLimits>
static void require_dbm_size_(
    DbmClass<ContextType, ValueType, ValueLimits> const& dbm,
    octdiff_var last_var
) {
    const auto end_var = last_var + 1;
    const auto var_size = end_var.to_index();
    INFO_REQUIRE( (dbm.size() == var_size), dbm.size(), var_size );
    INFO_REQUIRE( (dbm.last_var() == last_var), dbm.last_var().to_string(), last_var.to_string() );
    INFO_REQUIRE( (dbm.end_var() == end_var), dbm.end_var().to_string(), end_var.to_string() );
}

template <
    template <typename, typename, typename> class DbmClass,
    typename ContextType,
    typename ValueType,
    typename ValueLimits,
    typename CheckFunction,
    typename = std::enable_if_t<!std::is_arithmetic<CheckFunction>::value>>
static void require_dbm_values_(
    DbmClass<ContextType, ValueType, ValueLimits> const& dbm,
    CheckFunction check
) {
    for (auto i = dbm.first_var(); i < dbm.end_var(); ++i) {
        for (auto j = dbm.first_var(); j < dbm.end_var(); ++j) {
            octdiff_vexpr vexpr(i, j);
            auto value_at = dbm.at(i, j);
            auto value_oper = dbm[vexpr];
            INFO_REQUIRE(( value_at == value_oper ), value_at, value_oper, i.to_string(), j.to_string(), vexpr.to_string());
            INFO_REQUIRE(( check(value_at, i, j) ), value_at, i.to_string(), j.to_string());
        }
    }
}

template <
    template <typename, typename, typename> class DbmClass,
    typename ContextType,
    typename ValueType,
    typename ValueLimits,
    typename = std::enable_if_t<std::is_arithmetic<ValueType>::value>>
static void require_dbm_values_(
    DbmClass<ContextType, ValueType, ValueLimits> const& dbm,
    ValueType value = DbmClass<ContextType, ValueType, ValueLimits>::default_constant()
) {
    require_dbm_values_(dbm, [value](ValueType v, octdiff_var, octdiff_var) -> bool { return value == v; });
}

static void test_dbm_creation_by_size_(cpu::seq_context & seq_ctx) {
    constexpr auto size_var = 100_ov;
    constexpr auto last_var = -size_var;
    constexpr auto initial_value = 1.11;

    using constant_type_ = std::remove_const<decltype(initial_value)>::type;

    SECTION("Making DBM with fixed size and NO initial value") {
        auto dbm = seq_ctx.make_dbm<cpu::dense_dbm, constant_type_>(size_var);
        require_dbm_size_(dbm, last_var);
        require_dbm_values_(dbm, dbm.default_constant());
    }

    SECTION("Making DBM with fixed size and initial value") {
        // last_var here to showcase the fact that the DBM can be created with a negative variable. The last_var will
        // be the next normalized variable (that is, `to_octdiff(var).normalize().increment(2)`)
        auto dbm = seq_ctx.make_dbm<cpu::dense_dbm, constant_type_>(last_var, initial_value);
        require_dbm_size_(dbm, last_var);
        require_dbm_values_(dbm, initial_value);
    }
}

template <typename ValueType>
static void do_test_dbm_by_cons_(cpu::seq_context & seq_ctx, oct_cons<ValueType> cons, dbm_major major) {
    const auto xi = cons.xi();
    const auto xj = cons.xj();
    const auto c = cons.c();
    const auto c2 = c * 2;
    const auto split = cons.split();
    const auto di = split.di(), dj = split.dj();
    const auto last_var = -to_octdiff(xi.compare(xj) >= 0 ? xi : xj).normalize();
    const auto initial_value = c2 + 13;
    const oct_system<ValueType> system = { cons };
    const auto diff_system = system.to_counterpart();
    const auto dbm = seq_ctx.make_dbm<cpu::dense_dbm>(diff_system, initial_value, major);
    const std::size_t index_di = di.xi().to_index() * dbm.size() + di.xj().to_index();
    const std::size_t index_dj = dj.valid() ? dj.xi().to_index() * dbm.size() + dj.xj().to_index() : 0;

    auto test_dbm_value = [&](ValueType value, octdiff_var xi_, octdiff_var xj_) {
        octdiff_vexpr vexpr_(xi_, xj_);
        bool valid_index = (vexpr_ == to_identity(split.di()).to_vexpr() || vexpr_ == to_identity(split.dj()).to_vexpr());
        INFO("cons = " << cons.to_string()); // Not working, why?
        return valid_index
            ? (cons.unit() ? value == c2 : value == c)
            : value == initial_value;
    };

    auto test_diff_cons = [&](octdiff_cons<ValueType> cons_) {
        const std::size_t index = (dbm.major() == dbm_major::row)
            ? cons_.xi().to_index() * dbm.size() + cons_.xj().to_index()
            : cons_.xj().to_index() * dbm.size() + cons_.xi().to_index();
        INFO_REQUIRE(( dbm.at(cons_) == dbm.at(cons_.xi(), cons_.xj()) ),
            dbm.at(cons_), dbm.at(cons_.xi(), cons_.xj()), cons_.to_string(), cons_.xi().to_string(), cons_.xj().to_string());
        INFO_REQUIRE(( dbm.at(cons_) == dbm[cons_] ),
            dbm.at(cons_) , dbm[cons_], cons_.to_string());
        INFO_REQUIRE(( dbm.at(cons_) == dbm[index] ),
            dbm.at(cons_) , dbm[index], cons_.to_string(), index);
        INFO_REQUIRE(( dbm[cons_] == cons_.c() ),
            dbm[cons_], cons_.c(), cons_.to_string());
    };

    require_dbm_size_(dbm, last_var);
    require_dbm_values_(dbm, test_dbm_value);
    test_diff_cons(di);
    if (dj.valid()) {
        test_diff_cons(dj);
        INFO_REQUIRE(( dbm[di] == dbm[dj] ), dbm[di], dbm[dj], di.to_string(), dj.to_string());
    }
}

static void test_dbm_creation_by_conversion_(cpu::seq_context & seq_ctx) {
    constexpr auto xi = 1_ov;
    constexpr auto xj = 2_ov;
    constexpr auto dxi = xi.to_counterpart();
    constexpr auto dxj = xj.to_counterpart();
    constexpr auto c = 10.4;

    dbm_major majors[] = { dbm_major::row, dbm_major::col };

    for (auto major : majors) {
        std::string base_msg =
            std::string("With ")
            + (major == dbm_major::row ? "row" : "column")
            + "-major DBM, ";
        SECTION(base_msg + "testing constraint:  xi - xj <= c")      { do_test_dbm_by_cons_(seq_ctx,  xi - xj <= c, major); }
        SECTION(base_msg + "testing constraint:  xi + xj <= c")      { do_test_dbm_by_cons_(seq_ctx,  xi + xj <= c, major); }
        SECTION(base_msg + "testing from constraint: -xi - xj <= c") { do_test_dbm_by_cons_(seq_ctx, -xi - xj <= c, major); }
        SECTION(base_msg + "testing constraint:  xi <= c")           { do_test_dbm_by_cons_(seq_ctx,  xi <= c, major); }
        SECTION(base_msg + "testing constraint: -xi <= c")           { do_test_dbm_by_cons_(seq_ctx, -xi <= c, major); }
    }
}

TEST_CASE("unit:adl/oct/cpu/dense_dbm.hpp", "[unit][adl][adl/oct][adl/oct/cpu]") {
    auto seq_ctx = cpu::seq_context::make();
    test_dbm_creation_by_size_(seq_ctx);
    test_dbm_creation_by_conversion_(seq_ctx);
}
