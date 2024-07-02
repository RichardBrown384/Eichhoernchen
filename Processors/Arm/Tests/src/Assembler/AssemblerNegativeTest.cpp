#include <gtest/gtest.h>

#include "Data/Assembler/AssemblerNegativeParameter.h"
#include "Data/Assembler/BlockTransferNegatives.h"
#include "Data/Assembler/Operand2Negatives.h"
#include "Data/Assembler/SingleDataTransferNegatives.h"

#include "Arm/Assembler.h"

class AssemblerNegativeTest : public ::testing::TestWithParam<AssemblerNegativeParameter> {};

TEST_P(AssemblerNegativeTest, AssemblerNegativeTestCase) {
    const AssemblerNegativeParameter param = GetParam();
    uint32_t instruction;
    bool result = rbrown::arm::Assemble(param.assembly, instruction);
    ASSERT_FALSE(result);
}

INSTANTIATE_TEST_SUITE_P(
    BlockDataTransferTestNegatives,
    AssemblerNegativeTest,
    ::testing::ValuesIn(ASSEMBLER_BLOCK_TRANSFER_NEGATIVE_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    Operand2TestNegatives,
    AssemblerNegativeTest,
    ::testing::ValuesIn(ASSEMBLER_OPERAND_2_NEGATIVE_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SingleDataTransferTestNegatives,
    AssemblerNegativeTest,
    ::testing::ValuesIn(ASSEMBLER_SINGLE_DATA_TRANSFER_NEGATIVE_VALUES)
);