#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"
#include "Common/Shifts.h"
#include "Common/SignExtend.h"

using namespace rbrown::arm;

struct Assembler::BranchInstruction {
    uint32_t conditionCode;
    uint32_t l;
    uint32_t offset;
};

constexpr auto EncodeBranchInstructionOffset(uint32_t n, uint32_t& offset) -> bool {
    const auto m = ArithmeticShiftRight(n - 8u, 2u);
    if (SignExtend24(m) == m) {
        offset = m;
        return true;
    }
    return false;
}

auto Assembler::AssembleB(SourceLine& source, uint32_t& result) -> bool {
    return AssembleBranchInstruction(source, result);
}

auto Assembler::AssembleBranchInstruction(SourceLine& source, uint32_t& result) -> bool {
    BranchInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
    };
    if (AssembleBranchInstruction(source, instruction)) {
        result = EncodeBranchInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleBranchInstruction(SourceLine& source, BranchInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleBranchInstructionOffset(source, instruction);
    } else {
        if (source.MatchAndAdvance('L')) {
            instruction.l = 1u;
            if (source.MatchWhitespace()) {
                return AssembleBranchInstructionOffset(source, instruction);
            } else if (AssembleConditionCode(source, instruction.conditionCode)) {
                return AssembleBranchInstructionOffset(source, instruction);
            }
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            return AssembleBranchInstructionOffset(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleBranchInstructionOffset(SourceLine& source, BranchInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        uint32_t n;
        if (AssembleBranchOffsetNumber(source, n)) {
            return EncodeBranchInstructionOffset(n, instruction.offset);
        }
    }
    return false;
}

auto Assembler::EncodeBranchInstruction(const BranchInstruction& instruction) -> uint32_t {
    const auto& [ conditionCode, l, offset ] = instruction;
    return ::EncodeBranchInstruction(conditionCode, l, offset);
}
