#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::SoftwareInterruptInstruction {
    uint32_t conditionCode;
    uint32_t comment;
};

auto Assembler::AssembleSwi(SourceLine& source, uint32_t& result) -> bool {
    return AssembleSoftwareInterruptInstruction(source, result);
}

auto Assembler::AssembleSoftwareInterruptInstruction(SourceLine& source, uint32_t& result) -> bool {
    SoftwareInterruptInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
    };
    if (AssembleSoftwareInterruptInstruction(source, instruction)) {
        result = EncodeSoftwareInterruptInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleSoftwareInterruptInstruction(SourceLine& source, SoftwareInterruptInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleSoftwareInterruptInstructionComment(source, instruction);
    } else if (AssembleConditionCode(source, instruction.conditionCode)) {
        return AssembleSoftwareInterruptInstructionComment(source, instruction);
    }
    return false;
}


auto Assembler::AssembleSoftwareInterruptInstructionComment(SourceLine& source, SoftwareInterruptInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        return AssembleSoftwareInterruptCommentNumber(source, instruction.comment);
    }
    return false;
}

auto Assembler::EncodeSoftwareInterruptInstruction(const SoftwareInterruptInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, comment] = instruction;
    return ::EncodeSoftwareInterruptInstruction(conditionCode, comment);
}
