#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_MULTIPLY_VALUES[] = {
    {
        .name = "Assembles MLA without condition",
        .assembly = "MLA R2, R3, R4, R5",
        .instruction = 0xE0225493
    },
    {
        .name = "Assembles MLA with condition",
        .assembly = "MLAGE R2, R3, R4, R5",
        .instruction = 0xA0225493
    },
    {
        .name = "Assembles MLA without condition and set flags",
        .assembly = "MLAS R2, R3, R4, R5",
        .instruction = 0xE0325493
    },
    {
        .name = "Assembles MLA with condition and set flags",
        .assembly = "MLALSS R2, R3, R4, R5",
        .instruction = 0x90325493
    },
    {
        .name = "Assembles MUL without condition",
        .assembly = "MUL R2, R3, R4",
        .instruction = 0xE0020493
    },
    {
        .name = "Assembles MUL with condition",
        .assembly = "MULMI R2, R3, R4",
        .instruction = 0x40020493
    },
    {
        .name = "Assembles MUL without condition and set flags",
        .assembly = "MULS R2, R3, R4",
        .instruction = 0xE0120493
    },
    {
        .name = "Assembles MUL with condition and set flags",
        .assembly = "MULLSS R2, R3, R4",
        .instruction = 0x90120493
    },
};
