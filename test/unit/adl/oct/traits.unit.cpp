// $flisboac 2017-04-08
#include "adl_catch.hpp"
#include "adl/assert.hpp"
#include "adl/oct/traits.hpp"

#define FOR_OCT " [for domain_space::oct]"
#define FOR_OCTDIFF " [for domain_space::octdiff]"

#define FOR_POSITIVE_ID   " [for non-negated (positive) id]"
#define FOR_NEGATIVE_ID   " [for negated (negative) id]"
#define FOR_ID_GT_0       " [for id > 0]"
#define FOR_ID_LT_0       " [for id < 0]"
#define FOR_ID_ZERO       " [for id == 0 (the invalid id value)]"
#define FOR_NORMALIZED_ID " [for normalized (positive) id]"
#define FOR_VALID_ID      " [for valid id (is_valid_id)]"
#define FOR_INVALID_ID    " [for non-valid id (!is_valid_id)]"
#define FOR_INBOUND_ID    " [for id within bounds (is_id_in_range)]"
#define FOR_OUTBOUND_ID   " [for id outside bounds (is_id_in_range)]"
#define FOR_FIRST_ID      " [for id == first_var_id]"
#define FOR_LAST_ID       " [for id == last_var_id]"
#define FOR_END_ID        " [for id == end_var_id]"
#define FOR_STRINGP_NAME  " [for `char const*` name]"
#define FOR_STRINGV_NAME  " [for `string_view` name]"
#define FOR_STRING_NAME   " [for `std::string` name]"

namespace {

namespace imports_ {
    using namespace adl;
    using domain_space = adl::oct::domain_space;
    using oct_traits = adl::oct::var_id_traits<domain_space::oct>;
    using octdiff_traits = adl::oct::var_id_traits<domain_space::octdiff>;
    using var_id_type = oct::default_var_id_type;
    using oct_limits = oct_traits::var_id_limits;
    using octdiff_limits = octdiff_traits::var_id_limits;

    adl_static_assert((oct_traits::valid));
    adl_static_assert((octdiff_traits::valid));
}

template <typename Traits>
static inline void test_definitions_values_() {
    using limits = typename Traits::var_id_limits;
    constexpr int domain_space = (int) Traits::space;
    constexpr auto max_variables = limits::max_variables;
    constexpr auto invalid_var_id = limits::invalid_var_id;
    constexpr auto first_var_id = limits::first_var_id;
    constexpr auto last_var_id = limits::last_var_id;
    constexpr auto begin_var_id = limits::begin_var_id;
    constexpr auto end_var_id = limits::end_var_id;
    constexpr auto rbegin_var_id = limits::rbegin_var_id;
    constexpr auto rend_var_id = limits::rend_var_id;
    constexpr auto min_var_id = limits::min_var_id_value;
    constexpr auto max_var_id = limits::max_var_id_value;

    SECTION("checking values") {

        // To show, fail some test. Useful for debugging
        INFO(domain_space << "::max_variables = " << max_variables);
        INFO(domain_space << "::invalid_var_id = " << invalid_var_id);
        INFO(domain_space << "::first_var_id = " << first_var_id);
        INFO(domain_space << "::last_var_id = " << last_var_id);
        INFO(domain_space << "::begin_var_id = " << begin_var_id);
        INFO(domain_space << "::end_var_id = " << end_var_id);
        INFO(domain_space << "::rbegin_var_id = " << rbegin_var_id);
        INFO(domain_space << "::rend_var_id = " << rend_var_id);
        INFO(domain_space << "::min_var_id = " << min_var_id);
        INFO(domain_space << "::max_var_id = " << max_var_id);

        REQUIRE((limits::first_var_id - 1 == limits::rend_var_id));
        REQUIRE((limits::begin_var_id > 0));
        REQUIRE((limits::last_var_id > 0));
        REQUIRE((limits::rbegin_var_id == limits::last_var_id));
        REQUIRE((limits::rend_var_id == limits::invalid_var_id));
        REQUIRE((limits::end_var_id <= limits::max_variables + 1));
    }
}

}

