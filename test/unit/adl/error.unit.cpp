// $flisboac 2017-03-19
#include "adl_catch.hpp"
#include "adl/error.hpp"

TEST_CASE("adl/error.hpp", "[adl][adl/error]") {

    SECTION("adl::error_info") {
        const adl::error_info warning_kind = adl::make_error_info(adl::error_kind_id::warning);
        const adl::error_info ok_kind = adl::make_error_info(adl::error_kind_id::ok);
        const adl::error_info error_kind = adl::make_error_info(adl::error_kind_id::error);
        const adl::error_info client_error_kind = adl::make_error_info(adl::error_kind_id::client_error);

        const adl::error_info no_error;
        const adl::error_info some_error = adl::error_id::function_not_supported;

        REQUIRE_SECTION((warning_kind == warning_kind), "error should equal itself");

        REQUIRE_SECTION((ok_kind == no_error), "no-error info should be an adl::error_kind_id::ok");
        REQUIRE_SECTION((no_error != error_kind), "no-error info should not be an adl::error_kind_id::error");
        REQUIRE_SECTION((no_error != client_error_kind), "no-error info should not be an adl::error_kind_id::client_error");
        REQUIRE_SECTION((no_error != warning_kind), "no-error info should not be an adl::error_kind_id::warning");

        REQUIRE_SECTION((some_error != ok_kind), "an error-kind error info should be an adl::error_kind_id::ok");
        REQUIRE_SECTION((some_error == error_kind), "an error-kind error info should not be an adl::error_kind_id::error");
        REQUIRE_SECTION((some_error != client_error_kind), "an error-kind error info should not be an adl::error_kind_id::client_error");
        REQUIRE_SECTION((some_error != warning_kind), "an error info-kind error should not be an adl::error_kind_id::warning");

        REQUIRE_SECTION_THROWS_AS((adl::done(some_error)), adl::internal_exception, "an error-kind error_info should throw adl::internal_exception");
    }
}
