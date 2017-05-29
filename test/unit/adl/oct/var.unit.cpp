// $flisboac 2017-04-23
#include "adl_catch.hpp"
#include "adl/assert.hpp"

#include "adl/oct/var.hpp"

#define MODULE_NAME_ "adl/oct/var"

#define OCT_VAR_CLASS_ adl::oct::oct_var
#define OCTDIFF_VAR_CLASS_ adl::oct::octdiff_var


//
// [[ FIXTURES ]]
//


namespace {

using namespace adl;
using namespace adl::oct;

template <typename VarType>
class var_test_ {
public:
    using var_id_traits = typename VarType::var_id_traits;
    using var_id_limits = typename VarType::var_id_limits;
    using var_id_type = typename VarType::var_id_type;
    using var_traits = typename VarType::var_traits;
    using var_type = typename VarType::var_type;
    using counterpart_var_type = typename VarType::counterpart_var_type;
    using counterpart_var_id_limits = typename var_id_limits::counterpart_var_id_limits;
    using counterpart_var_id_traits = typename var_id_traits::counterpart_var_id_traits;

    constexpr static const domain_space space = VarType::space;
    constexpr static const domain_space counterpart_space = VarType::counterpart_space;
    constexpr static const bool is_oct = space == domain_space::oct;
    constexpr static const bool is_counterpart_oct = counterpart_space == domain_space::oct;

public:
    virtual ~var_test_() = default;
    virtual void test_all();
    virtual void test_assertions();
    virtual void test_construction();
    virtual void test_properties();
    virtual void test_methods();
    virtual void test_conversions();
    virtual void test_operators();

protected:
    const var_id_type i_0 = var_id_limits::invalid_var_id;
    const var_id_type i_f = var_id_limits::first_var_id;
    const var_id_type i_F = is_oct ? -var_id_limits::first_var_id : var_id_limits::first_var_id + 1;
    const var_id_type i_l = var_id_limits::last_var_id;
    const var_id_type i_L = is_oct ? -var_id_limits::last_var_id : var_id_limits::last_var_id + 1;
    const var_id_type i_e = var_id_limits::end_var_id;
    const var_id_type i_E = is_oct ? -var_id_limits::end_var_id : var_id_limits::end_var_id + 1;
    const var_id_type i_r = var_id_limits::rend_var_id;
    const var_id_type i_p { 127 };
    const var_id_type i_n { -127 };
    const var_id_type i_p1 { (is_oct ? 3 : 5) };
    const var_id_type i_n1 { (is_oct ? -3 : 6) };
    const var_id_type i_p2 { (is_oct ? 4 : 7) };
    const var_id_type i_n2 { (is_oct ? -4 : 8) };
    const var_id_type i_p3 { 1 };
    const var_id_type i_n3 { (is_oct ? -1 : 2) };

    const var_type v_0 = var_type::invalid();
    const var_type v_f = var_type::first();
    const var_type v_F = var_type::first().negate();
    const var_type v_l = var_type::last();
    const var_type v_L = var_type::last().negate();
    const var_type v_e = var_type::end();
    const var_type v_E = var_type::end().negate();
    const var_type v_r = var_type::rend();
    const var_type v_default;
    const var_type v_p { i_p };
    const var_type v_n { i_n };
    const var_type v_p1 { i_p1 };
    const var_type v_n1 { i_n1 };
    const var_type v_p2 { i_p2 };
    const var_type v_n2 { i_n2 };
    const var_type v_p3 { i_p3 };
    const var_type v_n3 { i_n3 };

