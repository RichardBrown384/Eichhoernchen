#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Encode.h"

using namespace rbrown::arm;

struct Assembler::DataProcessingInstruction {
    uint32_t conditionCode;
    uint32_t immediate;
    uint32_t opcode;
    uint32_t s;
    uint32_t rn;
    uint32_t rd;
    uint32_t op2;
};

auto Assembler::AssembleAdc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_ADC, result);
}

auto Assembler::AssembleAdd(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_ADD, result);
}

auto Assembler::AssembleAnd(SourceLine& source, uint32_t& result) -> bool{
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_AND, result);
}

auto Assembler::AssembleBic(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_BIC, result);
}

auto Assembler::AssembleCmp(SourceLine& source, uint32_t& result) -> bool {
    return AssembleCompareDataProcessingInstruction(source, DATA_OPCODE_CMP, result);
}

auto Assembler::AssembleCmn(SourceLine& source, uint32_t& result) -> bool {
    return AssembleCompareDataProcessingInstruction(source, DATA_OPCODE_CMN, result);
}

auto Assembler::AssembleEor(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_EOR, result);
}

auto Assembler::AssembleMov(SourceLine& source, uint32_t& result) -> bool {
    return AssembleMoveDataProcessingInstruction(source, DATA_OPCODE_MOV, result);
}

auto Assembler::AssembleMvn(SourceLine& source, uint32_t& result) -> bool {
    return AssembleMoveDataProcessingInstruction(source, DATA_OPCODE_MVN, result);
}

auto Assembler::AssembleOrr(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_ORR, result);
}

auto Assembler::AssembleRsb(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_RSB, result);
}

auto Assembler::AssembleRsc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_RSC, result);
}

auto Assembler::AssembleSbc(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_SBC, result);
}

auto Assembler::AssembleSub(SourceLine& source, uint32_t& result) -> bool {
    return AssembleDataProcessingInstruction(source, DATA_OPCODE_SUB, result);
}

auto Assembler::AssembleTeq(SourceLine& source, uint32_t& result) -> bool {
    return AssembleCompareDataProcessingInstruction(source, DATA_OPCODE_TEQ, result);
}

auto Assembler::AssembleTst(SourceLine& source, uint32_t& result) -> bool {
    return AssembleCompareDataProcessingInstruction(source, DATA_OPCODE_TST, result);
}

auto Assembler::AssembleDataProcessingInstruction(SourceLine& source, uint32_t opcode, uint32_t& result) -> bool{
    DataProcessingInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .opcode = opcode
    };
    if (AssembleDataProcessingInstruction(source, instruction)) {
        result = EncodeDataProcessingInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleDataProcessingInstruction(SourceLine& source, DataProcessingInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleDataProcessingInstructionArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('S')) {
            instruction.s = 1u;
            return AssembleDataProcessingInstructionArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('S')) {
                instruction.s = 1u;
            }
            return AssembleDataProcessingInstructionArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleDataProcessingInstructionArguments(
    SourceLine& source,
    DataProcessingInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        if (AssembleWhitespaceCommaRegisterNumber(source, instruction.rn)) {
            return AssembleDataProcessingInstructionOperand2(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleCompareDataProcessingInstruction(SourceLine& source, uint32_t opcode, uint32_t& result) -> bool {
    DataProcessingInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .opcode = opcode,
        .s = 1u
    };
    if (AssembleCompareDataProcessingInstruction(source, instruction)) {
        result = EncodeDataProcessingInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleCompareDataProcessingInstruction(SourceLine& source, DataProcessingInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleCompareDataProcessingInstructionArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('P')) {
            if (source.MatchAndAdvance('L')) {
                instruction.conditionCode = CONDITION_CODE_PL;
                if (source.MatchAndAdvance('P')) {
                    instruction.rd = 15u;
                }
            } else {
                instruction.rd = 15u;
            }
            return AssembleCompareDataProcessingInstructionArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('P')) {
                instruction.rd = 15u;
            }
            return AssembleCompareDataProcessingInstructionArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleCompareDataProcessingInstructionArguments(
    SourceLine& source,
    DataProcessingInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rn)) {
        return AssembleDataProcessingInstructionOperand2(source, instruction);
    }
    return false;
}

auto Assembler::AssembleMoveDataProcessingInstruction(SourceLine& source, uint32_t opcode, uint32_t& result) -> bool {
    DataProcessingInstruction instruction = {
        .conditionCode = CONDITION_CODE_AL,
        .opcode = opcode
    };
    if (AssembleMoveDataProcessingInstruction(source, instruction)) {
        result = EncodeDataProcessingInstruction(instruction);
        return true;
    }
    return false;
}

auto Assembler::AssembleMoveDataProcessingInstruction(SourceLine& source, DataProcessingInstruction& instruction) -> bool {
    if (source.MatchWhitespace()) {
        return AssembleMoveDataProcessingInstructionArguments(source, instruction);
    } else {
        if (source.MatchAndAdvance('S')) {
            instruction.s = 1u;
            return AssembleMoveDataProcessingInstructionArguments(source, instruction);
        } else if (AssembleConditionCode(source, instruction.conditionCode)) {
            if (source.MatchAndAdvance('S')) {
                instruction.s = 1u;
            }
            return AssembleMoveDataProcessingInstructionArguments(source, instruction);
        }
    }
    return false;
}

auto Assembler::AssembleMoveDataProcessingInstructionArguments(
    SourceLine& source,
    DataProcessingInstruction& instruction) -> bool {
    if (AssembleWhitespaceRegisterNumber(source, instruction.rd)) {
        return AssembleDataProcessingInstructionOperand2(source, instruction);
    }
    return false;
}

auto Assembler::AssembleDataProcessingInstructionOperand2(
    SourceLine& source,
    DataProcessingInstruction& instruction) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        if (source.Match('R')) {
            if (AssembleShiftTypeOperand2(source, instruction.op2)) {
                instruction.immediate = 0u;
                return true;
            }
        } else if (AssembleImmediateTypeOperand2(source, instruction.op2)) {
            instruction.immediate = 1u;
            return true;
        }
    }
    return false;
}

auto Assembler::EncodeDataProcessingInstruction(const DataProcessingInstruction& instruction) -> uint32_t {
    const auto&[conditionCode, immediate, opcode, s, rn, rd, op2] = instruction;
    return ::EncodeDataProcessingInstruction(
        conditionCode, immediate,
        opcode, s, rn, rd,
        op2);
}
