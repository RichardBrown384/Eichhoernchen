#pragma once

#include "Data/Common/CommonParameter.h"

constexpr CommonParameter COMMON_DATA_PROCESSING_VALUES[] = {
    {
        .name = "Assembles ADC without condition",
        .assembly = "ADC R2, R3, R4",
        .instruction = 0xE0A32004
    },
    {
        .name = "Assembles ADC with condition",
        .assembly = "ADCLT R2, R3, R4",
        .instruction = 0xB0A32004
    },
    {
        .name = "Assembles ADC without condition and set flags",
        .assembly = "ADCS R2, R3, R4",
        .instruction = 0xE0B32004
    },
    {
        .name = "Assembles ADC with condition and set flags",
        .assembly = "ADCHIS R2, R3, R4",
        .instruction = 0x80B32004
    },
    {
        .name = "Assembles ADD without condition",
        .assembly = "ADD R2, R3, R4",
        .instruction = 0xE0832004
    },
    {
        .name = "Assembles ADD with immediate",
        .assembly = "ADD R2, R3, #34816",
        .instruction = 0xE2832B22
    },
    {
        .name = "Assembles AND without condition",
        .assembly = "AND R2, R3, R4",
        .instruction = 0xE0032004
    },
    {
        .name = "Assembles BIC without condition",
        .assembly = "BIC R2, R3, R4",
        .instruction = 0xE1C32004
    },
    {
        .name = "Assembles CMP without condition",
        .assembly = "CMP R2, R4",
        .instruction = 0xE1520004
    },
    {
        .name = "Assembles CMP with condition",
        .assembly = "CMPPL R2, R4",
        .instruction = 0x51520004
    },
    {
        .name = "Assembles CMP with instruction specified shift",
        .assembly = "CMP R2, R4, LSR #3",
        .instruction = 0xE15201A4
    },
    {
        .name = "Assembles CMP with register specified shift",
        .assembly = "CMP R2, R4, LSR R3",
        .instruction = 0xE1520334
    },
    {
        .name = "Assembles CMP with immediate",
        .assembly = "CMP R2, #12",
        .instruction = 0xE352000C
    },
    {
        .name = "Assembles CMPP  without condition",
        .assembly = "CMPP R2, R4",
        .instruction = 0xE152F004
    },
    {
        .name = "Assembles CMN without condition",
        .assembly = "CMN R2, R4",
        .instruction = 0xE1720004
    },
    {
        .name = "Assembles CMNP without condition",
        .assembly = "CMNP R2, R4",
        .instruction = 0xE172F004
    },
    {
        .name = "Assembles EOR without condition",
        .assembly = "EOR R2, R3, R4",
        .instruction = 0xE0232004
    },
    {
        .name = "Assembles MOV without condition",
        .assembly = "MOV R2, R4",
        .instruction = 0xE1A02004
    },
    {
        .name = "Assembles MOV with condition",
        .assembly = "MOVLT R2, R4",
        .instruction = 0xB1A02004
    },
    {
        .name = "Assembles MOV without condition and set flags",
        .assembly = "MOVS R2, R4",
        .instruction = 0xE1B02004
    },
    {
        .name = "Assembles MOV with condition and set flags",
        .assembly = "MOVLTS R2, R4",
        .instruction = 0xB1B02004
    },
    {
        .name = "Assembles MOV with instruction specified shift",
        .assembly = "MOV R2, R4, LSR #3",
        .instruction = 0xE1A021A4
    },
    {
        .name = "Assembles MOV with register specified shift",
        .assembly = "MOV R2, R4, LSR R3",
        .instruction = 0xE1A02334
    },
    {
        .name = "Assembles MOV with immediate",
        .assembly = "MOV R2, #12",
        .instruction = 0xE3A0200C
    },
    {
        .name = "Assembles MVN without condition",
        .assembly = "MVN R2, R4",
        .instruction = 0xE1E02004
    },
    {
        .name = "Assembles ORR without condition",
        .assembly = "ORR R2, R3, R4",
        .instruction = 0xE1832004
    },
    {
        .name = "Assembles RSB without condition",
        .assembly = "RSB R2, R3, R4",
        .instruction = 0xE0632004
    },
    {
        .name = "Assembles RSC without condition",
        .assembly = "RSC R2, R3, R4",
        .instruction = 0xE0E32004
    },
    {
        .name = "Assembles SBC without condition",
        .assembly = "SBC R2, R3, R4",
        .instruction = 0xE0C32004
    },
    {
        .name = "Assembles SUB without condition",
        .assembly = "SUB R2, R3, R4",
        .instruction = 0xE0432004
    },
    {
        .name = "Assembles TEQ without condition",
        .assembly = "TEQ R2, R4",
        .instruction = 0xE1320004
    },
    {
        .name = "Assembles TEQP without condition",
        .assembly = "TEQP R2, R4",
        .instruction = 0xE132F004
    },
    {
        .name = "Assembles TST without condition",
        .assembly = "TST R2, R4",
        .instruction = 0xE1120004
    },
    {
        .name = "Assembles TSTP without condition",
        .assembly = "TSTP R2, R4",
        .instruction = 0xE112F004
    },
};
