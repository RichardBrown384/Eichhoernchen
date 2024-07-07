#include "Archimedes/KeyboardTest.h"

#include "Common/Util/BitUtil.h"

KeyboardTest::KeyboardTest() : m_keyboard{} {}

auto KeyboardTest::Write(uint8_t v) -> void { m_keyboard.WriteByte(v); }
auto KeyboardTest::Read(uint8_t& v) -> bool { return m_keyboard.ReadByte(v); }
auto KeyboardTest::KeyDown(uint8_t v) -> void { m_keyboard.KeyDown(v); }
auto KeyboardTest::KeyUp(uint8_t v) -> void { m_keyboard.KeyUp(v); }
auto KeyboardTest::MouseMotion(uint8_t x, uint8_t y) -> void { m_keyboard.MouseMotion(x, y); }
auto KeyboardTest::MouseButtonDown(uint8_t v) -> void { m_keyboard.MouseButtonDown(v); }
auto KeyboardTest::MouseButtonUp(uint8_t v) -> void { m_keyboard.MouseButtonUp(v); }

auto KeyboardTest::WriteReset() -> void { Write(KBD_CODE_HRST); }
auto KeyboardTest::WriteResetAcknowledge1() -> void { Write(KBD_CODE_RAK1); }
auto KeyboardTest::WriteResetAcknowledge2() -> void { Write(KBD_CODE_RAK2); }

auto KeyboardTest::WriteRequestData(uint8_t v) -> void { Write(0x40u + EncodeBitField(v, 0u, 4u)); }

auto KeyboardTest::WriteByteAcknowledge() -> void { Write(KBD_CODE_BACK); }
auto KeyboardTest::WriteBothScanAcknowledge() -> void { Write(KBD_CODE_SMAK); }
auto KeyboardTest::WriteMouseScanAcknowledge() -> void { Write(KBD_CODE_MACK); }
auto KeyboardTest::WriteKeyboardScanAcknowledge() -> void { Write(KBD_CODE_SACK); }
auto KeyboardTest::WriteNoScanAcknowledge() -> void { Write(KBD_CODE_NACK); }

auto KeyboardTest::WriteRequestMouseData() -> void { Write(KBD_CODE_RQMP); }
auto KeyboardTest::WriteNoOp() -> void { Write(KBD_CODE_NOOP); }
auto KeyboardTest::WriteRequestKeyboardId() -> void { Write(KBD_CODE_KBID); }

auto KeyboardTest::WriteLed(uint8_t v) -> void { Write(EncodeBitField(v, 0u, 3u)); }