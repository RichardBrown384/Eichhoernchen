#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::CoprocessorDataOperationsInstruction {
    uint32_t conditionCode;
    uint32_t cpOpc;
    uint32_t crn;
    uint32_t crd;
    uint32_t cpn;
    uint32_t cp;
    uint32_t crm;
};

auto Assembler::AssembleCdp(SourceLine& source, uint32_t& result) -> bool {
    return AssembleCoprocessorDataOperationsInstruction(source, result);
}

auto Assembler::AssembleCoprocessorDataOperationsInstruction(SourceLine& source, uint32_t& result) -> bool {
    CoprocessorDataOperationsInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
    };
    if (AssembleCoprocessorDataOperationsInstruction(source, instruction)) {
        result = EncodeCoprocessorDataOperationsInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataOperationsInstruction(SourceLine& source, CoprocessorDataOperationsInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleCoprocessorDataOperationsInstructionArguments(source, instruction);
    } else if (AssembleConditionCode(source, instruction.conditionCode)) {
        return AssembleCoprocessorDataOperationsInstructionArguments(source, instruction);
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataOperationsInstructionArguments(SourceLine& source, CoprocessorDataOperationsInstruction& instruction) -> bool {
    if (AssembleCoprocessorCoprocessorArgument(source, instruction.cpn)) {
        if (AssembleCoprocessorOpcodeArgument(source, instruction.cpOpc)) {
            if (AssembleCoprocessorDataOperationsInstructionRegisterNumbers(source, instruction)) {
                return AssembleCoprocessorInformationArgument(source, instruction.cp);
            }
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorDataOperationsInstructionRegisterNumbers(SourceLine& source, CoprocessorDataOperationsInstruction& instruction) -> bool {
    if (AssembleCoprocessorCoprocessorRegisterArgument(source, instruction.crd)) {
        if (AssembleCoprocessorCoprocessorRegisterArgument(source, instruction.crn)) {
            return AssembleCoprocessorCoprocessorRegisterArgument(source, instruction.crm);
        }
    }
    return false;
}

auto Assembler::EncodeCoprocessorDataOperationsInstruction(const CoprocessorDataOperationsInstruction& instruction) -> uint32_t {
    const auto& [conditionCode, cpOpc, crn, crd, cpn, cp, crm] = instruction;
    return ::EncodeCoprocessorDataOperationsInstruction(conditionCode, cpOpc, crn, crd, cpn, cp, crm);
}

