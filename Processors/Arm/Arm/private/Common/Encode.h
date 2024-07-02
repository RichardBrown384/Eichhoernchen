#pragma once

#include <cstdint>

constexpr auto EncodeBit1(uint32_t v, uint32_t n) {
    return (v & 0x1u) << n;
}

constexpr auto EncodeBit3(uint32_t v, uint32_t n) {
    return (v & 0x7u) << n;
}

constexpr auto EncodeBit4(uint32_t v, uint32_t n) {
    return (v & 0xFu) << n;
}

constexpr auto EncodeBit8(uint32_t v, uint32_t n) {
    return (v & 0xFFu) << n;
}

constexpr auto EncodeBit12(uint32_t v, uint32_t n) {
    return (v & 0xFFFu) << n;
}

constexpr auto EncodeBit16(uint32_t v, uint32_t n) {
    return (v & 0xFFFFu) << n;
}

constexpr auto EncodeBit23(uint32_t v, uint32_t n) {
    return (v & 0x7FFFFFu) << n;
}

constexpr auto EncodeBit24(uint32_t v, uint32_t n) {
    return (v & 0xFFFFFFu) << n;
}

constexpr auto EncodeDataProcessingInstruction(
    uint32_t condition_code, uint32_t immediate, uint32_t opcode, uint32_t s,
    uint32_t rn, uint32_t rd, uint32_t op2) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit1(immediate, 25u) +
           EncodeBit4(opcode, 21u) +
           EncodeBit1(s, 20u) +
           EncodeBit4(rn, 16u) +
           EncodeBit4(rd, 12u) +
           EncodeBit12(op2, 0u);
}

constexpr auto EncodeMultiplyInstruction(
    uint32_t condition_code, uint32_t accumulate, uint32_t s, uint32_t rd,
    uint32_t rn, uint32_t rs, uint32_t rm) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit1(accumulate, 21u) +
           EncodeBit1(s, 20u) +
           EncodeBit4(rd, 16u) +
           EncodeBit4(rn, 12u) +
           EncodeBit4(rs, 8u) +
           EncodeBit4(0x9u, 4u) +
           EncodeBit4(rm, 0u);
}

constexpr auto EncodeSwapInstruction(uint32_t condition_code, uint32_t b, uint32_t rn, uint32_t rd, uint32_t rm) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit1(0x1u, 24u) +
           EncodeBit1(b, 22u) +
           EncodeBit4(rn, 16u) +
           EncodeBit4(rd, 12u) +
           EncodeBit4(0x9u, 4u) +
           EncodeBit4(rm, 0u);
}

constexpr auto EncodeSingleDataTransferInstruction(
    uint32_t condition_code, uint32_t immediate, uint32_t p, uint32_t u, uint32_t b, uint32_t w, uint32_t l,
    uint32_t rn, uint32_t rd, uint32_t offset) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit1(0x1u, 26u) +
           EncodeBit1(immediate, 25u) +
           EncodeBit1(p, 24u) +
           EncodeBit1(u, 23u) +
           EncodeBit1(b, 22u) +
           EncodeBit1(w, 21u) +
           EncodeBit1(l, 20u) +
           EncodeBit4(rn, 16u) +
           EncodeBit4(rd, 12u) +
           EncodeBit12(offset, 0u);
}

constexpr auto EncodeBlockDataTransferInstruction(
    uint32_t condition_code, uint32_t p, uint32_t u, uint32_t s, uint32_t w, uint32_t l,
    uint32_t rn, uint32_t register_list) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x4u, 25u) +
           EncodeBit1(p, 24u) +
           EncodeBit1(u, 23u) +
           EncodeBit1(s, 22u) +
           EncodeBit1(w, 21u) +
           EncodeBit1(l, 20u) +
           EncodeBit4(rn, 16u) +
           EncodeBit16(register_list, 0u);
}

constexpr auto EncodeBranchInstruction(uint32_t condition_code, uint32_t l, uint32_t offset) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x5u, 25u) +
           EncodeBit1(l, 24u) +
           EncodeBit24(offset, 0u);
}

constexpr auto EncodeCoprocessorDataTransferInstruction(
    uint32_t condition_code, uint32_t p, uint32_t u, uint32_t n, uint32_t w, uint32_t l,
    uint32_t rn, uint32_t crd, uint32_t cpn, uint32_t offset) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x6u, 25u) +
           EncodeBit1(p, 24u) +
           EncodeBit1(u, 23u) +
           EncodeBit1(n, 22u) +
           EncodeBit1(w, 21u) +
           EncodeBit1(l, 20u) +
           EncodeBit4(rn, 16u) +
           EncodeBit4(crd, 12u) +
           EncodeBit4(cpn, 8u) +
           EncodeBit8(offset, 0u);
}

constexpr auto EncodeCoprocessorDataOperationsInstruction(
    uint32_t condition_code, uint32_t cp_opc, uint32_t crn, uint32_t crd,
    uint32_t cpn, uint32_t cp, uint32_t crm) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x7u, 25u) +
           EncodeBit4(cp_opc, 20u) +
           EncodeBit4(crn, 16u) +
           EncodeBit4(crd, 12u) +
           EncodeBit4(cpn, 8u) +
           EncodeBit3(cp, 5u) +
           EncodeBit4(crm, 0u);
}

constexpr auto EncodeCoprocessorRegisterTransfersInstruction(
    uint32_t condition_code, uint32_t cp_opc, uint32_t l, uint32_t crn, uint32_t rd,
    uint32_t cpn, uint32_t cp, uint32_t crm) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x7u, 25u) +
           EncodeBit3(cp_opc, 21u) +
           EncodeBit1(l, 20u) +
           EncodeBit4(crn, 16u) +
           EncodeBit4(rd, 12u) +
           EncodeBit4(cpn, 8u) +
           EncodeBit3(cp, 5u) +
           EncodeBit1(0x1u, 4u) +
           EncodeBit4(crm, 0u);
}

constexpr auto EncodeSoftwareInterruptInstruction(
    uint32_t condition_code, uint32_t comment) {
    return EncodeBit4(condition_code, 28u) +
           EncodeBit3(0x7u, 25u) +
           EncodeBit3(0x1u, 24u) +
           EncodeBit23(comment, 0u);
}
