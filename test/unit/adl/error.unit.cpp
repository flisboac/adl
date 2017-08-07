// $flisboac 2017-03-19
#include "adl_catch.hpp"
#include "adl/error.hpp"

TEST_CASE("unit:adl/error", "[adl][adl/error]") {
    const auto warning_kind = adl::make_error_kind(adl::errk::warning);
    const auto ok_kind = adl::make_error_kind(adl::errk::ok);
    const auto error_kind = adl::make_error_kind(adl::errk::error);
    const auto client_error_kind = adl::make_error_kind(adl::errk::client_error);

    const adl::error_info no_error;
    const adl::error_info internal_error = adl::make_error_info(adl::err::error);

    SECTION("adl::error_enum_to_int") {
        constexpr int value = -2;
        constexpr adl::errk kind = adl::errk::warning;
        static_assert((adl::error_enum_to_int(kind), true), "must be constexpr");
        REQUIRE((adl::error_enum_to_int(kind) == value));
    }

    SECTION("adl::error_info") {

        REQUIRE( (warning_kind == warning_kind) );

        REQUIRE( (no_error == ok_kind) );
        REQUIRE( (no_error != error_kind) );
        REQUIRE( (no_error != client_error_kind) );
        REQUIRE( (no_error != warning_kind) );

        REQUIRE( (internal_error != ok_kind) );
        REQUIRE( (internal_error == error_kind) );
        REQUIRE( (internal_error != client_error_kind) );
        REQUIRE( (internal_error != warning_kind) );
    }

    SECTION("adl: result functions") {
        constexpr int return_value = 2;

        REQUIRE_THROWS_AS( (adl::finished(internal_error)), adl::internal_exception );
        REQUIRE_NOTHROW( (adl::finished(no_error)) );

        REQUIRE_THROWS_AS( (adl::result(internal_error, return_value) == return_value), adl::internal_exception );
        REQUIRE_NOTHROW( (adl::result(no_error, return_value) == return_value) );

        REQUIRE_NOTHROW( (adl::safe_result(no_error, return_value)) );
        REQUIRE_NOTHROW( (*adl::safe_result(no_error, return_value) == return_value) );
        REQUIRE_NOTHROW( (!adl::safe_result(internal_error, return_value)) );
    }
}
