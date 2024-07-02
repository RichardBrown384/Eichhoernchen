#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::CoprocessorRegisterTransfersInstruction {
    uint32_t conditionCode;
    uint32_t cpOpc;
    uint32_t l;
    uint32_t crn;
    uint32_t rd;
    uint32_t cpn;
    uint32_t cp;
    uint32_t crm;
};

auto Assembler::AssembleMcr(SourceLine& source, uint32_t& result) -> bool {
    return AssembleStoreCoprocessorRegisterTransfersInstruction(source, result);
}

auto Assembler::AssembleMrc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleLoadCoprocessorRegisterTransfersInstruction(source, result);
}

auto Assembler::AssembleLoadCoprocessorRegisterTransfersInstruction(SourceLine& source, uint32_t& result) -> bool {
    CoprocessorRegisterTransfersInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .l = 1u
    };
    if (AssembleCoprocessorRegisterTransfersInstruction(source, instruction)) {
        result = EncodeCoprocessorRegisterTransfersInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleStoreCoprocessorRegisterTransfersInstruction(SourceLine& source, uint32_t& result) -> bool {
    CoprocessorRegisterTransfersInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL
    };
    if (AssembleCoprocessorRegisterTransfersInstruction(source, instruction)) {
        result = EncodeCoprocessorRegisterTransfersInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleCoprocessorRegisterTransfersInstruction(SourceLine& source, CoprocessorRegisterTransfersInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleCoprocessorRegisterTransfersInstructionArguments(source, instruction);
    } else if (AssembleConditionCode(source, instruction.conditionCode)) {
        return AssembleCoprocessorRegisterTransfersInstructionArguments(source, instruction);
    }
    return false;
}

auto Assembler::AssembleCoprocessorRegisterTransfersInstructionArguments(SourceLine& source, CoprocessorRegisterTransfersInstruction& instruction) -> bool {
    if (AssembleCoprocessorCoprocessorArgument(source, instruction.cpn)) {
        if (AssembleCoprocessorOpcodeArgument(source, instruction.cpOpc)) {
            if (AssembleCoprocessorRegisterTransfersInstructionRegisterNumbers(source, instruction)) {
                return AssembleCoprocessorInformationArgument(source, instruction.cp);
            }
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorRegisterTransfersInstructionRegisterNumbers(SourceLine& source, CoprocessorRegisterTransfersInstruction& instruction) -> bool {
    if (AssembleCoprocessorProcessorRegisterArgument(source, instruction.rd)) {
        if (AssembleCoprocessorCoprocessorRegisterArgument(source, instruction.crn)) {
            return AssembleCoprocessorCoprocessorRegisterArgument(source, instruction.crm);
        }
    }
    return false;
}

auto Assembler::EncodeCoprocessorRegisterTransfersInstruction(const CoprocessorRegisterTransfersInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, cpOpc, l, crn, rd, cpn, cp, crm] = instruction;
    return ::EncodeCoprocessorRegisterTransfersInstruction(conditionCode, cpOpc, l, crn,rd, cpn, cp, crm);
}