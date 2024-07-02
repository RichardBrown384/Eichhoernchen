#pragma once

#include "Data/Assembler/AssemblerPositiveParameter.h"

constexpr AssemblerPositiveParameter ASSEMBLER_OPERAND_2_POSITIVE_VALUES[] = {
    {
        .name = "Assembles dyadic data processing (ADD) instruction without any shift",
        .assembly = "ADD R2, R3, R4",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles dyadic data processing instruction with instruction specified LSL",
        .assembly = "ADD R2, R3, R4, LSL #3",
        .instruction = 0xE0832184
    },
    {
        .name = "Assembles dyadic data processing instruction with instruction specified LSR",
        .assembly = "ADD R2, R3, R4, LSR #2",
        .instruction = 0xE0832124
    },
    {
        .name = "Assembles dyadic data processing instruction with instruction specified ASR",
        .assembly = "ADD R2, R3, R4, ASR #4",
        .instruction = 0xE0832244
    },
    {
        .name = "Assembles dyadic data processing instruction with instruction specified ROR",
        .assembly = "ADD R2, R3, R4, ROR #4",
        .instruction = 0xE0832264
    },
    {
        .name = "Assembles dyadic data processing instruction with instruction specified LSL #0",
        .assembly = "ADD R2, R3, R4, LSL #0",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles instruction specified LSR #0 as LSL #0",
        .assembly = "ADD R2, R3, R4, LSR #0",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles instruction specified ASR #0 as LSL #0",
        .assembly = "ADD R2, R3, R4, ASR #0",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles instruction specified ROR #0 as LSL #0",
        .assembly = "ADD R2, R3, R4, ROR #0",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles instruction specified LSR #32",
        .assembly = "ADD R2, R3, R4, LSR #32",
        .instruction = 0xE0832024
    },
    {
        .name = "Assembles instruction specified ASR #32",
        .assembly = "ADD R2, R3, R4, ASR #32",
        .instruction = 0xE0832044
    },
    {
        .name = "Assembles RRX",
        .assembly = "ADD R2, R3, R4, RRX",
        .instruction = 0xE0832064
    },
    {
        .name = "Assembles dyadic data processing instruction with register specified LSL",
        .assembly = "ADD R2, R3, R4, LSL R8",
        .instruction = 0xE0832814
    },
    {
        .name = "Assembles dyadic data processing instruction with register specified LSR",
        .assembly = "ADD R2, R3, R4, LSR R8",
        .instruction = 0xE0832834
    },
    {
        .name = "Assembles dyadic data processing instruction with register specified ASR",
        .assembly = "ADD R2, R3, R4, ASR R8",
        .instruction = 0xE0832854
    },
    {
        .name = "Assembles dyadic data processing instruction with register specified ROR",
        .assembly = "ADD R2, R3, R4, ROR R8",
        .instruction = 0xE0832874
    },
    {
        .name = "Assembles dyadic data processing (ADD) with base 10 immediate value",
        .assembly = "ADD R2, R3, #892",
        .instruction = 0xE2832FDF
    },
    {
        .name = "Assembles dyadic data processing (ADD) with base 16 immediate value",
        .assembly = "ADD R2, R3, #0x00830000",
        .instruction = 0xE2832883
    }
};
