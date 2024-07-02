#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_COPROCESSOR_REGISTER_TRANSFER_VALUES[] = {
    {
        .name = "MCR without coprocessor information",
        .assembly = "MCR p0, #7, R1, c3, c4",
        .instruction = 0xEEE31014
    },
    {
        .name = "MCR with coprocessor information",
        .assembly = "MCR p0, #7, R1, c3, c4, #3",
        .instruction = 0xEEE31074
    },
    {
        .name = "MCR with condition code",
        .assembly = "MCRMI p0, #7, R1, c3, c4",
        .instruction = 0x4EE31014
    },
    {
        .name = "MRC without coprocessor information",
        .assembly = "MRC p0, #7, R1, c3, c4",
        .instruction = 0xEEF31014
    },
    {
        .name = "MRC with coprocessor information",
        .assembly = "MRC p0, #7, R1, c3, c4, #3",
        .instruction = 0xEEF31074
    },
    {
        .name = "MRC with condition code",
        .assembly = "MRCMI p0, #7, R1, c3, c4",
        .instruction = 0x4EF31014
    }
};
