set(ARM_ASSEMBLER_TEST_SOURCE
    src/Assembler/AssemblerNegativeTest.cpp
    src/Assembler/AssemblerPositiveTest.cpp
)

set(ARM_DISASSEMBLER_TEST_SOURCE
    src/Disassembler/DisassemblerPositiveTest.cpp
)

set(ARM_TEST_CONDITION_CODES_SOURCE
    src/Arm/ConditionCodes/ConditionCodeEQ.cpp
    src/Arm/ConditionCodes/ConditionCodeNE.cpp
    src/Arm/ConditionCodes/ConditionCodeCS.cpp
    src/Arm/ConditionCodes/ConditionCodeCC.cpp
    src/Arm/ConditionCodes/ConditionCodeMI.cpp
    src/Arm/ConditionCodes/ConditionCodePL.cpp
    src/Arm/ConditionCodes/ConditionCodeVS.cpp
    src/Arm/ConditionCodes/ConditionCodeVC.cpp
    src/Arm/ConditionCodes/ConditionCodeHI.cpp
    src/Arm/ConditionCodes/ConditionCodeLS.cpp
    src/Arm/ConditionCodes/ConditionCodeGE.cpp
    src/Arm/ConditionCodes/ConditionCodeLT.cpp
    src/Arm/ConditionCodes/ConditionCodeGT.cpp
    src/Arm/ConditionCodes/ConditionCodeLE.cpp
    src/Arm/ConditionCodes/ConditionCodeAL.cpp
    src/Arm/ConditionCodes/ConditionCodeNV.cpp
)

set(ARM_TEST_DATA_PROCESSING_INSTRUCTION_SHIFT_SOURCE
    src/Arm/DataProcessingInstructionShift/BarrelShifter/BarrelShifterInstructionShiftASR.cpp
    src/Arm/DataProcessingInstructionShift/BarrelShifter/BarrelShifterInstructionShiftLSL.cpp
    src/Arm/DataProcessingInstructionShift/BarrelShifter/BarrelShifterInstructionShiftLSR.cpp
    src/Arm/DataProcessingInstructionShift/BarrelShifter/BarrelShifterInstructionShiftROR.cpp
    src/Arm/DataProcessingInstructionShift/BarrelShifter/BarrelShifterInstructionShiftRRX.cpp
    src/Arm/DataProcessingInstructionShift/Operations/AdcOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/AddOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/AndOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/BicOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/CmnOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/CmpOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/EorOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/MovOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/MvnOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/OrrOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/RsbOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/RscOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/SbcOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/SubOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/TeqOperand2InstructionShift.cpp
    src/Arm/DataProcessingInstructionShift/Operations/TstOperand2InstructionShift.cpp
)

set(ARM_TEST_DATA_PROCESSING_REGISTER_SHIFT_SOURCE
    src/Arm/DataProcessingRegisterShift/BarrelShifter/BarrelShifterRegisterShiftASR.cpp
    src/Arm/DataProcessingRegisterShift/BarrelShifter/BarrelShifterRegisterShiftLSL.cpp
    src/Arm/DataProcessingRegisterShift/BarrelShifter/BarrelShifterRegisterShiftLSR.cpp
    src/Arm/DataProcessingRegisterShift/BarrelShifter/BarrelShifterRegisterShiftROR.cpp
    src/Arm/DataProcessingRegisterShift/Operations/AdcOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/AddOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/AndOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/BicOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/CmnOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/CmpOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/EorOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/MovOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/MvnOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/OrrOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/RsbOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/RscOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/SbcOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/SubOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/TeqOperand2RegisterShift.cpp
    src/Arm/DataProcessingRegisterShift/Operations/TstOperand2RegisterShift.cpp
)

set(ARM_TEST_DATA_PROCESSING_IMMEDIATE_SOURCE
    src/Arm/DataProcessingImmediate/BarrelShifter/BarrelShifterImmediate.cpp
    src/Arm/DataProcessingImmediate/Operations/AdcOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/AddOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/AndOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/BicOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/CmnOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/CmpOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/EorOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/MovOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/MvnOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/OrrOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/RsbOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/RscOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/SbcOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/SubOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/TeqOperand2Immediate.cpp
    src/Arm/DataProcessingImmediate/Operations/TstOperand2Immediate.cpp
)

set(ARM_TEST_MULTIPLY_SOURCE
    src/Arm/Multiply/Operations/Multiply.cpp
    src/Arm/Multiply/Operations/MultiplyAccumulate.cpp
    src/Arm/Multiply/Timing/MultiplyAccumulateTiming.cpp
    src/Arm/Multiply/Timing/MultiplyTiming.cpp
)

