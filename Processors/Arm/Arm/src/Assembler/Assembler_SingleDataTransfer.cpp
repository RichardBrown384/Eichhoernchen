#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::SingleDataTransferInstruction {
    uint32_t conditionCode;
    uint32_t immediate;
    uint32_t p;
    uint32_t u;
    uint32_t b;
    uint32_t w;
    uint32_t l;
    uint32_t rn;
    uint32_t rd;
    uint32_t offset;
    uint32_t t;
};

auto Assembler::AssembleLdr(SourceLine& source, uint32_t& result) -> bool {
    return AssembleLoadSingleDataTransferInstruction(source, result);
}

auto Assembler::AssembleStr(SourceLine& source, uint32_t& result) -> bool {
    return AssembleStoreSingleDataTransferInstruction(source, result);
}

auto Assembler::AssembleLoadSingleDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    SingleDataTransferInstruction instruction = {
            .conditionCode = CONDITION_CODE_AL,
            .immediate = 1u,
            .p = 1u,
            .u = 1u,
            .l = 1u
    };
    if (AssembleSingleDataTransferInstruction(source, instruction)) {
        result = EncodeSingleDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleStoreSingleDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    SingleDataTransferInstruction instruction = {
            .conditionCode = CONDITION_CODE_AL,
            .immediate = 1u,
            .p = 1u,
            .u = 1u
    };
    if (AssembleSingleDataTransferInstruction(source, instruction)) {
        result = EncodeSingleDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleSingleDataTransferInstruction(SourceLine& source, SingleDataTransferInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleSingleDataTransferArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('B')) {
            instruction.b = 1u;
            if (source.MatchAndAdvance('T')) {
                instruction.t = 1u;
            }
            return AssembleSingleDataTransferArguments(source, instruction);
        } else if (source.MatchAndAdvance('T')) {
            instruction.t = 1u;
            return AssembleSingleDataTransferArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('B')) {
                instruction.b = 1u;
            }
            if (source.MatchAndAdvance('T')) {
                instruction.t = 1u;
            }
            return AssembleSingleDataTransferArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleSingleDataTransferArguments(SourceLine& source, SingleDataTransferInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        return AssembleSingleDataTransferAddress(source, instruction);
    }
    return false;
}

auto Assembler::AssembleSingleDataTransferAddress(SourceLine& source, SingleDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        if (source.MatchAndAdvance('[')) {
            if (AssembleRegisterNumber(source, instruction.rn)) {
                if (source.MatchAndAdvance(']')) {
                    if (source.Match(',')) {
                        if (AssembleSingleDataTransferOffsetOrOperand2(source, instruction)) {
                            instruction.p = 0u; // post indexed
                            instruction.w = instruction.t;
                            return true;
                        }
                    } else {
                        instruction.immediate = 0u; // offset is immediate value
                        return AssembleSingleDataTransferWriteBack(source, instruction);
                    }
                } else if (AssembleSingleDataTransferOffsetOrOperand2(source, instruction)) {
                    if (source.MatchAndAdvance(']')) {
                        return AssembleSingleDataTransferWriteBack(source, instruction);
                    }
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleSingleDataTransferOffsetOrOperand2(SourceLine& source, SingleDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        if (source.Match('#')) {
            instruction.immediate = 0u; // offset is immediate value
            return AssembleTransferOffsetNumber(source, instruction.u, instruction.offset);
        } else {
            if (source.MatchAndAdvance('-')) {
                instruction.u = 0u;
            }
            if (AssembleShiftTypeOperand2(source, instruction.offset)) {
                // Make sure this isn't a register specified shift
                return (instruction.offset & 0x10u) == 0u;
            }
        }
    }
    return false;
}

auto Assembler::AssembleSingleDataTransferWriteBack(SourceLine& source, SingleDataTransferInstruction& instruction) -> bool {
    if (instruction.t) {
        if (source.MatchAndAdvance('!')) {
            return false;
        }
        instruction.p = 0u; // post indexed
        instruction.w = 1u; // write back
    } else {
        if (source.MatchAndAdvance('!')) {
            instruction.w = 1u; // write back
        }
    }
    return true;
}

auto Assembler::EncodeSingleDataTransferInstruction(const SingleDataTransferInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, immediate, p, u, b, w, l, rn, rd, offset, t] = instruction;
    return ::EncodeSingleDataTransferInstruction(
        conditionCode, immediate,
        p, u, b, w,
        l,
        rn, rd, offset);
}