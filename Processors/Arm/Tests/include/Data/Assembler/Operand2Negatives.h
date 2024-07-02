#pragma once

#include "Data/Assembler/AssemblerNegativeParameter.h"

constexpr AssemblerNegativeParameter ASSEMBLER_OPERAND_2_NEGATIVE_VALUES[] = {
    {
        .name = "Should not assemble instruction specified LSR #32",
        .assembly = "ADD R2, R3, R4, LSL #32",
    },
    {
        .name = "Should not assemble instruction specified ROR #32",
        .assembly = "ADD R2, R3, R4, ROR #32",
    },
    {
        .name = "Should not assemble instruction specified RRX",
        .assembly = "ADD R2, R3, R4, RRX #10",
    },
    {
        .name = "Should not assemble register specified RRX",
        .assembly = "ADD R2, R3, R4, RRX R5",
    },
    {
        .name = "Should not assemble an immediate value that cannot be packed into 8 bits",
        .assembly = "ADD R2, R3, #0x20008000",
    }
};