set(ARM_TEST_SINGLE_DATA_TRANSFER_IMMEDIATE_SOURCE
    src/Arm/SingleDataTransferImmediate/Load/PostIndexed/LoadByteImmediatePostIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Load/PostIndexed/LoadWordImmediatePostIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Load/PostIndexedUserMode/LoadByteImmediatePostIndexedUserMode.cpp
    src/Arm/SingleDataTransferImmediate/Load/PostIndexedUserMode/LoadWordImmediatePostIndexedUserMode.cpp
    src/Arm/SingleDataTransferImmediate/Load/PreIndexed/LoadByteImmediatePreIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Load/PreIndexed/LoadWordImmediatePreIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Load/PreIndexedWriteback/LoadByteImmediatePreIndexedWriteback.cpp
    src/Arm/SingleDataTransferImmediate/Load/PreIndexedWriteback/LoadWordImmediatePreIndexedWriteback.cpp
    src/Arm/SingleDataTransferImmediate/Store/PostIndexed/StoreWordImmediatePostIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Store/PostIndexed/StoreByteImmediatePostIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Store/PostIndexedUserMode/StoreWordImmediatePostIndexedUserMode.cpp
    src/Arm/SingleDataTransferImmediate/Store/PostIndexedUserMode/StoreByteImmediatePostIndexedUserMode.cpp
    src/Arm/SingleDataTransferImmediate/Store/PreIndexed/StoreByteImmediatePreIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Store/PreIndexed/StoreWordImmediatePreIndexed.cpp
    src/Arm/SingleDataTransferImmediate/Store/PreIndexedWriteback/StoreByteImmediatePreIndexedWriteback.cpp
    src/Arm/SingleDataTransferImmediate/Store/PreIndexedWriteback/StoreWordImmediatePreIndexedWriteback.cpp
)

set(ARM_TEST_SINGLE_DATA_TRANSFER_REGISTER_SOURCE
    src/Arm/SingleDataTransferRegister/Load/PostIndexed/LoadByteRegisterPostIndexed.cpp
    src/Arm/SingleDataTransferRegister/Load/PostIndexed/LoadWordRegisterPostIndexed.cpp
    src/Arm/SingleDataTransferRegister/Load/PostIndexedUserMode/LoadWordRegisterPostIndexedUserMode.cpp
    src/Arm/SingleDataTransferRegister/Load/PostIndexedUserMode/LoadByteRegisterPostIndexedUserMode.cpp
    src/Arm/SingleDataTransferRegister/Load/PreIndexed/LoadByteRegisterPreIndexed.cpp
    src/Arm/SingleDataTransferRegister/Load/PreIndexed/LoadWordRegisterPreIndexed.cpp
    src/Arm/SingleDataTransferRegister/Load/PreIndexedWriteback/LoadByteRegisterPreIndexedWriteback.cpp
    src/Arm/SingleDataTransferRegister/Load/PreIndexedWriteback/LoadWordRegisterPreIndexedWriteback.cpp
    src/Arm/SingleDataTransferRegister/Store/PostIndexed/StoreWordRegisterPostIndexed.cpp
    src/Arm/SingleDataTransferRegister/Store/PostIndexed/StoreByteRegisterPostIndexed.cpp
    src/Arm/SingleDataTransferRegister/Store/PostIndexedUserMode/StoreWordRegisterPostIndexedUserMode.cpp
    src/Arm/SingleDataTransferRegister/Store/PostIndexedUserMode/StoreByteRegisterPostIndexedUserMode.cpp
    src/Arm/SingleDataTransferRegister/Store/PreIndexed/StoreByteRegisterPreIndexed.cpp
    src/Arm/SingleDataTransferRegister/Store/PreIndexed/StoreWordRegisterPreIndexed.cpp
    src/Arm/SingleDataTransferRegister/Store/PreIndexedWriteback/StoreByteRegisterPreIndexedWriteback.cpp
    src/Arm/SingleDataTransferRegister/Store/PreIndexedWriteback/StoreWordRegisterPreIndexedWriteback.cpp
)

