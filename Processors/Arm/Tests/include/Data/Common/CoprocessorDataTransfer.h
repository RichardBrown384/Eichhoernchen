#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_COPROCESSOR_DATA_TRANSFER_VALUES[] = {
    {
        .name = "LDC pre-indexed without condition and zero offset",
        .assembly = "LDC p2, c3, [R5]",
        .instruction = 0xED953200
    },
    {
        .name = "LDC pre-indexed without condition and zero offset and write back",
        .assembly = "LDC p2, c3, [R5]!",
        .instruction = 0xEDB53200
    },
    {
        .name = "LDC pre-indexed without condition and non-zero offset",
        .assembly = "LDC p2, c3, [R5, #8]",
        .instruction = 0xED953202
    },
    {
        .name = "LDC pre-indexed without condition and non-zero offset and write back",
        .assembly = "LDC p2, c3, [R5, #8]!",
        .instruction = 0xEDB53202
    },
    {
        .name = "LDC pre-indexed without condition and negative offset",
        .assembly = "LDC p2, c3, [R5, #-8]",
        .instruction = 0xED153202
    },
    {
        .name = "LDC pre-indexed with condition",
        .assembly = "LDCNE p2, c3, [R5]",
        .instruction = 0x1D953200
    },
    {
        .name = "LDC pre-indexed long form",
        .assembly = "LDCL p2, c3, [R5]",
        .instruction = 0xEDD53200
    },
    {
        .name = "LDC pre-indexed long form with condition",
        .assembly = "LDCEQL p2, c3, [R5]",
        .instruction = 0x0DD53200
    },
    {
        .name = "LDC post-indexed, non-zero offset without condition",
        .assembly = "LDC p2, c3, [R5], #12",
        .instruction = 0xECB53203
    },
    {
        .name = "STC pre-indexed without condition and zero offset",
        .assembly = "STC p2, c3, [R5]",
        .instruction = 0xED853200
    },
    {
        .name = "STC pre-indexed without condition and zero offset and write back",
        .assembly = "STC p2, c3, [R5]!",
        .instruction = 0xEDA53200
    },
    {
        .name = "STC pre-indexed without condition and non-zero offset",
        .assembly = "STC p2, c3, [R5, #8]",
        .instruction = 0xED853202
    },
    {
        .name = "STC pre-indexed without condition and non-zero offset and write back",
        .assembly = "STC p2, c3, [R5, #8]!",
        .instruction = 0xEDA53202
    },
    {
        .name = "STC pre-indexed without condition and negative offset",
        .assembly = "STC p2, c3, [R5, #-8]",
        .instruction = 0xED053202
    },
    {
        .name = "STC pre-indexed with condition",
        .assembly = "STCNE p2, c3, [R5]",
        .instruction = 0x1D853200
    },
    {
        .name = "STC pre-indexed long form",
        .assembly = "STCL p2, c3, [R5]",
        .instruction = 0xEDC53200
    },
    {
        .name = "STC pre-indexed long form with condition",
        .assembly = "STCEQL p2, c3, [R5]",
        .instruction = 0x0DC53200
    },
    {
        .name = "STC post-indexed, non-zero offset without condition",
        .assembly = "STC p2, c3, [R5], #12",
        .instruction = 0xECA53203
    },
};
