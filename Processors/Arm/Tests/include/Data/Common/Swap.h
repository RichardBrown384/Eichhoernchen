#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_SWAP_VALUES[] = {
    {
        .name = "Assembles SWP without condition",
        .assembly = "SWP R2, R3, [R4]",
        .instruction = 0xE1042093
    },
    {
        .name = "Assembles SWP with condition",
        .assembly = "SWPMI R2, R3, [R4]",
        .instruction = 0x41042093
    },
    {
        .name = "Assembles SWP Byte without condition",
        .assembly = "SWPB R2, R3, [R4]",
        .instruction = 0xE1442093
    },
    {
        .name = "Assembles SWP Byte with condition",
        .assembly = "SWPEQB R2, R3, [R4]",
        .instruction = 0x01442093
    },
};