set(ARM_TEST_BLOCK_DATA_TRANSFER_SOURCE
    src/Arm/BlockDataTransfer/Load/PostIndexed/LoadMultipleDecrementAfter.cpp
    src/Arm/BlockDataTransfer/Load/PostIndexed/LoadMultipleIncrementAfter.cpp
    src/Arm/BlockDataTransfer/Load/PreIndexed/LoadMultipleDecrementBefore.cpp
    src/Arm/BlockDataTransfer/Load/PreIndexed/LoadMultipleIncrementBefore.cpp
    src/Arm/BlockDataTransfer/Load/PostIndexedWriteback/LoadMultipleDecrementAfterWriteback.cpp
    src/Arm/BlockDataTransfer/Load/PostIndexedWriteback/LoadMultipleIncrementAfterWriteback.cpp
    src/Arm/BlockDataTransfer/Load/PreIndexedWriteback/LoadMultipleDecrementBeforeWriteback.cpp
    src/Arm/BlockDataTransfer/Load/PreIndexedWriteback/LoadMultipleIncrementBeforeWriteback.cpp
    src/Arm/BlockDataTransfer/Store/PostIndexed/StoreMultipleDecrementAfter.cpp
    src/Arm/BlockDataTransfer/Store/PostIndexed/StoreMultipleIncrementAfter.cpp
    src/Arm/BlockDataTransfer/Store/PreIndexed/StoreMultipleDecrementBefore.cpp
    src/Arm/BlockDataTransfer/Store/PreIndexed/StoreMultipleIncrementBefore.cpp
    src/Arm/BlockDataTransfer/Store/PostIndexedWriteback/StoreMultipleDecrementAfterWriteback.cpp
    src/Arm/BlockDataTransfer/Store/PostIndexedWriteback/StoreMultipleIncrementAfterWriteback.cpp
    src/Arm/BlockDataTransfer/Store/PreIndexedWriteback/StoreMultipleDecrementBeforeWriteback.cpp
    src/Arm/BlockDataTransfer/Store/PreIndexedWriteback/StoreMultipleIncrementBeforeWriteback.cpp

)

set(ARM_TEST_BRANCH_SOURCE
    src/Arm/Branch/Branch.cpp
    src/Arm/Branch/BranchWithLink.cpp
)

set(ARM_TEST_COPROCESSOR_DATA_TRANSFER_SOURCE
    src/Arm/CoprocessorDataTransfer/LoadCoprocessorLongForm.cpp
    src/Arm/CoprocessorDataTransfer/LoadCoprocessorShortForm.cpp
    src/Arm/CoprocessorDataTransfer/StoreCoprocessorLongForm.cpp
    src/Arm/CoprocessorDataTransfer/StoreCoprocessorShortForm.cpp
)

set(ARM_TEST_SOFTWARE_INTERRUPT_SOURCE
    src/Arm/SoftwareInterrupt/SoftwareInterrupt.cpp
)

set(ARM_TEST_COPROCESSOR_REGISTER_TRANSFER_SOURCE
    src/Arm/CoprocessorRegisterTransfer/MoveRegisterFromCoprocessor.cpp
    src/Arm/CoprocessorRegisterTransfer/MoveRegisterToCoprocessor.cpp
)

set(ARM_TEST_COPROCESSOR_DATA_OPERATION_SOURCE
    src/Arm/CoprocessorDataOperation/CoprocessorDataOperation.cpp
)

set(ARM_TEST_EXCEPTIONS_SOURCE
    src/Arm/Exceptions/AddressException.cpp
    src/Arm/Exceptions/DataAbort.cpp
    src/Arm/Exceptions/FastInterrupt.cpp
    src/Arm/Exceptions/Interrupt.cpp
    src/Arm/Exceptions/PrefetchAbort.cpp
    src/Arm/Exceptions/UndefinedInstruction.cpp
)

set(ARM_TEST_SOURCE
    src/Arm/ArmTest.cpp
    ${ARM_TEST_CONDITION_CODES_SOURCE}
    ${ARM_TEST_DATA_PROCESSING_INSTRUCTION_SHIFT_SOURCE}
    ${ARM_TEST_DATA_PROCESSING_REGISTER_SHIFT_SOURCE}
    ${ARM_TEST_DATA_PROCESSING_IMMEDIATE_SOURCE}
    ${ARM_TEST_MULTIPLY_SOURCE}
    ${ARM_TEST_SINGLE_DATA_TRANSFER_IMMEDIATE_SOURCE}
    ${ARM_TEST_SINGLE_DATA_TRANSFER_REGISTER_SOURCE}
    ${ARM_TEST_BLOCK_DATA_TRANSFER_SOURCE}
    ${ARM_TEST_BRANCH_SOURCE}
    ${ARM_TEST_COPROCESSOR_DATA_TRANSFER_SOURCE}
    ${ARM_TEST_SOFTWARE_INTERRUPT_SOURCE}
    ${ARM_TEST_COPROCESSOR_REGISTER_TRANSFER_SOURCE}
    ${ARM_TEST_COPROCESSOR_DATA_OPERATION_SOURCE}
    ${ARM_TEST_EXCEPTIONS_SOURCE}
)

set(ARM_TEST_SOURCE
    ${ARM_ASSEMBLER_TEST_SOURCE}
    ${ARM_DISASSEMBLER_TEST_SOURCE}
    ${ARM_TEST_SOURCE}
)

add_executable(ArmTest ${ARM_TEST_SOURCE})

target_include_directories(
    ArmTest
    PRIVATE
    include
)

target_compile_options(
    ArmTest
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)

target_link_libraries(ArmTest PRIVATE Arm gmock gtest gtest_main)

add_test(NAME ArmTest COMMAND ArmTest)
