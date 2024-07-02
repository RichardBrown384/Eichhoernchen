#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"
#include "Common/Shifts.h"

using namespace rbrown::arm;

constexpr auto EncodeInstructionSpecifiedShift(uint32_t shiftAmount, uint32_t shiftType) {
    return ((shiftAmount & 0x1Fu) << 3u) + ((shiftType & 0x3u) << 1u);
}

constexpr auto EncodeRegisterSpecifiedShift(uint32_t rs, uint32_t shiftType) {
    return ((rs & 0xFu) << 4u) + ((shiftType & 0x3u) << 1u) + 1u;
}

constexpr auto EncodeShiftTypeOperand2(uint32_t shift, uint32_t rm) {
    return ((shift & 0xFFu) << 4u) + (rm & 0xFu);
}

constexpr auto EncodeImmediateTypeOperand2(uint32_t immediate, uint32_t& op2) {
    for (auto rotate = 0u; rotate < 16u; rotate++) {
        const auto imm = RotateLeft(immediate, rotate << 1u);
        if (imm < 0x100u) {
            op2 = (rotate << 8u) + imm;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleShiftTypeOperand2(SourceLine& source, uint32_t& op2) -> bool {
    uint32_t shift;
    uint32_t rm;
    if (AssembleRegisterNumber(source, rm)) {
        if (source.MatchAndAdvanceCommaWhitespace()) {
            if (AssembleShiftTypeOperand2Shift(source, shift)) {
                op2 = EncodeShiftTypeOperand2(shift, rm);
                return true;
            }
        } else {
            op2 = EncodeShiftTypeOperand2(0u, rm);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleShiftTypeOperand2Shift(SourceLine& source, uint32_t& shift) -> bool {
    if (source.MatchAndAdvance('A')) {
        if (source.MatchAndAdvance('S')) {
            if (source.MatchAndAdvance('R')) {
                return AssembleASR(source, shift);
            }
        }
    } else {
        if (source.MatchAndAdvance('L')) {
            if (source.MatchAndAdvance('S')) {
                if (source.MatchAndAdvance('L')) {
                    return AssembleLSL(source, shift);
                } else if (source.MatchAndAdvance('R')) {
                    return AssembleLSR(source, shift);
                }
            }
        } else if (source.MatchAndAdvance('R')) {
            if (source.MatchAndAdvance('O')) {
                if (source.MatchAndAdvance('R')) {
                    return AssembleROR(source, shift);
                }
            } else if (source.MatchAndAdvance('R')) {
                if (source.MatchAndAdvance('X')) {
                    return AssembleRRX(source, shift);
                }
            }
        }
    }
    return false;
}

auto Assembler::AssembleLSL(SourceLine& source, uint32_t& shift) -> bool {
    uint32_t shiftAmount, rs;
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleShiftAmountNumber(source, shiftAmount)) {
            if (shiftAmount < 32u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_LSL);
                return true;
            }
        } else if (AssembleRegisterNumber(source, rs)) {
            shift = EncodeRegisterSpecifiedShift(rs, SHIFT_TYPE_LSL);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleLSR(SourceLine& source, uint32_t& shift) -> bool {
    uint32_t shiftAmount, rs;
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleShiftAmountNumber(source, shiftAmount)) {
            if (shiftAmount == 0u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_LSL);
                return true;
            } else if (shiftAmount < 33u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_LSR);
                return true;
            }
        } else if (AssembleRegisterNumber(source, rs)) {
            shift = EncodeRegisterSpecifiedShift(rs, SHIFT_TYPE_LSR);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleASR(SourceLine& source, uint32_t& shift) -> bool {
    uint32_t shiftAmount, rs;
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleShiftAmountNumber(source, shiftAmount)) {
            if (shiftAmount == 0u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_LSL);
                return true;
            } else if (shiftAmount < 33u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_ASR);
                return true;
            }
        } else if (AssembleRegisterNumber(source, rs)) {
            shift = EncodeRegisterSpecifiedShift(rs, SHIFT_TYPE_ASR);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleROR(SourceLine& source, uint32_t& shift) -> bool {
    uint32_t shiftAmount, rs;
    if (source.MatchAndAdvanceWhitespace()) {
        if (AssembleShiftAmountNumber(source, shiftAmount)) {
            if (shiftAmount == 0u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_LSL);
                return true;
            } else if (shiftAmount < 32u) {
                shift = EncodeInstructionSpecifiedShift(shiftAmount, SHIFT_TYPE_ROR);
                return true;
            }
        } else if (AssembleRegisterNumber(source, rs)) {
            shift = EncodeRegisterSpecifiedShift(rs, SHIFT_TYPE_ROR);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleRRX(SourceLine& source, uint32_t& shift) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        if (source.Match('R') || source.Match('#')) {
            return false;
        }
    }
    shift = EncodeInstructionSpecifiedShift(0, SHIFT_TYPE_ROR);
    return true;
}

auto Assembler::AssembleImmediateTypeOperand2(SourceLine& source, uint32_t& op2) -> bool {
    uint32_t immediate;
    if (AssembleImmediateNumber(source, immediate)) {
        return EncodeImmediateTypeOperand2(immediate, op2);
    }
    return false;
}