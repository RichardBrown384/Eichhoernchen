#pragma once

#include "Data/Assembler/AssemblerPositiveParameter.h"

constexpr AssemblerPositiveParameter ASSEMBLER_CONDITION_CODE_POSITIVE_VALUES[] = {
    {
        .name = "Condition code EQ assembles correctly",
        .assembly = "ADDEQ R1, R2, R2",
        .instruction = 0x00821002
    },
    {
        .name = "Condition code NE assembles correctly",
        .assembly = "ADDNE R1, R2, R2",
        .instruction = 0x10821002
    },
    {
        .name = "Condition code CS assembles correctly",
        .assembly = "ADDCS R1, R2, R2",
        .instruction = 0x20821002
    },
    {
        .name = "Condition code CC assembles correctly",
        .assembly = "ADDCC R1, R2, R2",
        .instruction = 0x30821002
    },
    {
        .name = "Condition code MI assembles correctly",
        .assembly = "ADDMI R1, R2, R2",
        .instruction = 0x40821002
    },
    {
        .name = "Condition code PL assembles correctly",
        .assembly = "ADDPL R1, R2, R2",
        .instruction = 0x50821002
    },
    {
        .name = "Condition code VS assembles correctly",
        .assembly = "ADDVS R1, R2, R2",
        .instruction = 0x60821002
    },
    {
        .name = "Condition code VC assembles correctly",
        .assembly = "ADDVC R1, R2, R2",
        .instruction = 0x70821002
    },
    {
        .name = "Condition code HI assembles correctly",
        .assembly = "ADDHI R1, R2, R2",
        .instruction = 0x80821002
    },
    {
        .name = "Condition code LS assembles correctly",
        .assembly = "ADDLS R1, R2, R2",
        .instruction = 0x90821002
    },
    {
        .name = "Condition code GE assembles correctly",
        .assembly = "ADDGE R1, R2, R2",
        .instruction = 0xA0821002
    },
    {
        .name = "Condition code LT assembles correctly",
        .assembly = "ADDLT R1, R2, R2",
        .instruction = 0xB0821002
    },
    {
        .name = "Condition code GT assembles correctly",
        .assembly = "ADDGT R1, R2, R2",
        .instruction = 0xC0821002
    },
    {
        .name = "Condition code LE assembles correctly",
        .assembly = "ADDLE R1, R2, R2",
        .instruction = 0xD0821002
    },
    {
        .name = "Condition code AL assembles correctly",
        .assembly = "ADDAL R1, R2, R2",
        .instruction = 0xE0821002
    },
    {
        .name = "Condition code NV assembles correctly",
        .assembly = "ADDNV R1, R2, R2",
        .instruction = 0xF0821002
    }
};
