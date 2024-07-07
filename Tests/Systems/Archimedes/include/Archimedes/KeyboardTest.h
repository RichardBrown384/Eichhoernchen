#pragma once

#include "Archimedes/ArchimedesKeyboard.h"
#include "Common/Util/BitUtil.h"

#include <gtest/gtest.h>

constexpr auto KBD_ID_UK = 0x81u;

constexpr auto KBD_CODE_HRST = 0xFFu;
constexpr auto KBD_CODE_RAK1 = 0xFEu;
constexpr auto KBD_CODE_RAK2 = 0xFD;
constexpr auto KBD_CODE_INVALID = 0xFCu;

constexpr auto KBD_CODE_BACK = 0x3Fu;
constexpr auto KBD_CODE_SMAK = 0x33u;
constexpr auto KBD_CODE_MACK = 0x32u;
constexpr auto KBD_CODE_SACK = 0x31u;
constexpr auto KBD_CODE_NACK = 0x30u;

constexpr auto KBD_CODE_RQMP = 0x22u;
constexpr auto KBD_CODE_NOOP = 0x21u;
constexpr auto KBD_CODE_KBID = 0x20u;

constexpr auto MOUSE_BUTTON_LEFT = 0x70u;
constexpr auto MOUSE_BUTTON_MIDDLE = 0x71u;
constexpr auto MOUSE_BUTTON_RIGHT = 0x72u;

constexpr auto KeyDownFirst(uint8_t v) { return 0xC0u + ExtractBitField(v, 4u, 4u); }
constexpr auto KeyDownSecond(uint8_t v) { return 0xC0u + ExtractBitField(v, 0u, 4u); }
constexpr auto KeyUpFirst(uint8_t v) { return 0xD0u + ExtractBitField(v, 4u, 4u); }
constexpr auto KeyUpSecond(uint8_t v) { return 0xD0u + ExtractBitField(v, 0u, 4u); }

class KeyboardTest : public testing::Test {
public:
    KeyboardTest();
    ~KeyboardTest() override = default;
    auto Write(uint8_t) -> void;
    auto Read(uint8_t&) -> bool;
    auto KeyDown(uint8_t) -> void;
    auto KeyUp(uint8_t) -> void;
    auto MouseMotion(uint8_t, uint8_t) -> void;
    auto MouseButtonDown(uint8_t) -> void;
    auto MouseButtonUp(uint8_t) -> void;
    auto WriteReset() -> void;
    auto WriteResetAcknowledge1() -> void;
    auto WriteResetAcknowledge2() -> void;
    auto WriteRequestData(uint8_t) -> void;
    auto WriteByteAcknowledge() -> void;
    auto WriteBothScanAcknowledge() -> void;
    auto WriteMouseScanAcknowledge() -> void;
    auto WriteKeyboardScanAcknowledge() -> void;
    auto WriteNoScanAcknowledge() -> void;
    auto WriteRequestMouseData() -> void;
    auto WriteNoOp() -> void;
    auto WriteRequestKeyboardId() -> void;
    auto WriteLed(uint8_t) -> void;
private:
    rbrown::acorn::archimedes::ArchimedesKeyboard m_keyboard;
};