    const var_id_type ci_0 = counterpart_var_id_limits::invalid_var_id;
    const var_id_type ci_f = counterpart_var_id_limits::first_var_id;
    const var_id_type ci_F = is_counterpart_oct ? -counterpart_var_id_limits::first_var_id : counterpart_var_id_limits::first_var_id + 1;
    const var_id_type ci_l = counterpart_var_id_limits::last_var_id;
    const var_id_type ci_L = is_counterpart_oct ? -counterpart_var_id_limits::last_var_id : counterpart_var_id_limits::last_var_id + 1;
    const var_id_type ci_e = counterpart_var_id_limits::end_var_id;
    const var_id_type ci_E = is_counterpart_oct ? -counterpart_var_id_limits::end_var_id : counterpart_var_id_limits::end_var_id + 1;
    const var_id_type ci_r = counterpart_var_id_limits::rend_var_id;
    const var_id_type ci_p { 127 };
    const var_id_type ci_n { -127 };
    const var_id_type ci_p1 { (is_counterpart_oct ? 3 : 5) };
    const var_id_type ci_n1 { (is_counterpart_oct ? -3 : 6) };
    const var_id_type ci_p2 { (is_counterpart_oct ? 4 : 7) };
    const var_id_type ci_n2 { (is_counterpart_oct ? -4 : 8) };
    const var_id_type ci_p3 { 1 };
    const var_id_type ci_n3 { (is_counterpart_oct ? -1 : 2) };

    const counterpart_var_type cv_0 = counterpart_var_type::invalid();
    const counterpart_var_type cv_f = counterpart_var_type::first();
    const counterpart_var_type cv_F = counterpart_var_type::first().negate();
    const counterpart_var_type cv_l = counterpart_var_type::last();
    const counterpart_var_type cv_L = counterpart_var_type::last().negate();
    const counterpart_var_type cv_e = counterpart_var_type::end();
    const counterpart_var_type cv_E = counterpart_var_type::end().negate();
    const counterpart_var_type cv_r = counterpart_var_type::rend();
    const counterpart_var_type cv_default;
    const counterpart_var_type cv_p { ci_p };
    const counterpart_var_type cv_n { ci_n };
    const counterpart_var_type cv_p1 { ci_p1 };
    const counterpart_var_type cv_n1 { ci_n1 };
    const counterpart_var_type cv_p2 { ci_p2 };
    const counterpart_var_type cv_n2 { ci_n2 };
    const counterpart_var_type cv_p3 { ci_p3 };
    const counterpart_var_type cv_n3 { ci_n3 };
};

template <typename VarType>
class unnamed_var_test_ : public var_test_<VarType> {
private:
    using superclass_ = var_test_<VarType>;
public:
    virtual ~unnamed_var_test_() = default;
    virtual void test_assertions();
    virtual void test_properties();
    virtual void test_conversions();
};

} // <anonymous namespace>


//
// [[ TESTS ]]
//


TEST_CASE_METHOD(unnamed_var_test_<OCT_VAR_CLASS_>,
    adl_UNIT_TEST_NAME(MODULE_NAME_, OCT_VAR_CLASS_),
    adl_UNIT_TEST_FLAGS(MODULE_NAME_)
) {
    test_all();
}

TEST_CASE_METHOD(unnamed_var_test_<OCTDIFF_VAR_CLASS_>,
    adl_UNIT_TEST_NAME(MODULE_NAME_, OCTDIFF_VAR_CLASS_),
    adl_UNIT_TEST_FLAGS(MODULE_NAME_)
) {
    test_all();
}


//
// [[ IMPLEMENTATION ]]
//


template <typename VarType>
inline void var_test_<VarType>::test_all() {
    test_assertions();
    test_construction();
    test_properties();
    test_methods();
    test_conversions();
}

template <typename VarType>
inline void var_test_<VarType>::test_assertions() {
    using namespace adl;

    // Consistency/correctness
    adl_static_assert(space == var_type::space);
    adl_static_assert(counterpart_space == counterpart_var_type::space);

    // Type requirements
    assert::literal_class<var_type>();
    assert::literal_constructible<var_type>();
    assert::literal_constructible<var_type, long long int>();
    assert::literal_copyable<var_type>();
    assert::literal_moveable<var_type>();
    assert::literal_seamlessly_convertible<var_type, counterpart_var_type>();
    assert::literal_seamlessly_explicitly_convertible<var_type, var_id_type>();
}

