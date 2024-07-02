#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_BLOCK_DATA_TRANSFER_VALUES[] = {
    {
        .name = "Load multiple increment after",
        .assembly = "LDMIA R3, {R4, R6-R10}",
        .instruction = 0xE89307D0
    },
    {
        .name = "Load multiple increment before",
        .assembly = "LDMIB R3, {R4, R6-R10}",
        .instruction = 0xE99307D0
    },
    {
        .name = "Load multiple decrement after",
        .assembly = "LDMDA R3, {R4, R6-R10}",
        .instruction = 0xE81307D0
    },
    {
        .name = "Load multiple decrement before",
        .assembly = "LDMDB R3, {R4, R6-R10}",
        .instruction = 0xE91307D0
    },
    {
        .name = "Load multiple increment after with write-back",
        .assembly = "LDMIA R3!, {R4, R6-R10}",
        .instruction = 0xE8B307D0
    },
    {
        .name = "Load multiple increment after with force-user bit",
        .assembly = "LDMIA R3, {R4, R6-R10}^",
        .instruction = 0xE8D307D0
    },
    {
        .name = "Load multiple increment after with condition code",
        .assembly = "LDMGEIA R3, {R4, R6-R10}",
        .instruction = 0xA89307D0
    },
    {
        .name = "Store multiple increment after",
        .assembly = "STMIA R3, {R4, R6-R10}",
        .instruction = 0xE88307D0
    },
    {
        .name = "Load multiple increment after with multiple register ranges",
        .assembly = "LDMIA R3, {R4, R6-R8, R10-R12, R14}",
        .instruction = 0xE8935DD0
    },
};
