#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_SINGLE_DATA_TRANSFER_VALUES[] = {
    {
        .name = "Load pre-indexed with zero offset",
        .assembly = "LDR R2, [R3]",
        .instruction = 0xE5932000
    },
    {
        .name = "Load pre-indexed with zero offset and write back",
        .assembly = "LDR R2, [R3]!",
        .instruction = 0xE5B32000
    },
    {
        .name = "Load byte pre-indexed with zero offset",
        .assembly = "LDRB R2, [R3]",
        .instruction = 0xE5D32000
    },
    {
        .name = "Load pre-indexed with non-zero offset",
        .assembly = "LDR R2, [R3, #100]",
        .instruction = 0xE5932064
    },
    {
        .name = "Load pre-indexed with negative non-zero offset",
        .assembly = "LDR R2, [R3, #-100]",
        .instruction = 0xE5132064
    },
    {
        .name = "Load pre-indexed with non-zero offset and write back",
        .assembly = "LDR R2, [R3, #100]!",
        .instruction = 0xE5B32064
    },
    {
        .name = "Load pre-indexed with offset greater than 8 bits",
        .assembly = "LDR R2, [R3, #3000]",
        .instruction = 0xE5932BB8
    },
    {
        .name = "Load pre-indexed register only",
        .assembly = "LDR R2, [R3, R4]",
        .instruction = 0xE7932004
    },
    {
        .name = "Load pre-indexed downwards register only",
        .assembly = "LDR R2, [R3, -R4]",
        .instruction = 0xE7132004
    },
    {
        .name = "Load pre-indexed register only with write back",
        .assembly = "LDR R2, [R3, R4]!",
        .instruction = 0xE7B32004
    },
    {
        .name = "Load pre-indexed with shift",
        .assembly = "LDR R2, [R3, R4, LSL #17]",
        .instruction = 0xE7932884
    },
    {
        .name = "Load pre-indexed with shift downwards",
        .assembly = "LDR R2, [R3, -R4, LSL #17]",
        .instruction = 0xE7132884
    },
    {
        .name = "Load pre-indexed with shift and write back",
        .assembly = "LDR R2, [R3, R4, LSL #17]!",
        .instruction = 0xE7B32884
    },
    {
        .name = "Load post-indexed zero offset with write back",
        .assembly = "LDRT R2, [R3]",
        .instruction = 0xE4B32000
    },
    {
        .name = "Load post-indexed with non-zero offset",
        .assembly = "LDR R2, [R3], #12",
        .instruction = 0xE493200C
    },
    {
        .name = "Load post-indexed with non-zero offset downwards",
        .assembly = "LDR R2, [R3], #-12",
        .instruction = 0xE413200C
    },
    {
        .name = "Load post-indexed non-zero offset with write back",
        .assembly = "LDRT R2, [R3], #12",
        .instruction = 0xE4B3200C
    },
    {
        .name = "Load byte post-indexed with non-zero offset",
        .assembly = "LDRB R2, [R3], #12",
        .instruction = 0xE4D3200C
    },
    {
        .name = "Load byte post-indexed with offset greater than 8 bits",
        .assembly = "LDRB R2, [R3], #4082",
        .instruction = 0xE4D32FF2
    },
    {
        .name = "Load post-indexed with register",
        .assembly = "LDR R2, [R3], R4",
        .instruction = 0xE6932004
    },
    {
        .name = "Load post-indexed byte with write back",
        .assembly = "LDRBT R2, [R3], R4",
        .instruction = 0xE6F32004
    },
    {
        .name = "Load post-indexed with shift",
        .assembly = "LDR R2, [R3], R4, LSL #17",
        .instruction = 0xE6932884
    },
    {
        .name = "Load post-indexed with shift downward",
        .assembly = "LDR R2, [R3], -R4, LSL #17",
        .instruction = 0xE6132884
    },
    {
        .name = "Load pre-indexed with condition",
        .assembly = "LDRNE R2, [R3]",
        .instruction = 0x15932000
    },
    {
        .name = "Load pre-indexed byte with condition",
        .assembly = "LDRNEB R2, [R3]",
        .instruction = 0x15D32000
    },
    {
        .name = "Load post-indexed condition and write back",
        .assembly = "LDRNET R2, [R3]",
        .instruction = 0x14B32000
    },
    {
        .name = "Load post-indexed byte with condition and write back",
        .assembly = "LDRNEBT R2, [R3]",
        .instruction = 0x14F32000
    },
    {
        .name = "Store pre-indexed zero offset",
        .assembly = "STR R2, [R3]",
        .instruction = 0xE5832000
    },
    {
        .name = "Store pre-indexed zero offset with write back",
        .assembly = "STR R2, [R3]!",
        .instruction = 0xE5A32000
    },
    {
        .name = "Store pre-indexed byte with zero-offset",
        .assembly = "STRB R2, [R3]",
        .instruction = 0xE5C32000
    },
    {
        .name = "Store post-indexed zero offset with write back",
        .assembly = "STRT R2, [R3]",
        .instruction = 0xE4A32000
    },
    {
        .name = "Store pre-indexed non-zero offset",
        .assembly = "STR R2, [R3, #100]",
        .instruction = 0xE5832064
    },
    {
        .name = "Store pre-indexed non-zero offset downwards",
        .assembly = "STR R2, [R3, #-100]",
        .instruction = 0xE5032064
    },
    {
        .name = "Store pre-indexed non-zero offset with right back",
        .assembly = "STR R2, [R3, #100]!",
        .instruction = 0xE5A32064
    },
    {
        .name = "Store pre-indexed with register",
        .assembly = "STR R2, [R3, R4]",
        .instruction = 0xE7832004
    },
    {
        .name = "Store pre-indexed with register downwards",
        .assembly = "STR R2, [R3, -R4]",
        .instruction = 0xE7032004
    },
    {
        .name = "Store pre-indexed with register write back",
        .assembly = "STR R2, [R3, R4]!",
        .instruction = 0xE7A32004
    },
    {
        .name = "Store pre-indexed with shift",
        .assembly = "STR R2, [R3, R4, LSL #17]",
        .instruction = 0xE7832884
    },
    {
        .name = "Store pre-indexed with shift downwards",
        .assembly = "STR R2, [R3, -R4, LSL #17]",
        .instruction = 0xE7032884
    },
    {
        .name = "Store pre-indexed with shift and write back",
        .assembly = "STR R2, [R3, R4, LSL #17]!",
        .instruction = 0xE7A32884
    },
    {
        .name = "Store post-indexed with non-zero offset",
        .assembly = "STR R2, [R3], #12",
        .instruction = 0xE483200C
    },
    {
        .name = "Store post-indexed with non-zero offset downwards",
        .assembly = "STR R2, [R3], #-12",
        .instruction = 0xE403200C
    },
    {
        .name = "Store post-indexed with non-zero offset and write back",
        .assembly = "STRT R2, [R3], #12",
        .instruction = 0xE4A3200C
    },
    {
        .name = "Store post-indexed byte with non-zero offset",
        .assembly = "STRB R2, [R3], #12",
        .instruction = 0xE4C3200C
    },
    {
        .name = "Store post-indexed with register",
        .assembly = "STR R2, [R3], R4",
        .instruction = 0xE6832004
    },
    {
        .name = "Store post-indexed with shift",
        .assembly = "STR R2, [R3], R4, LSL #17",
        .instruction = 0xE6832884
    },
    {
        .name = "Store post-indexed with shift downward",
        .assembly = "STR R2, [R3], -R4, LSL #17",
        .instruction = 0xE6032884
    },
    {
        .name = "Store post-indexed byte zero offset with write back",
        .assembly = "STRBT R2, [R3]",
        .instruction = 0xE4E32000
    },
};