template <typename VarType>
inline void var_test_<VarType>::test_construction() {

    SECTION("constructor(long long int)") {
        // NOTE: This assumes default_var_id_type is int. If it ever changes in the future, corrections must be done to
        // accomodate the new limits.
        REQUIRE_TRUE_NOTHROW( (var_type(std::numeric_limits<long long int>::max()).equals(v_0)) );

        REQUIRE_TRUE_NOTHROW( (v_p.id() == i_p) );
        REQUIRE_TRUE_NOTHROW( (is_oct ? v_n.id() == i_n : v_n.id() == i_0) );
        REQUIRE_TRUE_NOTHROW( (v_p1.id() == i_p1) );
        REQUIRE_TRUE_NOTHROW( (v_n1.id() == i_n1) );
        REQUIRE_TRUE_NOTHROW( (v_p2.id() == i_p2) );
        REQUIRE_TRUE_NOTHROW( (v_n2.id() == i_n2) );
        REQUIRE_TRUE_NOTHROW( (v_p3.id() == i_p3) );
        REQUIRE_TRUE_NOTHROW( (v_n3.id() == i_n3) );
    }

    SECTION("id_to_range(long long int)") {
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_0) == var_id_traits::id_to_range(i_0)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_f) == var_id_traits::id_to_range(i_f)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_F) == var_id_traits::id_to_range(i_F)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_l) == var_id_traits::id_to_range(i_l)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_L) == var_id_traits::id_to_range(i_L)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_e) == var_id_traits::id_to_range(i_e)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_E) == var_id_traits::id_to_range(i_E)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_r) == var_id_traits::id_to_range(i_r)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_range(i_p) == var_id_traits::id_to_range(i_p)) );
        REQUIRE_TRUE_NOTHROW( (is_counterpart_oct || var_type::id_to_range(i_n) == var_id_traits::id_to_range(i_n)) );
        REQUIRE_TRUE_NOTHROW( (is_oct || var_type::id_to_range(i_n) == var_id_traits::id_to_range(i_n)) );
    }

    SECTION("id_to_valid(long long int)") {
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_0) == var_id_traits::id_to_valid(i_0)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_f) == var_id_traits::id_to_valid(i_f)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_F) == var_id_traits::id_to_valid(i_F)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_l) == var_id_traits::id_to_valid(i_l)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_L) == var_id_traits::id_to_valid(i_L)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_e) == var_id_traits::id_to_valid(i_e)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_E) == var_id_traits::id_to_valid(i_E)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_r) == var_id_traits::id_to_valid(i_r)) );
        REQUIRE_TRUE_NOTHROW( (var_type::id_to_valid(i_p) == var_id_traits::id_to_valid(i_p)) );
        REQUIRE_TRUE_NOTHROW( (is_counterpart_oct || var_type::id_to_valid(i_n) == var_id_traits::id_to_valid(i_n)) );
        REQUIRE_TRUE_NOTHROW( (is_oct || var_type::id_to_valid(i_n) == var_id_traits::id_to_valid(i_n)) );
    }

    SECTION("from_range(long long int)") {
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_0).id() == var_type::id_to_range(i_0)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_f).id() == var_type::id_to_range(i_f)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_F).id() == var_type::id_to_range(i_F)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_l).id() == var_type::id_to_range(i_l)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_L).id() == var_type::id_to_range(i_L)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_e).id() == var_type::id_to_range(i_e)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_E).id() == var_type::id_to_range(i_E)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_r).id() == var_type::id_to_range(i_r)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_p).id() == var_type::id_to_range(i_p)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_range(i_n).id() == var_type::id_to_range(i_n)) );
    }

    SECTION("from_valid(long long int)") {
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_0).id() == var_type::id_to_valid(i_0)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_f).id() == var_type::id_to_valid(i_f)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_F).id() == var_type::id_to_valid(i_F)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_l).id() == var_type::id_to_valid(i_l)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_L).id() == var_type::id_to_valid(i_L)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_e).id() == var_type::id_to_valid(i_e)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_E).id() == var_type::id_to_valid(i_E)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_r).id() == var_type::id_to_valid(i_r)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_p).id() == var_type::id_to_valid(i_p)) );
        REQUIRE_TRUE_NOTHROW( (var_type::from_valid(i_n).id() == var_type::id_to_valid(i_n)) );
    }

    SECTION("\"Constants\"") {
        REQUIRE_TRUE_NOTHROW( (var_type::invalid().id() == i_0) );
        REQUIRE_TRUE_NOTHROW( (var_type::first().id() == i_f) );
        REQUIRE_TRUE_NOTHROW( (var_type::last().id() == i_l) );
        REQUIRE_TRUE_NOTHROW( (var_type::begin().id() == i_f) );
        REQUIRE_TRUE_NOTHROW( (var_type::end().id() == i_e) );
        REQUIRE_TRUE_NOTHROW( (var_type::rbegin().id() == i_l) );
        REQUIRE_TRUE_NOTHROW( (var_type::rend().id() == i_r) );
    }
}

