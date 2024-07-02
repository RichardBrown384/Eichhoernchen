#include <gtest/gtest.h>

#include "Data/Common/CommonParameter.h"
#include "Data/Common/BlockDataTransfer.h"
#include "Data/Common/Branch.h"
#include "Data/Common/CoprocessorDataTransfer.h"
#include "Data/Common/CoprocessorDataOperation.h"
#include "Data/Common/CoprocessorRegisterTransfer.h"
#include "Data/Common/DataProcessing.h"
#include "Data/Common/Multiply.h"
#include "Data/Common/SingleDataTransfer.h"
#include "Data/Common/SoftwareInterrupt.h"
#include "Data/Common/Swap.h"

#include "Data/Assembler/ConditionCodesPositives.h"
#include "Data/Assembler/Operand2Positives.h"

#include "Arm/Assembler.h"

class AssemblerCommonPositiveTest : public ::testing::TestWithParam<CommonParameter> {};

TEST_P(AssemblerCommonPositiveTest, AssemblerCommonPositiveTestCase) {
    const CommonParameter param = GetParam();
    uint32_t instruction;
    if (rbrown::arm::Assemble(param.assembly, instruction)) {
        ASSERT_EQ(instruction, param.instruction);
    } else {
        ADD_FAILURE();
    }
}

INSTANTIATE_TEST_SUITE_P(
    BlockDataTransferTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_BLOCK_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    BranchTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_BRANCH_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorDataTransferTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorDataOperationTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_DATA_OPERATION_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorRegisterTransfersTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_REGISTER_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    DataProcessingTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_DATA_PROCESSING_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    MultiplyTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_MULTIPLY_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SingleDataTransferTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_SINGLE_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SoftwareInterruptTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_SOFTWARE_INTERRUPT_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SwapTestPositives,
    AssemblerCommonPositiveTest,
    ::testing::ValuesIn(COMMON_SWAP_VALUES)
);

class AssemblerPositiveTest : public ::testing::TestWithParam<AssemblerPositiveParameter> {};

TEST_P(AssemblerPositiveTest, AssemblerCommonPositiveTestCase) {
    const AssemblerPositiveParameter param = GetParam();
    uint32_t instruction;
    if (rbrown::arm::Assemble(param.assembly, instruction)) {
        ASSERT_EQ(instruction, param.instruction);
    } else {
        ADD_FAILURE();
    }
}

INSTANTIATE_TEST_SUITE_P(
    ConditionCodesTestPositives,
    AssemblerPositiveTest,
    ::testing::ValuesIn(ASSEMBLER_CONDITION_CODE_POSITIVE_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    Operand2TestPositives,
    AssemblerPositiveTest,
    ::testing::ValuesIn(ASSEMBLER_OPERAND_2_POSITIVE_VALUES)
);