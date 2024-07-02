#pragma once

#include <cstdint>
#include <string>

namespace rbrown::arm {

class SourceLine;

class Assembler {
public:
    Assembler() = delete;
    static auto Assemble(const std::string&, uint32_t&) -> bool;
private:
    struct DataProcessingInstruction;
    struct MultiplyInstruction;
    struct SwapInstruction;
    struct SingleDataTransferInstruction;
    struct BlockDataTransferInstruction;
    struct BranchInstruction;
    struct CoprocessorDataTransferInstruction;
    struct CoprocessorDataOperationsInstruction;
    struct CoprocessorRegisterTransfersInstruction;
    struct SoftwareInterruptInstruction;

    static auto Assemble(SourceLine&, uint32_t&) -> bool;

    // Condition codes
    static auto AssembleConditionCode(SourceLine&, uint32_t&) -> bool;

    // Numbers
    static auto AssembleRegisterNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleWhitespaceRegisterNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleWhitespaceCommaRegisterNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleShiftAmountNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleImmediateNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleTransferOffsetNumber(SourceLine&, uint32_t&, uint32_t&) -> bool;
    static auto AssembleBranchOffsetNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorRegisterNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorOpcodeNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorInformationNumber(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSoftwareInterruptCommentNumber(SourceLine&, uint32_t&) -> bool;

    // Operand 2
    static auto AssembleShiftTypeOperand2(SourceLine&, uint32_t&) -> bool;
    static auto AssembleShiftTypeOperand2Shift(SourceLine&, uint32_t&) -> bool;
    static auto AssembleLSL(SourceLine&, uint32_t&) -> bool;
    static auto AssembleLSR(SourceLine&, uint32_t&) -> bool;
    static auto AssembleASR(SourceLine&, uint32_t&) -> bool;
    static auto AssembleROR(SourceLine&, uint32_t&) -> bool;
    static auto AssembleRRX(SourceLine&, uint32_t&) -> bool;
    static auto AssembleImmediateTypeOperand2(SourceLine&, uint32_t&) -> bool;

    // Coprocessors
    static auto AssembleCoprocessorCoprocessorArgument(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorOpcodeArgument(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorProcessorRegisterArgument(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorCoprocessorRegisterArgument(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorInformationArgument(SourceLine&, uint32_t&) -> bool;

    // Data processing
    static auto AssembleAdc(SourceLine&, uint32_t&) -> bool;
    static auto AssembleAdd(SourceLine&, uint32_t&) -> bool;
    static auto AssembleAnd(SourceLine&, uint32_t&) -> bool;
    static auto AssembleBic(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCmp(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCmn(SourceLine&, uint32_t&) -> bool;
    static auto AssembleEor(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMov(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMvn(SourceLine&, uint32_t&) -> bool;
    static auto AssembleOrr(SourceLine&, uint32_t&) -> bool;
    static auto AssembleRsb(SourceLine&, uint32_t&) -> bool;
    static auto AssembleRsc(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSbc(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSub(SourceLine&, uint32_t&) -> bool;
    static auto AssembleTeq(SourceLine&, uint32_t&) -> bool;
    static auto AssembleTst(SourceLine&, uint32_t&) -> bool;

    static auto AssembleDataProcessingInstruction(SourceLine&, uint32_t, uint32_t&) -> bool;
    static auto AssembleDataProcessingInstruction(SourceLine&, DataProcessingInstruction&) -> bool;
    static auto AssembleDataProcessingInstructionArguments(SourceLine&, DataProcessingInstruction&) -> bool;

    static auto AssembleCompareDataProcessingInstruction(SourceLine&, uint32_t, uint32_t&) -> bool;
    static auto AssembleCompareDataProcessingInstruction(SourceLine&, DataProcessingInstruction&) -> bool;
    static auto AssembleCompareDataProcessingInstructionArguments(SourceLine&, DataProcessingInstruction&) -> bool;

    static auto AssembleMoveDataProcessingInstruction(SourceLine&, uint32_t, uint32_t&) -> bool;
    static auto AssembleMoveDataProcessingInstruction(SourceLine&, DataProcessingInstruction&) -> bool;
    static auto AssembleMoveDataProcessingInstructionArguments(SourceLine&, DataProcessingInstruction&) -> bool;

    static auto AssembleDataProcessingInstructionOperand2(SourceLine&, DataProcessingInstruction&) -> bool;
    static auto EncodeDataProcessingInstruction(const DataProcessingInstruction&) -> uint32_t;

    // Multiply Accumulate + Multiply
    static auto AssembleMla(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMul(SourceLine&, uint32_t&) -> bool;

    static auto AssembleMultiplyAccumulateInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMultiplyAccumulateInstruction(SourceLine&, MultiplyInstruction&) -> bool;
    static auto AssembleMultiplyAccumulateArguments(SourceLine&, MultiplyInstruction&) -> bool;
    static auto AssembleMultiplyInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMultiplyInstruction(SourceLine&, MultiplyInstruction&) -> bool;
    static auto AssembleMultiplyArguments(SourceLine&, MultiplyInstruction&) -> bool;
    static auto EncodeMultiplyInstruction(const MultiplyInstruction&) -> uint32_t;

    // Swap
    static auto AssembleSwp(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSwapInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSwapInstruction(SourceLine&, SwapInstruction&) -> bool;
    static auto AssembleSwapArguments(SourceLine&, SwapInstruction&) -> bool;
    static auto AssembleSwapDestinationRegisterNumber(SourceLine&, uint32_t&) -> bool;
    static auto EncodeSwapInstruction(const SwapInstruction&) -> uint32_t;

    // Single Data Transfer
    static auto AssembleLdr(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStr(SourceLine&, uint32_t&) -> bool;

    static auto AssembleLoadSingleDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStoreSingleDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSingleDataTransferInstruction(SourceLine&, SingleDataTransferInstruction&) -> bool;
    static auto AssembleSingleDataTransferArguments(SourceLine&, SingleDataTransferInstruction&) -> bool;
    static auto AssembleSingleDataTransferAddress(SourceLine&, SingleDataTransferInstruction&) -> bool;
    static auto AssembleSingleDataTransferOffsetOrOperand2(SourceLine&, SingleDataTransferInstruction&) -> bool;
    static auto AssembleSingleDataTransferWriteBack(SourceLine&, SingleDataTransferInstruction&) -> bool;
    static auto EncodeSingleDataTransferInstruction(const SingleDataTransferInstruction&) -> uint32_t;

    // Block Data Transfer
    static auto AssembleLdm(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStm(SourceLine&, uint32_t&) -> bool;

    static auto AssembleLoadBlockDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStoreBlockDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleBlockDataTransferInstruction(SourceLine&, BlockDataTransferInstruction&) -> bool;
    static auto AssembleBlockDataTransferIncrement(SourceLine&, BlockDataTransferInstruction&) -> bool;
    static auto AssembleBlockDataTransferDecrement(SourceLine&, BlockDataTransferInstruction&) -> bool;
    static auto AssembleBlockDataTransferArguments(SourceLine&, BlockDataTransferInstruction&) -> bool;
    static auto AssembleBlockDataTransferRegisterList(SourceLine&, uint32_t&) -> bool;
    static auto AssembleBlockDataTransferRegisterRange(SourceLine&, uint32_t&) -> bool;
    static auto EncodeBlockDataTransferInstruction(const BlockDataTransferInstruction&) -> uint32_t;

    // Branch
    static auto AssembleB(SourceLine&, uint32_t&) -> bool;

    static auto AssembleBranchInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleBranchInstruction(SourceLine&, BranchInstruction&) -> bool;
    static auto AssembleBranchInstructionOffset(SourceLine&, BranchInstruction&) -> bool;
    static auto EncodeBranchInstruction(const BranchInstruction&) -> uint32_t;

    // Coprocessor Data Transfer
    static auto AssembleLdc(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStc(SourceLine&, uint32_t&) -> bool;

    static auto AssembleLoadCoprocessorDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStoreCoprocessorDataTransferInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorDataTransferInstruction(SourceLine&, CoprocessorDataTransferInstruction&) -> bool;
    static auto AssembleCoprocessorDataTransferInstructionArguments(SourceLine&, CoprocessorDataTransferInstruction&) -> bool;
    static auto AssembleCoprocessorDataTransferInstructionAddress(SourceLine&, CoprocessorDataTransferInstruction&) -> bool;
    static auto AssembleCoprocessorDataTransferInstructionOffset(SourceLine&, CoprocessorDataTransferInstruction&) -> bool;
    static auto EncodeCoprocessorDataTransferInstruction(const CoprocessorDataTransferInstruction&) -> uint32_t;

    // Coprocessor Data Operations
    static auto AssembleCdp(SourceLine&, uint32_t&) -> bool;

    static auto AssembleCoprocessorDataOperationsInstruction(SourceLine& source, uint32_t& result) -> bool;
    static auto AssembleCoprocessorDataOperationsInstruction(SourceLine&, CoprocessorDataOperationsInstruction&) -> bool;
    static auto AssembleCoprocessorDataOperationsInstructionArguments(SourceLine&, CoprocessorDataOperationsInstruction&) -> bool;
    static auto AssembleCoprocessorDataOperationsInstructionRegisterNumbers(SourceLine&, CoprocessorDataOperationsInstruction&) -> bool;
    static auto EncodeCoprocessorDataOperationsInstruction(const CoprocessorDataOperationsInstruction& instruction) -> uint32_t;

    // Coprocessor Register Transfer
    static auto AssembleMcr(SourceLine&, uint32_t&) -> bool;
    static auto AssembleMrc(SourceLine&, uint32_t&) -> bool;

    static auto AssembleLoadCoprocessorRegisterTransfersInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleStoreCoprocessorRegisterTransfersInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleCoprocessorRegisterTransfersInstruction(SourceLine&, CoprocessorRegisterTransfersInstruction&) -> bool;
    static auto AssembleCoprocessorRegisterTransfersInstructionArguments(SourceLine&, CoprocessorRegisterTransfersInstruction&) -> bool;
    static auto AssembleCoprocessorRegisterTransfersInstructionRegisterNumbers(SourceLine&, CoprocessorRegisterTransfersInstruction&) -> bool;
    static auto EncodeCoprocessorRegisterTransfersInstruction(const CoprocessorRegisterTransfersInstruction&) -> uint32_t;

    // Software Interrupts
    static auto AssembleSwi(SourceLine&, uint32_t&) -> bool;

    static auto AssembleSoftwareInterruptInstruction(SourceLine&, uint32_t&) -> bool;
    static auto AssembleSoftwareInterruptInstruction(SourceLine&, SoftwareInterruptInstruction&) -> bool;
    static auto AssembleSoftwareInterruptInstructionComment(SourceLine&, SoftwareInterruptInstruction&) -> bool;
    static auto EncodeSoftwareInterruptInstruction(const SoftwareInterruptInstruction&) -> uint32_t;
};

}