#include "Arm/Disassembler.h"

#include <sstream>
#include <array>

#include <fmt/core.h>

#include "Common/BitUtil.h"
#include "Common/Constants.h"
#include "Common/Decode.h"
#include "Common/Shifts.h"
#include "Common/SignExtend.h"

namespace {

// Symbol Constants

constexpr std::array CONDITION_CODES = {
    "EQ", "NE", "CS", "CC",
    "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT",
    "GT", "LE", "" /*AL*/, "NV"
};

constexpr std::array SHIFT_TYPES = {
    "LSL", "LSR", "ASR", "ROR"
};

constexpr std::array BLOCK_ADDRESS_MODES = {
    "DA", "IA", "DB", "IB"
};

// Symbol helpers

constexpr auto GetConditionSymbol(uint32_t c) {
    return CONDITION_CODES[c];
}

constexpr auto GetLoadStoreSymbol(bool l) {
    return l ? "LD" : "ST";
}

constexpr auto GetWriteBackSymbol(bool w) {
    return w ? "!" : "";
}

constexpr auto GetUpDownSymbol(bool u) {
    return u ? "" : "-";
}

constexpr auto GetLoadPsrOrForceUserSymbol(bool s) {
    return s ? "^" : "";
}

constexpr auto GetPsrSymbol(uint32_t r) {
    return (r == 15u) ? "P" : "";
}

// Data processing instructions (including SWP, MUL, MLA)

auto DisassembleRegisterShift(uint32_t instruction) {
    const auto rs = InstructionRegisterRs(instruction);
    const auto shiftType = InstructionShiftType(instruction);
    const auto rm = InstructionRegisterRm(instruction);
    return fmt::format("R{}, {} R{}", rm, SHIFT_TYPES[shiftType], rs);
}

auto DisassembleImmediateShift(uint32_t instruction) -> std::string {
    const auto shiftAmount = InstructionShiftAmount(instruction);
    const auto shiftType = InstructionShiftType(instruction);
    const auto rm = InstructionRegisterRm(instruction);
    const auto immediateShift = shiftAmount ? shiftAmount : 32u;
    switch (shiftType) {
        case SHIFT_TYPE_LSL: {
            if (immediateShift == 32u) {
                return fmt::format("R{}", rm);
            }
            return fmt::format("R{}, LSL #{}", rm, immediateShift);
        }
        case SHIFT_TYPE_LSR: {
            return fmt::format("R{}, LSR #{}", rm, immediateShift);
        }
        case SHIFT_TYPE_ASR: {
            return fmt::format("R{}, ASR #{}", rm, immediateShift);
        }
        case SHIFT_TYPE_ROR: {
            if (immediateShift == 32u) {
                return fmt::format("R{}, RRX", rm);
            }
            return fmt::format("R{}, ROR #{}", rm, immediateShift);
        }
        default:
            return {};
    }
}

auto DisassembleRegisterOperand2(uint32_t instruction) {
    if (InstructionDataProcessingRegisterShiftBit(instruction)) {
        return DisassembleRegisterShift(instruction);
    } else {
        return DisassembleImmediateShift(instruction);
    }
}

auto DisassembleImmediateOperand2(uint32_t instruction) {
    const auto rotateAmount = InstructionRotateAmount(instruction);
    const auto immediate = InstructionImmediate(instruction);
    const auto op2 = RotateRight(immediate, rotateAmount << 1u);
    return fmt::format("#{}", op2);
}

auto DisassembleAnd(uint32_t instruction, const std::string& mnemonic, const std::string& op2) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto s = InstructionDataProcessingPsrBit(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "{}{}{} R{}, R{}, {}",
        mnemonic, conditionSymbol, s ? "S" : "",
        rd, rn, op2);
}

auto DisassembleTst(uint32_t instruction, const std::string& mnemonic, const std::string& op2) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    const auto psrSymbol = GetPsrSymbol(rd);
    return fmt::format(
        "{}{}{} R{}, {}",
        mnemonic, conditionSymbol, psrSymbol,
        rn, op2);
}

auto DisassembleMov(uint32_t instruction, const std::string& mnemonic, const std::string& op2) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto s = InstructionDataProcessingPsrBit(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "{}{}{} R{}, {}",
        mnemonic, conditionSymbol, s ? "S" : "",
        rd, op2);
}

