#include "adl_catch.hpp"
#include "adl.cfg.hpp"
#include <iostream>


static char const* source =
#include "adl/oct/cl/kernel/defs.inc.h"
;

#include <stddef.h>
#undef adl_INCLUDE_STRFY
#define adl_INCLUDE_STRFY_(s__) s__
#define adl_INCLUDE_STRFY(s__) adl_INCLUDE_STRFY_(s__)
extern "C" {
#include "adl/oct/cl/kernel/defs.inc.h"
}


TEST_CASE("unit:adl/oct/cl/kernel/defs.h", "") {
    // TODO Tests
}
