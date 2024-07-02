#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::BlockDataTransferInstruction {
    uint32_t conditionCode;
    uint32_t p;
    uint32_t u;
    uint32_t s;
    uint32_t w;
    uint32_t l;
    uint32_t rn;
    uint32_t registerList;
};

auto Assembler::AssembleLdm(SourceLine& source, uint32_t& result) -> bool {
    return AssembleLoadBlockDataTransferInstruction(source, result);
}

auto Assembler::AssembleStm(SourceLine& source, uint32_t& result) -> bool {
    return AssembleStoreBlockDataTransferInstruction(source, result);
}

auto Assembler::AssembleLoadBlockDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    BlockDataTransferInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .l = 1u
    };
    if (AssembleBlockDataTransferInstruction(source, instruction)) {
        result = EncodeBlockDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleStoreBlockDataTransferInstruction(SourceLine& source, uint32_t& result) -> bool {
    BlockDataTransferInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL
    };
    if (AssembleBlockDataTransferInstruction(source, instruction)) {
        result = EncodeBlockDataTransferInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferInstruction(SourceLine& source, BlockDataTransferInstruction& instruction) -> bool {
    if (source.Match('I')) {
        if (AssembleBlockDataTransferIncrement(source, instruction)) {
            return AssembleBlockDataTransferArguments(source, instruction);
        }
    } else {
        if (source.Match('D')) {
            if (AssembleBlockDataTransferDecrement(source, instruction)) {
                return AssembleBlockDataTransferArguments(source, instruction);
            }
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.Match('I')) {
                if (AssembleBlockDataTransferIncrement(source, instruction)) {
                    return AssembleBlockDataTransferArguments(source, instruction);
                }
            } else if (source.Match('D')) {
                if (AssembleBlockDataTransferDecrement(source, instruction)) {
                    return AssembleBlockDataTransferArguments(source, instruction);
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferIncrement(SourceLine& source, BlockDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvance('I')) {
        if (source.MatchAndAdvance('A')) {
            instruction.u = 1u;
            return true;
        } else {
            if (source.MatchAndAdvance('B')) {
                instruction.u = 1u;
                instruction.p = 1u;
                return true;
            }
        }
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferDecrement(SourceLine& source, BlockDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvance('D')) {
        if (source.MatchAndAdvance('A')) {
            return true;
        } else {
            if (source.MatchAndAdvance('B')) {
                instruction.p = 1u;
                return true;
            }
        }
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferArguments(SourceLine& source, BlockDataTransferInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleRegisterNumber(source, instruction.rn)) {
            if (source.MatchAndAdvance('!')) {
                instruction.w = 1u;
            }
            if (source.MatchAndAdvanceCommaWhitespace()) {
                if (AssembleBlockDataTransferRegisterList(source, instruction.registerList)) {
                    if (source.MatchAndAdvance('^')) {
                        instruction.s = 1u;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferRegisterList(SourceLine& source, uint32_t& registerList) -> bool {
    if (source.MatchAndAdvance('{')) {
        if (AssembleBlockDataTransferRegisterRange(source, registerList)) {
            for (auto i = 1u; i < 16u; i++) {
                if (source.Match(',')) {
                    if (source.MatchAndAdvanceCommaWhitespace()) {
                        if (!AssembleBlockDataTransferRegisterRange(source, registerList)) {
                            return false;
                        }
                    }
                } else {
                    break;
                }
            }
            return source.MatchAndAdvance('}');
        }
    }
    return false;
}

auto Assembler::AssembleBlockDataTransferRegisterRange(SourceLine& source, uint32_t& registerList) -> bool {
    uint32_t start, end;
    if (AssembleRegisterNumber(source, start)) {
        if (source.MatchAndAdvance('-')) {
            if (AssembleRegisterNumber(source, end)) {
                if (start < end) {
                    registerList |= (1u << (end + 1u)) - (1u << start);
                    return true;
                }
            }
        } else {
            registerList |= 1u << start;
            return true;
        }
    }
    return false;
}

auto Assembler::EncodeBlockDataTransferInstruction(const BlockDataTransferInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, p, u, s, w, l, rn, registerList] = instruction;
    return ::EncodeBlockDataTransferInstruction(
        conditionCode,
        p, u, s, w,
        l,
        rn,
        registerList);
}