auto DisassembleSwp(uint32_t instruction, const std::string& byte) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto rm = InstructionRegisterRm(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "SWP{}{} R{}, R{}, [R{}]",
        conditionSymbol, byte,
        rd, rm, rn);
}

auto DisassembleMul(uint32_t instruction) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto s = InstructionDataProcessingPsrBit(instruction);
    const auto rd = InstructionRegisterRn(instruction);
    const auto rs = InstructionRegisterRs(instruction);
    const auto rm = InstructionRegisterRm(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "MUL{}{} R{}, R{}, R{}",
        conditionSymbol, s ? "S" : "",
        rd, rm, rs);
}

auto DisassembleMla(uint32_t instruction) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto s = InstructionDataProcessingPsrBit(instruction);
    const auto rd = InstructionRegisterRn(instruction);
    const auto rn = InstructionRegisterRd(instruction);
    const auto rs = InstructionRegisterRs(instruction);
    const auto rm = InstructionRegisterRm(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "MLA{}{} R{}, R{}, R{}, R{}",
        conditionSymbol, s ? "S" : "",
        rd, rm, rs, rn);
}

bool RegisterDataProcessingRegisterShiftInvalid(uint32_t instruction) {
    return InstructionDataProcessingRegisterShiftBit(instruction) &&
        InstructionDataProcessingRegisterShiftInvalidBit(instruction);
}

auto DisassembleDataProcessingRegister(uint32_t instruction) -> std::string {
    const auto op2 = DisassembleRegisterOperand2(instruction);
    if (!op2.empty()) {
        switch (InstructionDataProcessingOpcode(instruction)) {
            case DATA_OPCODE_AND: {
                if (RegisterDataProcessingRegisterShiftInvalid(instruction)) {
                    return DisassembleMul(instruction);
                } else {
                    return DisassembleAnd(instruction, "AND", op2);
                }
            }
            case DATA_OPCODE_EOR: {
                if (RegisterDataProcessingRegisterShiftInvalid(instruction)) {
                    return DisassembleMla(instruction);
                } else {
                    return DisassembleAnd(instruction, "EOR", op2);
                }
            }
            case DATA_OPCODE_SUB: return DisassembleAnd(instruction, "SUB", op2);
            case DATA_OPCODE_RSB: return DisassembleAnd(instruction, "RSB", op2);
            case DATA_OPCODE_ADD: return DisassembleAnd(instruction, "ADD", op2);
            case DATA_OPCODE_ADC: return DisassembleAnd(instruction, "ADC", op2);
            case DATA_OPCODE_SBC: return DisassembleAnd(instruction, "SBC", op2);
            case DATA_OPCODE_RSC: return DisassembleAnd(instruction, "RSC", op2);
            case DATA_OPCODE_TST: {
                if (RegisterDataProcessingRegisterShiftInvalid(instruction)) {
                    return DisassembleSwp(instruction, "");
                } else {
                    return DisassembleTst(instruction, "TST", op2);
                }
            }
            case DATA_OPCODE_TEQ: return DisassembleTst(instruction, "TEQ", op2);
            case DATA_OPCODE_CMP: {
                if (RegisterDataProcessingRegisterShiftInvalid(instruction)) {
                    return DisassembleSwp(instruction, "B");
                } else {
                    return DisassembleTst(instruction, "CMP", op2);
                }
            }
            case DATA_OPCODE_CMN: return DisassembleTst(instruction, "CMN", op2);
            case DATA_OPCODE_ORR: return DisassembleAnd(instruction, "ORR", op2);
            case DATA_OPCODE_MOV: return DisassembleMov(instruction, "MOV", op2);
            case DATA_OPCODE_BIC: return DisassembleAnd(instruction, "BIC", op2);
            case DATA_OPCODE_MVN: return DisassembleMov(instruction, "MVN", op2);
            default:
                return {};
        }
    }
    return {};
}

