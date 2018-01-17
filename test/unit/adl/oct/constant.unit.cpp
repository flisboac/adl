// $flisboac 2017-04-02
#include "adl_catch.hpp"
#include "adl/oct/constant.hpp"

TEST_CASE("unit:adl/oct/constant") {
    using namespace adl::oct;

    // NOT ALLOWED
    //int i = float_int(1);

    // ALLOWED
    auto i = static_cast<int>(float_int(1));
     i += float_int(1);

    float_int f = 1 + 2;
    f += 2;
    static_assert(std::is_same<float_int, decltype(float_int(1) * 1)>::value, "oops");
    static_assert(std::is_same<float_int, decltype(1 * float_int(1))>::value, "oops");
}