TEST_CASE("adl/oct/traits.hpp:var_id", "[adl][adl/oct][adl/oct/traits]") {
    using namespace imports_;

    constexpr var_id_type ov_0 = 0,
        ov_p = 1,
        ov_n = -1,
        ov_p1 = 3,
        ov_n1 = -3,
        ov_p2 = 4,
        ov_n2 = -4,
        ov_p3 = 1,
        ov_n3 = -1,
        ov_L = -oct_limits::last_var_id, // conceptually, this is the last value, non-normalized
        ov_e = oct_limits::end_var_id,
        ov_E = -oct_limits::end_var_id, // conceptually, end_var_id + 1
        ov_f = oct_limits::first_var_id,
        ov_F = -oct_limits::first_var_id,
        ov_l = oct_limits::last_var_id;
    constexpr var_id_type odv_0 = 0,
        odv_p = 1,
        odv_n = -1,
        odv_p1 = 5,
        odv_n1 = 6,
        odv_p2 = 7,
        odv_n2 = 8,
        odv_p3 = 1,
        odv_n3 = 2,
        odv_L = octdiff_limits::last_var_id + 1,
        odv_e = octdiff_limits::end_var_id,
        odv_E = octdiff_limits::end_var_id + 1,
        odv_f = octdiff_limits::first_var_id,
        odv_F = octdiff_limits::first_var_id + 1,
        odv_l = octdiff_limits::last_var_id;

    constexpr const char *const ov_p1_name = "x3";
    constexpr const char *const ov_n1_name = "-x3";
    constexpr const char *const ov_x_name = "-xl";
    constexpr const string_view ov_p1_vname(ov_p1_name);
    constexpr const string_view ov_n1_vname(ov_n1_name);
    constexpr const string_view ov_x_vname(ov_x_name);
    const std::string ov_p1_sname(ov_p1_name);
    const std::string ov_n1_sname(ov_n1_name);
    const std::string ov_x_sname(ov_x_name);

    constexpr size_t ov_p1_index = ov_p1 - 1;
    constexpr size_t ov_n1_index = ov_p1 - 1; // it is calculated from the normalized id!
    constexpr size_t odv_p1_index = odv_p1 - 1;
    constexpr size_t odv_n1_index = odv_n1 - 1;

    test_definitions_values_<oct_traits>();
    test_definitions_values_<octdiff_traits>();
    REQUIRE( (octdiff_limits::max_variables == (oct_limits::max_variables * 2)) );
    REQUIRE( (oct_limits::last_var_id + 1 == oct_limits::end_var_id) );
    REQUIRE( (octdiff_limits::last_var_id + 2 == octdiff_limits::end_var_id) );

    //
    // [ static property functions ]
    //

    SECTION("is_valid_id(id) [must be within bounds of the valid ids limits, excluding invalids and ends]") {

        adl_static_assert( (oct_traits::is_valid_id(0), true) );
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_p)), FOR_ID_GT_0 FOR_INBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_n)), FOR_ID_LT_0 FOR_INBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_f)), FOR_FIRST_ID FOR_NORMALIZED_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_F)), FOR_FIRST_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_l)), FOR_LAST_ID FOR_NORMALIZED_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_valid_id(ov_L)), FOR_LAST_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_valid_id(ov_0)), FOR_ID_ZERO FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_valid_id(ov_e)), FOR_END_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_valid_id(ov_E)), FOR_OUTBOUND_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::is_valid_id(0), true) );
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_valid_id(odv_p)), FOR_ID_GT_0 FOR_INBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_valid_id(odv_f)), FOR_FIRST_ID FOR_NORMALIZED_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_valid_id(odv_F)), FOR_FIRST_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_valid_id(odv_l)), FOR_LAST_ID FOR_NORMALIZED_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_valid_id(odv_L)), FOR_LAST_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_valid_id(odv_0)), FOR_ID_ZERO FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_valid_id(odv_n)), FOR_ID_LT_0 FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_valid_id(odv_e)), FOR_END_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_valid_id(odv_E)), FOR_OUTBOUND_ID FOR_OCTDIFF);
    }

    SECTION("is_id_in_range(id) [should be true for id == end, id == invalid and first <= id <= last]") {

        adl_static_assert( (oct_traits::is_id_in_range(0), true) );
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(0)),    FOR_ID_ZERO FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_f)), FOR_FIRST_ID FOR_NORMALIZED_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_F)), FOR_FIRST_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_l)), FOR_LAST_ID FOR_NORMALIZED_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_L)), FOR_LAST_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_p1)), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_n1)), FOR_NEGATIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_id_in_range(ov_e)), FOR_END_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_id_in_range(ov_E)), FOR_OUTBOUND_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::is_id_in_range(0), true) );
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(0)),     FOR_ID_ZERO FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_f)), FOR_FIRST_ID FOR_NORMALIZED_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_F)), FOR_FIRST_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_l)), FOR_LAST_ID FOR_NORMALIZED_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_L)), FOR_LAST_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_p1)), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_n1)), FOR_NEGATIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_id_in_range(odv_e)), FOR_END_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_id_in_range(odv_n)), FOR_ID_LT_0 FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_id_in_range(odv_E)), FOR_OUTBOUND_ID FOR_OCTDIFF);
    }

    SECTION("is_positive_id(id) [should be true if id refers to a non-negated variable]") {

        adl_static_assert( (oct_traits::is_positive_id(0), true) );
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_positive_id(ov_p)), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_positive_id(ov_n)), FOR_NEGATIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_positive_id(ov_p1)), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_positive_id(ov_n1)), FOR_NEGATIVE_ID FOR_OCT);

        // octdiff cannot receive negative values for ids, because it's outside its working range
        adl_static_assert( (octdiff_traits::is_positive_id(0), true) );
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_positive_id(odv_p)), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_positive_id(odv_p1)), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_positive_id(odv_n1)), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("is_negative_id(id) [should be true if id refers to a negated variable]") {

        adl_static_assert( (oct_traits::is_negative_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_negative_id(ov_p)), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_negative_id(ov_n)), FOR_NEGATIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (!oct_traits::is_negative_id(ov_p1)), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( ( oct_traits::is_negative_id(ov_n1)), FOR_NEGATIVE_ID FOR_OCT);

        // octdiff cannot receive negative values for ids, because it's outside its working range
        adl_static_assert( (octdiff_traits::is_negative_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_negative_id(odv_p)), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (!octdiff_traits::is_negative_id(odv_p1)), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( ( octdiff_traits::is_negative_id(odv_n1)), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    //
    // [ static functions ]
    //

    SECTION("normalize_id(id) [should return the positive (non-negated) version of the variable id]") {

        adl_static_assert( (oct_traits::normalize_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::normalize_id(0) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::normalize_id(ov_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::normalize_id(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::normalize_id(ov_p1) == ov_p1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::normalize_id(ov_n1) == ov_p1), FOR_NEGATIVE_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::normalize_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(odv_e) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(odv_E) == 0), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(odv_p1) == odv_p1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::normalize_id(odv_n1) == odv_p1), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("negate_id(id) [returns the variable's opposite occurrence (negated)]") {

        adl_static_assert( (oct_traits::negate_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::negate_id(0) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::negate_id(ov_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::negate_id(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::negate_id(ov_p1) == ov_n1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::negate_id(ov_n1) == ov_p1), FOR_NEGATIVE_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::negate_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(odv_e) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(odv_E) == 0), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(odv_p1) == odv_n1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::negate_id(odv_n1) == odv_p1), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("increment_id(id) [should increment the normalized value of id, resulting in a positive variable]") {

        adl_static_assert( (oct_traits::increment_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::increment_id(0) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::increment_id(ov_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::increment_id(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::increment_id(ov_p1) == ov_p2), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::increment_id(ov_n1) == ov_p2), FOR_NEGATIVE_ID FOR_OCT);
        // TODO Increment with offset != 1

        adl_static_assert( (octdiff_traits::increment_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(odv_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(odv_E) == 0), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(odv_p1) == odv_p2), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::increment_id(odv_n1) == odv_p2), FOR_NEGATIVE_ID FOR_OCTDIFF);
        // TODO Increment with offset != 1
    }

    SECTION("decrement_id(id) [should decrement the normalized value of id, resulting in a positive variable]") {

        adl_static_assert( (oct_traits::decrement_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::decrement_id(0) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::decrement_id(ov_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::decrement_id(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::decrement_id(ov_p2) == ov_p1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::decrement_id(ov_n2) == ov_p1), FOR_NEGATIVE_ID FOR_OCT);
        // TODO Decrement with offset != 1

        adl_static_assert( (octdiff_traits::decrement_id(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(odv_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(odv_E) == 0), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(odv_p2) == odv_p1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::decrement_id(odv_n2) == odv_p1), FOR_NEGATIVE_ID FOR_OCTDIFF);
        // TODO Decrement with offset != 1
    }

    SECTION("id_equals(a, b) [should compare ids a and b, considering negated-ness, positive ids before]") {

        adl_static_assert( (oct_traits::id_equals(0, 0), true) );
        // TODO Implementation

        adl_static_assert( (octdiff_traits::id_equals(0, 0), true) );
        // TODO Implementation
    }

    SECTION("id_compare(a, b) [should compare ids a and b, considering negated-ness, positive ids before]") {
        // <invalids>, <positive, negative>..., <end>
        adl_static_assert( (oct_traits::id_compare(0, 0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_p1) == 0), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_n1) == 0), FOR_OCT)
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_0, ov_p1) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_0, ov_n1) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_E, ov_p1) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_E, ov_n1) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_e) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_e) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_n1) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_p2) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_n2) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_p2) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_n2) == -1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_0) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_0) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p1, ov_E) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_E) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_e, ov_p1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_e, ov_n1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n1, ov_p1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p2, ov_p1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_p2, ov_n1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n2, ov_p1) == 1), FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_compare(ov_n2, ov_n1) == 1), FOR_OCT);

        adl_static_assert( (octdiff_traits::id_compare(0, 0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_p1) == 0), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_n1) == 0), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_0, odv_p1) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_0, odv_n1) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_E, odv_p1) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_E, odv_n1) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_e) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_e) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_n1) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_p2) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_n2) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_p2) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_n2) == -1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_0) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_0) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_E) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_E) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_p1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_n1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_p1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p2, odv_p1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_p2, odv_n1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n2, odv_p1) == 1), FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_compare(odv_n2, odv_n1) == 1), FOR_OCTDIFF);
    }

    SECTION("id_sign(id) [should return -1 if id is negated or 1 if id is non-negated, else return 0]") {

        adl_static_assert( (oct_traits::id_sign(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(0) == 0), FOR_ID_ZERO FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_e) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_p) == 1), FOR_ID_LT_0 FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_n) == -1), FOR_ID_GT_0 FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_p1) == 1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_sign(ov_n1) == -1), FOR_NEGATIVE_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::id_sign(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(odv_e) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(odv_E) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(odv_p1) == 1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_sign(odv_n1) == -1), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("id_sign_format(id) [should return the variable's default sign format") {

        adl_static_assert( (oct_traits::id_sign_format(0), true) );
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(0)     && string_view(oct_traits::id_sign_format(0)).empty()), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_e)  && string_view(oct_traits::id_sign_format(ov_e)).empty()), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_E)  && string_view(oct_traits::id_sign_format(ov_E)).empty()), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_p1) && string_view(oct_traits::id_sign_format(ov_p1)) == oct_limits::positive_var_name_format), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_n1) && string_view(oct_traits::id_sign_format(ov_n1)) == oct_limits::negative_var_name_format), FOR_NEGATIVE_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::id_sign_format(0), true) );
        REQUIRE_SECTION_NOTHROW( (nullptr != octdiff_traits::id_sign_format(0)     && string_view(octdiff_traits::id_sign_format(0)).empty()), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_e) && string_view(octdiff_traits::id_sign_format(odv_e)).empty()), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_E) && string_view(octdiff_traits::id_sign_format(odv_E)).empty()), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_n) && string_view(octdiff_traits::id_sign_format(odv_n)).empty()), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(odv_p1)    && string_view(octdiff_traits::id_sign_format(ov_p1)) == octdiff_limits::positive_var_name_format), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (nullptr != oct_traits::id_sign_format(odv_n1)    && string_view(octdiff_traits::id_sign_format(ov_n1)) == octdiff_limits::negative_var_name_format), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    //
    // [ static conversion functions ]
    //

    SECTION("name_to_id(name) ["
        "must return valid variable IDs for partially numeric octagon-only variable names, "
        "or the invalid variable ID otherwise]"
    ) {
        adl_static_assert( (oct_traits::name_to_id(ov_p1_name), true) );
        adl_static_assert( (oct_traits::name_to_id(ov_p1_vname), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_p1_name) == ov_p1), FOR_POSITIVE_ID FOR_STRINGP_NAME FOR_OCT );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_n1_name) == ov_n1), FOR_NEGATIVE_ID FOR_STRINGP_NAME FOR_OCT );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_x_name) == oct_limits::invalid_var_id), FOR_INVALID_ID FOR_STRINGP_NAME FOR_OCT );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_p1_vname) == ov_p1), FOR_POSITIVE_ID FOR_STRINGV_NAME FOR_OCT );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_n1_vname) == ov_n1), FOR_NEGATIVE_ID FOR_STRINGV_NAME FOR_OCT );
        REQUIRE_SECTION_NOTHROW( (oct_traits::name_to_id(ov_x_vname) == oct_limits::invalid_var_id), FOR_INVALID_ID FOR_STRINGV_NAME FOR_OCT );
        REQUIRE_SECTION( (oct_traits::name_to_id(ov_p1_sname) == ov_p1), FOR_POSITIVE_ID FOR_STRING_NAME FOR_OCT );
        REQUIRE_SECTION( (oct_traits::name_to_id(ov_n1_sname) == ov_n1), FOR_NEGATIVE_ID FOR_STRING_NAME FOR_OCT );
        REQUIRE_SECTION( (oct_traits::name_to_id(ov_x_sname) == oct_limits::invalid_var_id), FOR_INVALID_ID FOR_STRING_NAME FOR_OCT );
        
        // NOTE Conversion from octdiff variable name to octdiff variable ID is prohibited atm.
    }

    SECTION("arithmetic_to_range(value) ["
        "must return a valid variable ID if the value passed is within the variable ID type's bounds (e.g. first-end), "
        "else invalid_var_id"
    ) {
        adl_static_assert( (oct_traits::arithmetic_to_range(0), true) );
        // TODO Implementation
        
        adl_static_assert( (octdiff_traits::arithmetic_to_range(0), true) );
        // TODO Implementation
    }

    SECTION("arithmetic_to_valid(value) ["
        "must return a valid variable ID if the value passed is a valid variable ID (e.g. first-last), "
        "else invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::arithmetic_to_valid(0), true) );
        // TODO Implementation
        
        adl_static_assert( (octdiff_traits::arithmetic_to_valid(0), true) );
        // TODO Implementation
    }

    SECTION("id_to_arithmetic(id) ["
        "should return the id only if it's within bounds, "
        "else return default-constructed value]"
    ) {

        constexpr auto fallback_value_ = 9999.9f;

        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_arithmetic(ov_0, fallback_value_) == oct_limits::invalid_var_id), FOR_ID_ZERO FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_arithmetic(ov_E, fallback_value_) == fallback_value_), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_arithmetic(ov_e, fallback_value_) == ov_e), FOR_END_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_arithmetic(ov_p1, fallback_value_) == ov_p1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_arithmetic(ov_n1, fallback_value_) == ov_n1), FOR_NEGATIVE_ID FOR_OCT);

        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_0, fallback_value_) == octdiff_limits::invalid_var_id), FOR_ID_ZERO FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_E, fallback_value_) == fallback_value_), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_n, fallback_value_) == fallback_value_), FOR_ID_LT_0 FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_e, fallback_value_) == odv_e), FOR_END_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_p1, fallback_value_) == odv_p1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_n1, fallback_value_) == odv_n1), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("id_to_range(id) ["
        "must return the provided variable ID if the ID itself is a variable ID within bounds (e.g. first-end), "
        "else return invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::id_to_range(0), true) );
        // TODO Implementation
        
        adl_static_assert( (octdiff_traits::id_to_range(0), true) );
        // TODO Implementation
    }

    SECTION("id_to_valid(id) ["
        "must return the provided variable ID if the ID itself is a valid variable ID (e.g. first-last), "
        "else return invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::id_to_valid(0), true) );
        // TODO Implementation
        
        adl_static_assert( (octdiff_traits::id_to_valid(0), true) );
        // TODO Implementation
    }

    SECTION("id_to_index(id) [should return a size_t value suitable for indexing a memory region (e.g. array)]") {
        // invalid values result in undefined behaviour

        adl_static_assert( (oct_traits::id_to_index(0), true) );
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_index(ov_p1) == ov_p1_index), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_index(ov_n1) == ov_n1_index), FOR_NEGATIVE_ID FOR_OCT);

        adl_static_assert( (octdiff_traits::id_to_index(0), true) );
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_index(odv_p1) == odv_p1_index), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_index(odv_n1) == odv_n1_index), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("id_to_counterpart(id) [should return id's counterpart variable in the opposite domain]") {

        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(0) == 0), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_E) == 0), FOR_OUTBOUND_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_e) == odv_e), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_p1) == odv_p1), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_n1) == odv_n1), FOR_NEGATIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_p3) == odv_p3), FOR_POSITIVE_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (oct_traits::id_to_counterpart(ov_n3) == odv_n3), FOR_NEGATIVE_ID FOR_OCT);

        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(0) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_E) == 0), FOR_OUTBOUND_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n) == 0), FOR_INVALID_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_e) == ov_e), FOR_INVALID_ID FOR_OCT);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_p1) == ov_p1), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n1) == ov_n1), FOR_NEGATIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_p3) == ov_p3), FOR_POSITIVE_ID FOR_OCTDIFF);
        REQUIRE_SECTION_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n3) == ov_n3), FOR_NEGATIVE_ID FOR_OCTDIFF);
    }

    SECTION("id_to_normal_oct(id) [should return a normalized oct-space variable ID]") {

        adl_static_assert( (oct_traits::id_to_normal_oct(0), true) );
        // TODO Implementation

        adl_static_assert( (octdiff_traits::id_to_normal_oct(0), true) );
        // TODO Implementation
    }

    SECTION("id_to_name(id) [should return a name representation of a variable ID, considering the sign]") {

        // TODO Implementation
    }
}