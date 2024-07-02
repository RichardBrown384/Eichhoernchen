#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_COPROCESSOR_DATA_OPERATION_VALUES[] = {
    {
        .name = "CDP without coprocessor information",
        .assembly = "CDP p1, #10, c1, c2, c3",
        .instruction = 0xEEA21103
    },
    {
        .name = "CDP with coprocessor information",
        .assembly = "CDP p1, #10, c1, c2, c3, #5",
        .instruction = 0xEEA211A3
    },
    {
        .name = "CDP with condition code",
        .assembly = "CDPEQ p1, #10, c1, c2, c3",
        .instruction = 0x0EA21103
    },
};
