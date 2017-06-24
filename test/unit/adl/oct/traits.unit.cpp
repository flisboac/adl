// $flisboac 2017-04-08
#include "adl_catch.hpp"
#include "adl/assert.hpp"
#include "adl/oct/traits.hpp"

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

    constexpr static const var_id_type
        ov_0 = oct_limits::invalid_var_id,
        ov_p = 127,
        ov_n = -127,
        ov_p1 = 3,
        ov_n1 = -3,
        ov_p2 = 4,
        ov_n2 = -4,
        ov_p3 = 1,
        ov_n3 = -1,
        ov_L = -oct_limits::last_var_id, // conceptually, this is the last value, non-normalized
        ov_e = oct_limits::end_var_id,
        ov_E = -oct_limits::end_var_id, // conceptually, end_var_id + 1
        ov_r = oct_limits::rend_var_id,
        ov_f = oct_limits::first_var_id,
        ov_F = -oct_limits::first_var_id,
        ov_l = oct_limits::last_var_id;
    constexpr static const var_id_type
        odv_0 = octdiff_limits::invalid_var_id,
        odv_p = 127,
        odv_n = -127,
        odv_p1 = 5,
        odv_n1 = 6,
        odv_p2 = 7,
        odv_n2 = 8,
        odv_p3 = 1,
        odv_n3 = 2,
        odv_L = octdiff_limits::last_var_id + 1,
        odv_e = octdiff_limits::end_var_id,
        odv_E = octdiff_limits::end_var_id + 1,
        odv_r = oct_limits::rend_var_id,
        odv_f = octdiff_limits::first_var_id,
        odv_F = octdiff_limits::first_var_id + 1,
        odv_l = octdiff_limits::last_var_id;

    constexpr static const char *const ov_p1_name = "x3";
    constexpr static const char *const ov_n1_name = "-x3";
    constexpr static const char *const ov_x_name = "-xl";
    constexpr static const auto ov_p1_vname = string_view(ov_p1_name);
    constexpr static const auto ov_n1_vname = string_view(ov_n1_name);
    constexpr static const auto ov_x_vname = string_view(ov_x_name);
    static const auto ov_p1_sname = std::string(ov_p1_name);
    static const auto ov_n1_sname = std::string(ov_n1_name);
    static const auto ov_x_sname = std::string(ov_x_name);

    constexpr static const char *const odv_p1_name = "x3__pos";
    constexpr static const char *const odv_n1_name = "x3__neg";
    static const auto odv_p1_sname = std::string(odv_p1_name);
    static const auto odv_n1_sname = std::string(odv_n1_name);

    constexpr static const size_t odv_p1_index = odv_p1 - 1;
    constexpr static const size_t odv_n1_index = odv_n1 - 1;
    constexpr static const size_t ov_p1_index = odv_p1_index;
    constexpr static const size_t ov_n1_index = odv_n1_index;
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