auto DisassembleDataProcessingImmediate(uint32_t instruction) -> std::string {
    const auto op2 = DisassembleImmediateOperand2(instruction);
    if (!op2.empty()) {
        switch (InstructionDataProcessingOpcode(instruction)) {
            case DATA_OPCODE_AND: return DisassembleAnd(instruction, "AND", op2);
            case DATA_OPCODE_EOR: return DisassembleAnd(instruction, "EOR", op2);
            case DATA_OPCODE_SUB: return DisassembleAnd(instruction, "SUB", op2);
            case DATA_OPCODE_RSB: return DisassembleAnd(instruction, "RSB", op2);
            case DATA_OPCODE_ADD: return DisassembleAnd(instruction, "ADD", op2);
            case DATA_OPCODE_ADC: return DisassembleAnd(instruction, "ADC", op2);
            case DATA_OPCODE_SBC: return DisassembleAnd(instruction, "SBC", op2);
            case DATA_OPCODE_RSC: return DisassembleAnd(instruction, "RSC", op2);
            case DATA_OPCODE_TST: return DisassembleTst(instruction, "TST", op2);
            case DATA_OPCODE_TEQ: return DisassembleTst(instruction, "TEQ", op2);
            case DATA_OPCODE_CMP: return DisassembleTst(instruction, "CMP", op2);
            case DATA_OPCODE_CMN: return DisassembleTst(instruction, "CMN", op2);
            case DATA_OPCODE_ORR: return DisassembleAnd(instruction, "ORR", op2);
            case DATA_OPCODE_MOV: return DisassembleMov(instruction, "MOV", op2);
            case DATA_OPCODE_BIC: return DisassembleAnd(instruction, "BIC", op2);
            case DATA_OPCODE_MVN: return DisassembleMov(instruction, "MVN", op2);
            default:
                return {};
        }
    }
    return {};
}

// Single data transfer

std::string DisassembleSingleDataTransfer(uint32_t instruction, const std::string& address) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto p = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto b = InstructionSingleDataTransferByteBit(instruction);
    const auto w = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto l = InstructionSingleDataTransferLoadBit(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto loadStoreSymbol = GetLoadStoreSymbol(l);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format("{}R{}{}{} R{}, {}",
        loadStoreSymbol,
        conditionSymbol,
        b ? "B": "",
        (!p & w) ? "T" : "",
        rd,
        address);
}

auto DisassembleImmediateSingleDataTransferAddress(uint32_t instruction) {
    const auto p = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto u = InstructionSingleDataTransferUpBit(instruction);
    const auto w = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto offset = InstructionSingleDataTransferOffset(instruction);
    const auto upDown = GetUpDownSymbol(u);
    const auto writeBack = GetWriteBackSymbol(w);
    if (p) {
        if (offset) {
            return fmt::format("[R{}, #{}{}]{}", rn, upDown, offset, writeBack);
        }
        return fmt::format("[R{}]{}", rn, writeBack);
    } else {
        if (offset) {
            return fmt::format("[R{}], #{}{}", rn, upDown, offset);
        }
        return fmt::format("[R{}]", rn);
    }
}

auto DisassembleRegisterSingleDataTransferAddress(uint32_t instruction) -> std::string {
    // When we get here, bit 4 should be zero
    const auto p = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto u = InstructionSingleDataTransferUpBit(instruction);
    const auto w = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto upDown = GetUpDownSymbol(u);
    const auto writeBack = GetWriteBackSymbol(w);
    const auto shift = DisassembleImmediateShift(instruction);
    if (!shift.empty()) {
        if (p) {
            return fmt::format("[R{}, {}{}]{}", rn, upDown, shift, writeBack);
        } else {
            return fmt::format("[R{}], {}{}", rn, upDown, shift);
        }
    }
    return {};
}

auto DisassembleSingleDataTransferImmediate(uint32_t instruction) -> std::string {
    const auto address = DisassembleImmediateSingleDataTransferAddress(instruction);
    if (!address.empty()) {
        return DisassembleSingleDataTransfer(instruction, address);
    }
    return {};
}

auto DisassembleSingleDataTransferRegister(uint32_t instruction) -> std::string {
    if (InstructionSingleDataTransferRegisterInvalidBit(instruction)) {
        return {};
    }
    const auto address = DisassembleRegisterSingleDataTransferAddress(instruction);
    if (!address.empty()) {
        return DisassembleSingleDataTransfer(instruction, address);
    }
    return {};
}

// Block data transfer

auto DisassembleRegisterRange(uint32_t start, uint32_t end) {
    if (start == end) {
        return fmt::format("R{}", start);
    } else {
        return fmt::format("R{}-R{}", start, end);
    }
}

