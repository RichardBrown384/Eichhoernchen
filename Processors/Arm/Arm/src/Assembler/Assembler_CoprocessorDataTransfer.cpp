#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::CoprocessorDataTransferInstruction {
    uint32_t conditionCode;
    uint32_t p;
    uint32_t u;
    uint32_t n;
    uint32_t w;
    uint32_t l;
    uint32_t rn;
    uint32_t crd;
    uint32_t cpn;
    uint32_t offset;
};

auto Assembler::AssembleLdc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleLoadCoprocessorDataTransferInstruction(source, result);
}

auto Assembler::AssembleStc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleStoreCoprocessorDataTransferInstruction(source, result);
}

auto Assembler::AssembleLoadCoprocessorDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    CoprocessorDataTransferInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .p = 1u,
        .u = 1u,
        .l = 1u
    };
    if (AssembleCoprocessorDataTransferInstruction(source, instruction)) {
        result = EncodeCoprocessorDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleStoreCoprocessorDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    CoprocessorDataTransferInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .p = 1u,
        .u = 1u
    };
    if (AssembleCoprocessorDataTransferInstruction(source, instruction)) {
        result = EncodeCoprocessorDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataTransferInstruction(SourceLine& source, CoprocessorDataTransferInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleCoprocessorDataTransferInstructionArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('L')) {
            instruction.n = 1u;
            return AssembleCoprocessorDataTransferInstructionArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchWhitespace()) {
                return AssembleCoprocessorDataTransferInstructionArguments(source, instruction);
            } else if (source.MatchAndAdvance('L')) {
                instruction.n = 1u;
                return AssembleCoprocessorDataTransferInstructionArguments(source, instruction);
            }
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataTransferInstructionArguments(SourceLine& source, CoprocessorDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleCoprocessorNumber(source, instruction.cpn)) {
            if (source.MatchAndAdvanceCommaWhitespace()) {
                if (AssembleCoprocessorRegisterNumber(source,instruction.crd)) {
                    if (source.MatchAndAdvanceCommaWhitespace()) {
                        return AssembleCoprocessorDataTransferInstructionAddress(source, instruction);
                    }
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataTransferInstructionAddress(SourceLine& source, CoprocessorDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvance('[')) {
        if (AssembleRegisterNumber(source, instruction.rn)) {
            if (source.MatchAndAdvance(']')) {
                if (source.MatchAndAdvanceCommaWhitespace()) {
                    if (AssembleCoprocessorDataTransferInstructionOffset(source, instruction)) {
                        instruction.p = 0u;
                        instruction.w = 1u;
                        return true;
                    }
                } else {
                    if (source.MatchAndAdvance('!')) {
                        instruction.w = 1u;
                    }
                    return true;
                }
            } else {
                if (source.MatchAndAdvanceCommaWhitespace()) {
                    if (AssembleCoprocessorDataTransferInstructionOffset(source, instruction)) {
                        if (source.MatchAndAdvance(']')) {
                            if (source.MatchAndAdvance('!')) {
                                instruction.w = 1u;
                            }
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataTransferInstructionOffset(SourceLine& source, CoprocessorDataTransferInstruction& instruction) -> bool {
    uint32_t u, offset;
    if (AssembleTransferOffsetNumber(source, u, offset)) {
        if ((offset & ~(0xFFu << 2u)) == 0u) {
            instruction.u = u;
            instruction.offset = offset >> 2u;
            return true;
        }
    }
    return false;
}

auto Assembler::EncodeCoprocessorDataTransferInstruction(const CoprocessorDataTransferInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, p, u, n, w, l, rn, crd, cpn, offset] = instruction;
    return ::EncodeCoprocessorDataTransferInstruction(
        conditionCode,
        p, u, n, w,
        l,
        rn, crd, cpn,
        offset);
}