TEST_CASE("unit:adl/oct/traits:var_id", "[unit][adl][adl/oct][adl/oct/traits]") {
    using namespace imports_;

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
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_p)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_n)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_f)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_F)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_l)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_valid_id(ov_L)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_valid_id(ov_0)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_valid_id(ov_e)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_valid_id(ov_r)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_valid_id(ov_E)) );

        adl_static_assert( (octdiff_traits::is_valid_id(0), true) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_valid_id(odv_p)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_valid_id(odv_f)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_valid_id(odv_F)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_valid_id(odv_l)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_valid_id(odv_L)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_valid_id(odv_0)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_valid_id(odv_n)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_valid_id(odv_e)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_valid_id(odv_r)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_valid_id(odv_E)) );
    }

    SECTION("is_id_in_range(id) [should be true for id == end, id == invalid and first <= id <= last]") {

        adl_static_assert( (oct_traits::is_id_in_range(0), true) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_0)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_f)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_F)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_l)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_L)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_p1)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_n1)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_e)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_id_in_range(ov_r)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_id_in_range(ov_E)) );

        adl_static_assert( (octdiff_traits::is_id_in_range(0), true) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_0)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_f)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_F)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_l)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_L)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_p1)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_n1)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_e)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_id_in_range(odv_r)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_id_in_range(odv_n)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_id_in_range(odv_E)) );
    }

    SECTION("is_positive_id(id) [should be true if id refers to a non-negated variable]") {

        adl_static_assert( (oct_traits::is_positive_id(0), true) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_positive_id(ov_p1)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_n1)) );

        // octdiff cannot receive negative values for ids, because it's outside its working range
        adl_static_assert( (octdiff_traits::is_positive_id(0), true) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_positive_id(odv_p1)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_n1)) );

        #if 0
            // NOTE If in the future is_positive_id starts to accept non-valid in-range values in the future,
            the following requires must be included:
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_0)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_e)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_r)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_E)) );
            REQUIRE_TRUE_NOTHROW( ( oct_traits::is_positive_id(ov_p)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_positive_id(ov_n)) );

            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_0)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_e)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_r)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_E)) );
            REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_positive_id(odv_p)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_positive_id(odv_n)) );
        #endif
    }

    SECTION("is_negative_id(id) [should be true if id refers to a negated variable]") {

        adl_static_assert( (oct_traits::is_negative_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_p1)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::is_negative_id(ov_n1)) );

        // octdiff cannot receive negative values for ids, because it's outside its working range
        adl_static_assert( (octdiff_traits::is_negative_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_p1)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::is_negative_id(odv_n1)) );

        #if 0
            // NOTE If in the future is_negative_id starts to accept non-valid in-range values in the future,
            // the following requires must be included:
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_0)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_e)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_r)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_E)) );
            REQUIRE_TRUE_NOTHROW( (!oct_traits::is_negative_id(ov_p)) );
            REQUIRE_TRUE_NOTHROW( ( oct_traits::is_negative_id(ov_n)) );

            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_0)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_e)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_r)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_E)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_p)) );
            REQUIRE_TRUE_NOTHROW( (!octdiff_traits::is_negative_id(odv_n)) );
        #endif
    }

    //
    // [ static functions ]
    //

    SECTION("normalize_id(id) [should return the positive (non-negated) version of the variable id]") {

        adl_static_assert( (oct_traits::normalize_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_r) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::normalize_id(ov_n1) == ov_p1) );

        adl_static_assert( (octdiff_traits::normalize_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_r) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::normalize_id(odv_n1) == odv_p1) );
    }

    SECTION("negate_id(id) [returns the variable's opposite occurrence (negated)]") {

        adl_static_assert( (oct_traits::negate_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::negate_id(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::negate_id(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::negate_id(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::negate_id(ov_p1) == ov_n1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::negate_id(ov_n1) == ov_p1) );

        adl_static_assert( (octdiff_traits::negate_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_p1) == odv_n1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::negate_id(odv_n1) == odv_p1) );
    }

    SECTION("increment_id(id) [should increment the normalized value of id, resulting in a positive variable]") {

        adl_static_assert( (oct_traits::increment_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_p1) == ov_p2) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_n1) == ov_p2) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_p3, 2) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::increment_id(ov_n3, 2) == ov_p1) );

        adl_static_assert( (octdiff_traits::increment_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_p1) == odv_p2) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_n1) == odv_p2) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_p3, 2) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::increment_id(odv_n3, 2) == odv_p1) );
    }

    SECTION("decrement_id(id) [should decrement the normalized value of id, resulting in a positive variable]") {

        adl_static_assert( (oct_traits::decrement_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_p2) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_n2) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_p1, 2) == ov_p3) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::decrement_id(ov_n1, 2) == ov_p3) );

        adl_static_assert( (octdiff_traits::decrement_id(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_p2) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_n2) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_p1, 2) == odv_p3) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::decrement_id(odv_n1, 2) == odv_p3) );
    }

    SECTION("id_equals(a, b) [should compare ids a and b, considering negated-ness, positive ids before]") {

        adl_static_assert( (oct_traits::id_equals(0, 0), true) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::id_equals(ov_0, ov_0)) ); // The oddballs
        REQUIRE_TRUE_NOTHROW( ( oct_traits::id_equals(ov_e, ov_e)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::id_equals(ov_r, ov_r)) );
        REQUIRE_TRUE_NOTHROW( ( oct_traits::id_equals(ov_p1, ov_p1)) ); // For the two occurrences of a variable
        REQUIRE_TRUE_NOTHROW( ( oct_traits::id_equals(ov_n1, ov_n1)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_p1, ov_n1)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_n1, ov_p1)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_p1, ov_p2)) ); // For the two occurrences of two consecutive variables, 1-2
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_p1, ov_n2)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_n1, ov_p2)) );
        REQUIRE_TRUE_NOTHROW( (!oct_traits::id_equals(ov_n1, ov_n2)) );

        adl_static_assert( (octdiff_traits::id_equals(0, 0), true) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::id_equals(odv_0, odv_0)) ); // The oddballs
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::id_equals(odv_e, odv_e)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::id_equals(odv_r, odv_r)) );
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::id_equals(odv_p1, odv_p1)) ); // For the two occurrences of a variable
        REQUIRE_TRUE_NOTHROW( ( octdiff_traits::id_equals(odv_n1, odv_n1)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_p1, odv_n1)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_n1, odv_p1)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_p1, odv_p2)) ); // For the two occurrences of two consecutive variables, 1-2
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_p1, odv_n2)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_n1, odv_p2)) );
        REQUIRE_TRUE_NOTHROW( (!octdiff_traits::id_equals(odv_n1, odv_n2)) );
    }

    SECTION("id_compare(a, b) [should compare ids a and b, considering negated-ness, positive ids before]") {
        // <invalids>, <positive, negative>..., <end>
        adl_static_assert( (oct_traits::id_compare(0, 0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_p1) == 0) ); // For the two occurrences of a variable
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_n1) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_n1) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_p2) < 0) ); // For the two occurrences of two consecutive variables, 1-2
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_n2) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_p2) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_n2) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p2, ov_p1) > 0) ); // For the two occurrences of two consecutive variables, 2-1
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p2, ov_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n2, ov_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n2, ov_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_e, ov_e) == 0) ); // For end value, any valid variable < end value
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_e, ov_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_e, ov_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_e) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_e) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_r, ov_r) == 0) ); // For rear end value, any valid variable > rear end value
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_r, ov_p1) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_r, ov_n1) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_p1, ov_r) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_n1, ov_r) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_e, ov_r) > 0) ); // end_value > rend_value
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_compare(ov_r, ov_e) < 0) );
        // NOTE Comparisons between invalid_var_id and any other value don't make sense.


        adl_static_assert( (octdiff_traits::id_compare(0, 0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_p1) == 0) ); // For the two occurrences of a variable
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_n1) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_n1) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_p2) < 0) ); // For the two occurrences of two consecutive variables, 1-2
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_n2) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_p2) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_n2) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p2, odv_p1) > 0) ); // For the two occurrences of two consecutive variables, 2-1
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p2, odv_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n2, odv_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n2, odv_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_e) == 0) ); // For end value, any valid variable < end value
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_n1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_e) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_e) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_r, odv_r) == 0) ); // For rear end value, any valid variable > rear end value
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_r, odv_p1) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_r, odv_n1) < 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_p1, odv_r) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_n1, odv_r) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_e, odv_r) > 0) ); // end_value > rend_value
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_compare(odv_r, odv_e) < 0) );
        // NOTE Comparisons between invalid_var_id and any other value don't make sense.
    }

    SECTION("id_sign(id) [should return -1 if id is negated or 1 if id is non-negated, else return 0]") {

        adl_static_assert( (oct_traits::id_sign(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_0) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_e) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_r) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_E) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_p) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_n) < 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_sign(ov_n1) < 0) );

        adl_static_assert( (octdiff_traits::id_sign(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_0) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_e) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_r) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_E) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_n) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_p1) > 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_sign(odv_n1) < 0) );
    }

    SECTION("id_sign_format(id) [should return the variable's default sign format") {

        adl_static_assert( (oct_traits::id_sign_format(0), true) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_0)  && string_view(oct_traits::id_sign_format(ov_0)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_e)  && string_view(oct_traits::id_sign_format(ov_e)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_r)  && string_view(oct_traits::id_sign_format(ov_r)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_E)  && string_view(oct_traits::id_sign_format(ov_E)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_p1) && string_view(oct_traits::id_sign_format(ov_p1)) == oct_limits::positive_var_name_format) );
        REQUIRE_TRUE_NOTHROW( (nullptr != oct_traits::id_sign_format(ov_n1) && string_view(oct_traits::id_sign_format(ov_n1)) == oct_limits::negative_var_name_format) );

        adl_static_assert( (octdiff_traits::id_sign_format(0), true) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_0)  && string_view(octdiff_traits::id_sign_format(odv_0)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_e)  && string_view(octdiff_traits::id_sign_format(odv_e)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_r)  && string_view(octdiff_traits::id_sign_format(odv_r)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_E)  && string_view(octdiff_traits::id_sign_format(odv_E)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_n)  && string_view(octdiff_traits::id_sign_format(odv_n)).empty()) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_p1) && string_view(octdiff_traits::id_sign_format(odv_p1)) == octdiff_limits::positive_var_name_format) );
        REQUIRE_TRUE_NOTHROW( (nullptr != octdiff_traits::id_sign_format(odv_n1) && string_view(octdiff_traits::id_sign_format(odv_n1)) == octdiff_limits::negative_var_name_format) );
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
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_p1_name) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_n1_name) == ov_n1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_x_name) == oct_limits::invalid_var_id) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_p1_vname) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_n1_vname) == ov_n1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::name_to_id(ov_x_vname) == oct_limits::invalid_var_id) );
        REQUIRE( (oct_traits::name_to_id(ov_p1_sname) == ov_p1) );
        REQUIRE( (oct_traits::name_to_id(ov_n1_sname) == ov_n1) );
        REQUIRE( (oct_traits::name_to_id(ov_x_sname) == oct_limits::invalid_var_id) );

        // NOTE Conversion from octdiff variable name to octdiff variable ID is prohibited atm.
    }

    SECTION("arithmetic_to_range(value) ["
        "must return a valid variable ID if the value passed is within the variable ID type's bounds (e.g. first-end), "
        "else invalid_var_id"
    ) {
        adl_static_assert( (oct_traits::arithmetic_to_range(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_e) == ov_e) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_r) == ov_r) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_p) == ov_p) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_n) == ov_n) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_range(ov_n1) == ov_n1) );

        adl_static_assert( (octdiff_traits::arithmetic_to_range(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_e) == odv_e) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_r) == odv_r) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_p) == odv_p) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_range(odv_n1) == odv_n1) );
    }

    SECTION("arithmetic_to_valid(value) ["
        "must return a valid variable ID if the value passed is a valid variable ID (e.g. first-last), "
        "else invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::arithmetic_to_valid(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_r) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_p) == ov_p) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_n) == ov_n) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::arithmetic_to_valid(ov_n1) == ov_n1) );

        adl_static_assert( (octdiff_traits::arithmetic_to_valid(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_r) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_p) == odv_p) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::arithmetic_to_valid(odv_n1) == odv_n1) );
    }

    SECTION("id_to_arithmetic(id) ["
        "should return the id only if it's within bounds, "
        "else return default-constructed value]"
    ) {
        constexpr auto fallback_value_ = 9999.9f;

        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_0, fallback_value_) == oct_limits::invalid_var_id) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_e, fallback_value_) == ov_e) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_r, fallback_value_) == ov_r) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_E, fallback_value_) == fallback_value_) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_p1, fallback_value_) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_arithmetic(ov_n1, fallback_value_) == ov_n1) );

        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_0, fallback_value_) == octdiff_limits::invalid_var_id) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_e, fallback_value_) == odv_e) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_r, fallback_value_) == odv_r) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_E, fallback_value_) == fallback_value_) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_n, fallback_value_) == fallback_value_) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_p1, fallback_value_) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_arithmetic(odv_n1, fallback_value_) == odv_n1) );
    }

    SECTION("id_to_range(id) ["
        "must return the provided variable ID if the ID itself is a variable ID within bounds (e.g. first-end), "
        "else return invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::id_to_range(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_e) == ov_e) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_r) == ov_r) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_p) == ov_p) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_n) == ov_n) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_range(ov_n1) == ov_n1) );

        adl_static_assert( (octdiff_traits::id_to_range(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_e) == odv_e) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_r) == odv_r) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_p) == odv_p) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_range(odv_n1) == odv_n1) );
    }

    SECTION("id_to_valid(id) ["
        "must return the provided variable ID if the ID itself is a valid variable ID (e.g. first-last), "
        "else return invalid_var_id]"
    ) {
        adl_static_assert( (oct_traits::id_to_valid(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_r) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_p) == ov_p) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_n) == ov_n) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_valid(ov_n1) == ov_n1) );

        adl_static_assert( (octdiff_traits::id_to_valid(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_e) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_r) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_p) == odv_p) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_valid(odv_n1) == odv_n1) );
    }

    SECTION("id_to_index(id) [should return a size_t value suitable for indexing a memory region (e.g. array)]") {
        // invalid values result in undefined behaviour

        adl_static_assert( (oct_traits::id_to_index(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_index(ov_p1) == ov_p1_index) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_index(ov_n1) == ov_n1_index) );

        adl_static_assert( (octdiff_traits::id_to_index(0), true) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_index(odv_p1) == odv_p1_index) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_index(odv_n1) == odv_n1_index) );
    }

    SECTION("id_to_counterpart(id) [should return id's counterpart variable in the opposite domain]") {

        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(0) == 0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_e) == odv_e) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_r) == odv_r) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_p1) == odv_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_n1) == odv_n1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_p3) == odv_p3) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_counterpart(ov_n3) == odv_n3) );

        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(0) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_0) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_e) == ov_e) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_r) == ov_r) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n1) == ov_n1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_p3) == ov_p3) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_counterpart(odv_n3) == ov_n3) );
    }

    SECTION("id_to_normal_oct(id) [should return a normalized oct-space variable ID]") {

        adl_static_assert( (oct_traits::id_to_normal_oct(0), true) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_r) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_E) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_n1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_p3) == ov_p3) );
        REQUIRE_TRUE_NOTHROW( (oct_traits::id_to_normal_oct(ov_n3) == ov_p3) );

        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(0) == 0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_0) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_e) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_r) == ov_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_E) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_n) == odv_0) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_p1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_n1) == ov_p1) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_p3) == ov_p3) );
        REQUIRE_TRUE_NOTHROW( (octdiff_traits::id_to_normal_oct(odv_n3) == ov_p3) );
    }

    SECTION("id_to_name(id) [should return a name representation of a variable ID, considering the sign]") {

        std::string buf;

        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_0), buf == oct_traits::id_to_name(ov_0) && buf.empty()) );
        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_e), buf == oct_traits::id_to_name(ov_e) && buf.empty()) );
        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_r), buf == oct_traits::id_to_name(ov_r) && buf.empty()) );
        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_E), buf == oct_traits::id_to_name(ov_E) && buf.empty()) );
        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_p1), buf == oct_traits::id_to_name(ov_p1) && buf == ov_p1_sname) );
        REQUIRE( (buf.clear(), oct_traits::id_to_name(buf, ov_n1), buf == oct_traits::id_to_name(ov_n1) && buf == ov_n1_sname) );

        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_0), buf == octdiff_traits::id_to_name(odv_0) && buf.empty()) );
        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_e), buf == octdiff_traits::id_to_name(odv_e) && buf.empty()) );
        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_r), buf == octdiff_traits::id_to_name(odv_r) && buf.empty()) );
        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_E), buf == octdiff_traits::id_to_name(odv_E) && buf.empty()) );
        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_p1), buf == octdiff_traits::id_to_name(odv_p1) && buf == odv_p1_sname) );
        REQUIRE( (buf.clear(), octdiff_traits::id_to_name(buf, odv_n1), buf == octdiff_traits::id_to_name(odv_n1) && buf == odv_n1_sname) );
    }
}