auto DisassembleRegisterList(uint32_t instruction) -> std::string {
    const auto list = InstructionBlockDataTransferRegisterList(instruction);
    std::stringstream result;
    for (auto i = 1u, start = 0u, separator = 0u; i < 17u; i++) {
        const auto previous = BitSet(list, i - 1u);
        const auto current = BitSet(list, i);
        const auto separatorChars = separator ? ", " : "";
        if (previous ^ current) {
            if (current) {
                start = i;
            } else {
                const auto range = DisassembleRegisterRange(start, i - 1u);
                if (range.empty()) {
                    return {};
                }
                result << separatorChars << range;
                separator = 1u;
            }
        }
    }
    return result.str();
}

auto DisassembleBlockDataTransferInner(uint32_t instruction, const std::string& list) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto p = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto u = InstructionSingleDataTransferUpBit(instruction);
    const auto s = InstructionBlockDataTransferPsrBit(instruction);
    const auto w = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto l = InstructionSingleDataTransferLoadBit(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto loadStoreSymbol = GetLoadStoreSymbol(l);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    const auto writeBack = GetWriteBackSymbol(w);
    const auto loadPsrOrForceUser = GetLoadPsrOrForceUserSymbol(s);
    return fmt::format("{}M{}{} R{}{}, {{{}}}{}",
            loadStoreSymbol,
            conditionSymbol,
            BLOCK_ADDRESS_MODES[((p << 1u) + u)],
            rn,
            writeBack,
            list,
            loadPsrOrForceUser
    );
}

auto DisassembleBlockDataTransfer(uint32_t instruction) -> std::string {
    const auto list = DisassembleRegisterList(instruction);
    if (!list.empty()) {
        return DisassembleBlockDataTransferInner(instruction, list);
    }
    return {};
}

// Branch

auto ExtendedBranchOffset(uint32_t offset) {
    return (SignExtend24(offset) << 2u) + 8u;
}

auto AbsoluteValue(uint32_t v) {
    const auto mask = ((v ^ 0x80000000u) >> 31u) - 0x1u;
    return (v ^ mask) - mask;
}

auto DisassembleBranch(uint32_t instruction) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto link = InstructionBranchLinkBit(instruction);
    const auto offset = InstructionBranchOffset(instruction);
    const auto extended = ExtendedBranchOffset(offset);
    const auto absolute = AbsoluteValue(extended);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    const auto signSymbol = (extended ^ absolute) ? "-" : "";
    return fmt::format("B{}{} #{}{}",
        link ? "L" : "",
        conditionSymbol,
        signSymbol,
        absolute
    );
}

// Coprocessor data transfer

auto DisassembleCoprocessorDataTransferAddress(uint32_t instruction) {
    const auto p = InstructionCoprocessorDataTransferPreIndexBit(instruction);
    const auto u = InstructionCoprocessorDataTransferUpBit(instruction);
    const auto w = InstructionCoprocessorDataTransferWriteBackBit(instruction);
    const auto rn = InstructionRegisterRn(instruction);
    const auto offset = InstructionCoprocessorDataTransferOffset(instruction) << 2u;
    const auto upDown = GetUpDownSymbol(u);
    const auto writeBack = GetWriteBackSymbol(w);
    if (p) {
        if (offset) {
            return fmt::format("[R{}, #{}{}]{}", rn, upDown, offset, writeBack);
        }
        return fmt::format("[R{}]{}", rn, writeBack);
    } else {
        return fmt::format("[R{}], #{}{}", rn, upDown, offset);
    }
}

auto DisassembleCoprocessorDataTransferInner(uint32_t instruction, const std::string& address) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto t = InstructionCoprocessorDataTransferLengthBit(instruction);
    const auto l =  InstructionCoprocessorDataTransferLoadBit(instruction);
    const auto coprocessorNumber = InstructionCoprocessorNumber(instruction);
    const auto coprocessorRd = InstructionCoprocessorRegisterCd(instruction);
    const auto loadStoreSymbol = GetLoadStoreSymbol(l);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format("{}C{}{} p{}, c{}, {}",
            loadStoreSymbol,
            conditionSymbol,
            t ? "L" : "",
            coprocessorNumber,
            coprocessorRd,
            address);
}

auto DisassembleCoprocessorDataTransfer(uint32_t instruction) -> std::string {
    const auto address = DisassembleCoprocessorDataTransferAddress(instruction);
    if (!address.empty()) {
        return DisassembleCoprocessorDataTransferInner(instruction, address);
    }
    return {};
}

// Coprocessor Data Operation, Register Transfer and Software Interrupt