template <typename VarType>
inline void var_test_<VarType>::test_properties() {
    // id() was already extensibly tested in test_construction()...

    SECTION("normal_id()") {
        REQUIRE_TRUE_NOTHROW( (v_0.normal_id() == var_id_traits::normalize_id(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.normal_id() == var_id_traits::normalize_id(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.normal_id() == var_id_traits::normalize_id(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.normal_id() == var_id_traits::normalize_id(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.normal_id() == var_id_traits::normalize_id(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.normal_id() == var_id_traits::normalize_id(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.normal_id() == var_id_traits::normalize_id(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.normal_id() == var_id_traits::normalize_id(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.normal_id() == var_id_traits::normalize_id(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.normal_id() == var_id_traits::normalize_id(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (is_oct || v_n.normal_id() == var_id_traits::normalize_id(i_n)) );
    }

    SECTION("oct_id()") {
        REQUIRE_TRUE_NOTHROW( (v_0.oct_id() == var_id_traits::id_to_normal_oct(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.oct_id() == var_id_traits::id_to_normal_oct(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.oct_id() == var_id_traits::id_to_normal_oct(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.oct_id() == var_id_traits::id_to_normal_oct(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.oct_id() == var_id_traits::id_to_normal_oct(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.oct_id() == var_id_traits::id_to_normal_oct(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.oct_id() == var_id_traits::id_to_normal_oct(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.oct_id() == var_id_traits::id_to_normal_oct(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.oct_id() == var_id_traits::id_to_normal_oct(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.oct_id() == var_id_traits::id_to_normal_oct(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (is_oct || v_n.oct_id() == var_id_traits::id_to_normal_oct(i_n)) );
    }

    SECTION("valid()") {
        REQUIRE_TRUE_NOTHROW( (v_0.valid() == var_id_traits::is_valid_id(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.valid() == var_id_traits::is_valid_id(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.valid() == var_id_traits::is_valid_id(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.valid() == var_id_traits::is_valid_id(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.valid() == var_id_traits::is_valid_id(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.valid() == var_id_traits::is_valid_id(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.valid() == var_id_traits::is_valid_id(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.valid() == var_id_traits::is_valid_id(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.valid() == var_id_traits::is_valid_id(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.valid() == var_id_traits::is_valid_id(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (v_p.valid() == var_id_traits::is_valid_id(i_p)) );
        REQUIRE_TRUE_NOTHROW( (v_n.valid() == var_id_traits::is_valid_id(i_n)) );
    }

    SECTION("sign()") {
        REQUIRE_TRUE_NOTHROW( (v_0.sign() == var_id_traits::id_sign(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.sign() == var_id_traits::id_sign(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.sign() == var_id_traits::id_sign(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.sign() == var_id_traits::id_sign(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.sign() == var_id_traits::id_sign(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.sign() == var_id_traits::id_sign(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.sign() == var_id_traits::id_sign(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.sign() == var_id_traits::id_sign(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.sign() == var_id_traits::id_sign(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.sign() == var_id_traits::id_sign(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (v_p.sign() == var_id_traits::id_sign(i_p)) );
        REQUIRE_TRUE_NOTHROW( (v_n.sign() == var_id_traits::id_sign(i_n)) );
    }

    SECTION("positive()") {
        REQUIRE_TRUE_NOTHROW( (v_0.positive() == var_id_traits::is_positive_id(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.positive() == var_id_traits::is_positive_id(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.positive() == var_id_traits::is_positive_id(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.positive() == var_id_traits::is_positive_id(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.positive() == var_id_traits::is_positive_id(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.positive() == var_id_traits::is_positive_id(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.positive() == var_id_traits::is_positive_id(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.positive() == var_id_traits::is_positive_id(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.positive() == var_id_traits::is_positive_id(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.positive() == var_id_traits::is_positive_id(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (v_p.positive() == var_id_traits::is_positive_id(i_p)) );
        REQUIRE_TRUE_NOTHROW( (v_n.positive() == var_id_traits::is_positive_id(i_n)) );
    }

    SECTION("negative()") {
        REQUIRE_TRUE_NOTHROW( (v_0.negative() == var_id_traits::is_negative_id(i_0)) );
        REQUIRE_TRUE_NOTHROW( (v_f.negative() == var_id_traits::is_negative_id(i_f)) );
        REQUIRE_TRUE_NOTHROW( (v_F.negative() == var_id_traits::is_negative_id(i_F)) );
        REQUIRE_TRUE_NOTHROW( (v_l.negative() == var_id_traits::is_negative_id(i_l)) );
        REQUIRE_TRUE_NOTHROW( (v_L.negative() == var_id_traits::is_negative_id(i_L)) );
        REQUIRE_TRUE_NOTHROW( (v_e.negative() == var_id_traits::is_negative_id(i_e)) );
        REQUIRE_TRUE_NOTHROW( (v_E.negative() == var_id_traits::is_negative_id(i_E)) );
        REQUIRE_TRUE_NOTHROW( (v_r.negative() == var_id_traits::is_negative_id(i_r)) );
        REQUIRE_TRUE_NOTHROW( (v_p1.negative() == var_id_traits::is_negative_id(i_p1)) );
        REQUIRE_TRUE_NOTHROW( (v_n1.negative() == var_id_traits::is_negative_id(i_n1)) );
        REQUIRE_TRUE_NOTHROW( (v_p.negative() == var_id_traits::is_negative_id(i_p)) );
        REQUIRE_TRUE_NOTHROW( (v_n.negative() == var_id_traits::is_negative_id(i_n)) );
    }
}

template <typename VarType>
inline void var_test_<VarType>::test_methods() {}

template <typename VarType>
inline void var_test_<VarType>::test_conversions() {}

template <typename VarType>
inline void var_test_<VarType>::test_operators() {}

template <typename VarType>
inline void unnamed_var_test_<VarType>::test_assertions() {
    superclass_::test_assertions();
    static_assert(std::is_same<bool const, decltype(VarType::named)>::value && !VarType::named,
        "The unnamed variable does not have the correct `static const bool named` value.");
}

template <typename VarType>
inline void unnamed_var_test_<VarType>::test_properties() {
    superclass_::test_properties();
}

template <typename VarType>
inline void unnamed_var_test_<VarType>::test_conversions() {
    superclass_::test_conversions();
}
