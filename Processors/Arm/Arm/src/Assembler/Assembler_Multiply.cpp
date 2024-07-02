#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::MultiplyInstruction {
    uint32_t conditionCode;
    uint32_t accumulate;
    uint32_t s;
    uint32_t rd;
    uint32_t rm;
    uint32_t rs;
    uint32_t rn;
};

auto Assembler::AssembleMla(SourceLine& source, uint32_t& result) -> bool {
    return AssembleMultiplyAccumulateInstruction(source, result);
}

auto Assembler::AssembleMul(SourceLine& source, uint32_t& result) -> bool {
    return AssembleMultiplyInstruction(source, result);
}

auto Assembler::AssembleMultiplyAccumulateInstruction(SourceLine& source, uint32_t& result) -> bool {
    MultiplyInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .accumulate = 1u
    };
    if (AssembleMultiplyAccumulateInstruction(source, instruction)) {
        result = EncodeMultiplyInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleMultiplyAccumulateInstruction(SourceLine& source, MultiplyInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleMultiplyAccumulateArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('S')) {
            instruction.s = 1u;
            return AssembleMultiplyAccumulateArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('S')) {
                instruction.s = 1u;
            }
            return AssembleMultiplyAccumulateArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleMultiplyAccumulateArguments(SourceLine& source, MultiplyInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        if (AssembleWhitespaceCommaRegisterNumber(source, instruction.rm)) {
            if (AssembleWhitespaceCommaRegisterNumber(source, instruction.rs)) {
                return AssembleWhitespaceCommaRegisterNumber(source, instruction.rn);
            }
        }
    }
    return false;
}

auto Assembler::AssembleMultiplyInstruction(SourceLine& source, uint32_t& result) -> bool {
    MultiplyInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
    };
    if (AssembleMultiplyInstruction(source, instruction)) {
        result = EncodeMultiplyInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleMultiplyInstruction(SourceLine& source, MultiplyInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleMultiplyArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('S')) {
            instruction.s = 1u;
            return AssembleMultiplyArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('S')) {
                instruction.s = 1u;
            }
            return AssembleMultiplyArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleMultiplyArguments(SourceLine& source, MultiplyInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        if (AssembleWhitespaceCommaRegisterNumber(source, instruction.rm)) {
            return AssembleWhitespaceCommaRegisterNumber(source, instruction.rs);
        }
    }
    return false;
}

auto Assembler::EncodeMultiplyInstruction(const MultiplyInstruction& instruction) -> uint32_t {
    const auto&[conditionCode, accumulate, s, rd, rm, rs, rn] = instruction;
    return ::EncodeMultiplyInstruction(
        conditionCode,
        accumulate,
        s,
        rd, rn, rs, rm);
}