auto DisassembleCoprocessorInfo(uint32_t coprocessorInfo) {
    return fmt::format(", #{}", coprocessorInfo);
}

auto DisassembleCoprocessorDataOperationInner(uint32_t instruction, const std::string& info) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto coprocessorNumber = InstructionCoprocessorNumber(instruction);
    const auto coprocessorOpc = InstructionCoprocessorDataOperationOpcode(instruction);
    const auto cd = InstructionCoprocessorRegisterCd(instruction);
    const auto cn = InstructionCoprocessorRegisterCn(instruction);
    const auto cm = InstructionCoprocessorRegisterCm(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "CDP{} p{}, #{}, c{}, c{}, c{}{}",
        conditionSymbol, coprocessorNumber, coprocessorOpc, cd, cn, cm, info
    );
}

auto DisassembleCoprocessorDataOperationInner(uint32_t instruction) -> std::string {
    const auto coprocessorInfo = InstructionCoprocessorInfo(instruction);
    if (coprocessorInfo) {
        const auto info = DisassembleCoprocessorInfo(coprocessorInfo);
        if (!info.empty()) {
            return DisassembleCoprocessorDataOperationInner(instruction, info);
        }
    } else {
        return DisassembleCoprocessorDataOperationInner(instruction, "");
    }
    return {};
}

auto DisassembleCoprocessorRegisterTransferInner(uint32_t instruction, const std::string& info) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto coprocessorNumber = InstructionCoprocessorNumber(instruction);
    const auto coprocessorOpc = InstructionCoprocessorRegisterTransferOpcode(instruction);
    const auto l = InstructionCoprocessorRegisterTransferLoadBit(instruction);
    const auto rd = InstructionRegisterRd(instruction);
    const auto cn = InstructionCoprocessorRegisterCn(instruction);
    const auto cm = InstructionCoprocessorRegisterCm(instruction);
    const auto mnemonic = l ? "MRC" : "MCR";
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format(
        "{}{} p{}, #{}, R{}, c{}, c{}{}",
        mnemonic, conditionSymbol, coprocessorNumber, coprocessorOpc, rd, cn, cm, info
    );
}

auto DisassembleCoprocessorRegisterTransfer(uint32_t instruction) -> std::string {
    const auto coprocessorInfo = InstructionCoprocessorInfo(instruction);
    if (coprocessorInfo) {
        const auto info = DisassembleCoprocessorInfo(coprocessorInfo);
        if (!info.empty()) {
            return DisassembleCoprocessorRegisterTransferInner(instruction, info);
        }
    } else {
        return DisassembleCoprocessorRegisterTransferInner(instruction, "");
    }
    return {};
}

auto DisassembleSoftwareInterrupt(uint32_t instruction) {
    const auto conditionCode = InstructionConditionCode(instruction);
    const auto comment = InstructionSoftwareInterruptComment(instruction);
    const auto conditionSymbol = GetConditionSymbol(conditionCode);
    return fmt::format("SWI{} {}", conditionSymbol, comment);
}

std::string DisassembleCoprocessorDataOperation(uint32_t instruction) {
    if (InstructionCoprocessorSwiBit(instruction)) {
        return DisassembleSoftwareInterrupt(instruction);
    } else if (InstructionCoprocessorRegisterTransferBit(instruction)) {
        return DisassembleCoprocessorRegisterTransfer(instruction);
    } else {
        return DisassembleCoprocessorDataOperationInner(instruction);
    }
}

// Disassemble

auto Disassemble(uint32_t instruction) -> std::string {
    switch (InstructionType(instruction)) {
        case 0x0u: return DisassembleDataProcessingRegister(instruction);
        case 0x1u: return DisassembleDataProcessingImmediate(instruction);
        case 0x2u: return DisassembleSingleDataTransferImmediate(instruction);
        case 0x3u: return DisassembleSingleDataTransferRegister(instruction);
        case 0x4u: return DisassembleBlockDataTransfer(instruction);
        case 0x5u: return DisassembleBranch(instruction);
        case 0x6u: return DisassembleCoprocessorDataTransfer(instruction);
        case 0x7u: return DisassembleCoprocessorDataOperation(instruction);
        default:return {};
    }
}

}

// Public interface
namespace rbrown::arm {
std::string Disassemble(uint32_t instruction) {
    return ::Disassemble(instruction);
}
}

