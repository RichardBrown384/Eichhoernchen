#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_BRANCH_VALUES[] = {
    {
        .name = "Branch without condition",
        .assembly = "B #300",
        .instruction = 0xEA000049
    },
    {
        .name = "Branch with condition",
        .assembly = "BNE #300",
        .instruction = 0x1A000049
    },
    {
        .name = "Branch with link without condition",
        .assembly = "BL #300",
        .instruction = 0xEB000049
    },
    {
        .name = "Branch with link with condition",
        .assembly = "BLNE #300",
        .instruction = 0x1B000049
    },
    {
        .name = "Negative Branch without condition",
        .assembly = "B #-300",
        .instruction = 0xEAFFFFB3
    },
};
