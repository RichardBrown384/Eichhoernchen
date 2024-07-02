#pragma once

constexpr auto PSR_MODE_USR = 0u;
constexpr auto PSR_MODE_FIQ = 1u;
constexpr auto PSR_MODE_IRQ = 2u;
constexpr auto PSR_MODE_SVC = 3u;

constexpr auto SHIFT_TYPE_LSL = 0u;
constexpr auto SHIFT_TYPE_LSR = 1u;
constexpr auto SHIFT_TYPE_ASR = 2u;
constexpr auto SHIFT_TYPE_ROR = 3u;

constexpr auto CONDITION_CODE_EQ = 0x0u;
constexpr auto CONDITION_CODE_NE = 0x1u;
constexpr auto CONDITION_CODE_CS = 0x2u;
constexpr auto CONDITION_CODE_CC = 0x3u;
constexpr auto CONDITION_CODE_MI = 0x4u;
constexpr auto CONDITION_CODE_PL = 0x5u;
constexpr auto CONDITION_CODE_VS = 0x6u;
constexpr auto CONDITION_CODE_VC = 0x7u;
constexpr auto CONDITION_CODE_HI = 0x8u;
constexpr auto CONDITION_CODE_LS = 0x9u;
constexpr auto CONDITION_CODE_GE = 0xAu;
constexpr auto CONDITION_CODE_LT = 0xBu;
constexpr auto CONDITION_CODE_GT = 0xCu;
constexpr auto CONDITION_CODE_LE = 0xDu;
constexpr auto CONDITION_CODE_AL = 0xEu;
constexpr auto CONDITION_CODE_NV = 0xFu;

constexpr auto DATA_OPCODE_AND = 0x0u;
constexpr auto DATA_OPCODE_EOR = 0x1u;
constexpr auto DATA_OPCODE_SUB = 0x2u;
constexpr auto DATA_OPCODE_RSB = 0x3u;
constexpr auto DATA_OPCODE_ADD = 0x4u;
constexpr auto DATA_OPCODE_ADC = 0x5u;
constexpr auto DATA_OPCODE_SBC = 0x6u;
constexpr auto DATA_OPCODE_RSC = 0x7u;
constexpr auto DATA_OPCODE_TST = 0x8u;
constexpr auto DATA_OPCODE_TEQ = 0x9u;
constexpr auto DATA_OPCODE_CMP = 0xAu;
constexpr auto DATA_OPCODE_CMN = 0xBu;
constexpr auto DATA_OPCODE_ORR = 0xCu;
constexpr auto DATA_OPCODE_MOV = 0xDu;
constexpr auto DATA_OPCODE_BIC = 0xEu;
constexpr auto DATA_OPCODE_MVN = 0xFu;

constexpr auto REGISTER_R14 = 14u;
constexpr auto REGISTER_R15 = 15u;