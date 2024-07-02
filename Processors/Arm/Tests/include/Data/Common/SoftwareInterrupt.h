#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_SOFTWARE_INTERRUPT_VALUES[] = {
    {
        .name = "SWI without condition",
        .assembly = "SWI 201",
        .instruction = 0xEF0000C9
    },
    {
        .name = "SWI with condition",
        .assembly = "SWINE 201",
        .instruction = 0x1F0000C9
    }
};
