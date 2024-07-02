#include <gtest/gtest.h>

#include "Arm/Disassembler.h"

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

class DisassemblerPositiveTest : public ::testing::TestWithParam<CommonParameter> {};

TEST_P(DisassemblerPositiveTest, DisassemblerPositiveTestCase) {
    const CommonParameter param = GetParam();
    std::string assembly = rbrown::arm::Disassemble(param.instruction);
    ASSERT_STRCASEEQ(assembly.c_str(), param.assembly);
}

INSTANTIATE_TEST_SUITE_P(
    BlockDataTransferTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_BLOCK_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    BranchTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_BRANCH_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorDataOperationTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_DATA_OPERATION_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorDataTransferTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    CoprocessorRegisterTransfersTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_COPROCESSOR_REGISTER_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    DataProcessingTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_DATA_PROCESSING_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    MultiplyTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_MULTIPLY_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SingleDataTransferTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_SINGLE_DATA_TRANSFER_VALUES)
);

INSTANTIATE_TEST_SUITE_P(
    SoftwareInterruptTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_SOFTWARE_INTERRUPT_VALUES)
);


INSTANTIATE_TEST_SUITE_P(
    SwapTestPositives,
    DisassemblerPositiveTest,
    ::testing::ValuesIn(COMMON_SWAP_VALUES)
);