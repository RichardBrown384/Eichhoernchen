#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::SwapInstruction {
    uint32_t conditionCode;
    uint32_t b;
    uint32_t rn;
    uint32_t rd;
    uint32_t rm;
};

auto Assembler::AssembleSwp(SourceLine& source, uint32_t& result) -> bool {
    return AssembleSwapInstruction(source, result);
}

auto Assembler::AssembleSwapInstruction(SourceLine& source, uint32_t& result) -> bool {
    SwapInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
    };
    if (AssembleSwapInstruction(source, instruction)) {
        result = EncodeSwapInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleSwapInstruction(SourceLine& source, SwapInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleSwapArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('B')) {
            instruction.b = 1u;
            return AssembleSwapArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('B')) {
                instruction.b = 1u;
            }
            return AssembleSwapArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleSwapArguments(SourceLine& source, SwapInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        if (AssembleWhitespaceCommaRegisterNumber(source, instruction.rm)) {
            return AssembleSwapDestinationRegisterNumber(source, instruction.rn);
        }
    }
    return false;
}

auto Assembler::AssembleSwapDestinationRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        if (source.MatchAndAdvance('[')) {
            if (AssembleRegisterNumber(source, r)) {
                return source.MatchAndAdvance(']');
            }
        }
    }
    return false;
}

auto Assembler::EncodeSwapInstruction(const SwapInstruction& instruction) -> uint32_t {
    const auto& [condition_code, b, rn, rd, rm] = instruction;
    return ::EncodeSwapInstruction(condition_code, b, rn, rd, rm);
}