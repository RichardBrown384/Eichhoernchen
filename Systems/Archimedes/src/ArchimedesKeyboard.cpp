#include "Archimedes/ArchimedesKeyboard.h"

#include "Common/Util/BitUtil.h"

constexpr auto KEYBOARD_ID_UK = 0x81u;

constexpr auto HRST = 0xFFu; // Hard reset
constexpr auto RAK1 = 0xFEu; // Reset ack 1
constexpr auto RAK2 = 0xFDu; // Reset ack 2

constexpr auto PDAT = 0xE0u;

constexpr auto KUDA = 0xD0u; // Key up data
constexpr auto KDDA = 0xC0u; // Key down data

constexpr auto RQPDF = 0x4Fu;
constexpr auto RQPDE = 0x4Eu;
constexpr auto RQPDD = 0x4Du;
constexpr auto RQPDC = 0x4Cu;
constexpr auto RQPDB = 0x4Bu;
constexpr auto RQPDA = 0x4Au;
constexpr auto RQPD9 = 0x49u;
constexpr auto RQPD8 = 0x48u;
constexpr auto RQPD7 = 0x47u;
constexpr auto RQPD6 = 0x46u;
constexpr auto RQPD5 = 0x45u;
constexpr auto RQPD4 = 0x44u;
constexpr auto RQPD3 = 0x43u;
constexpr auto RQPD2 = 0x42u;
constexpr auto RQPD1 = 0x41u;
constexpr auto RQPD0 = 0x40u;

constexpr auto BACK = 0x3Fu; // Byte acknowledge in two byte sequence
constexpr auto SMAK = 0x33u; // Enables both keyboard and mouse scanning
constexpr auto MACK = 0x32u; // Enables mouse scanning only
constexpr auto SACK = 0x31u; // Enables key scanning only
constexpr auto NACK = 0x30u; // Disables key scanning (and transmission) as well as mouse position

constexpr auto RQMP = 0x22u; // Request mouse
constexpr auto PRST = 0x21u; // No op
constexpr auto RQID = 0x20u; // Request keyboard id

constexpr auto LED7 = 0x07u;
constexpr auto LED6 = 0x06u;
constexpr auto LED5 = 0x05u;
constexpr auto LED4 = 0x04u;
constexpr auto LED3 = 0x03u;
constexpr auto LED2 = 0x02u;
constexpr auto LED1 = 0x01u;
constexpr auto LED0 = 0x00u;

constexpr auto EncodeDataRequestResponse(uint8_t v) -> uint8_t {
    return PDAT + ExtractBitField(v, 0u, 4u);
}

constexpr auto EncodeKeyDown(uint8_t v) -> std::pair<uint8_t, uint8_t> {
    return {
        KDDA + ExtractBitField(v, 4u, 4u),
        KDDA + ExtractBitField(v, 0u, 4u)
    };
}

constexpr auto EncodeKeyUp(uint8_t v) -> std::pair<uint8_t, uint8_t> {
    return {
        KUDA + ExtractBitField(v, 4u, 4u),
        KUDA + ExtractBitField(v, 0u, 4u)
    };
}

namespace rbrown::acorn::archimedes {

ArchimedesKeyboard::ArchimedesKeyboard() :
    communicationState{ CommunicationState::AwaitingRestart },
    scanMode{ ScanMode::Restart },
    keyboardState{},
    queue{} {}

auto ArchimedesKeyboard::KeyDown(uint8_t key) -> void {
    PushKeyDown(key);
}

auto ArchimedesKeyboard::KeyUp(uint8_t key) -> void {
    PushKeyUp(key);
}

auto ArchimedesKeyboard::MouseMotion(uint8_t x, uint8_t y) -> void {
    if (MouseScanActive())  {
        PushMouseMotion(x, y);
    }
}

auto ArchimedesKeyboard::MouseButtonDown(uint8_t button) -> void {
    // A3000 service manual says mouse buttons behave the same way as keyboard keys wrt scan mode
    PushKeyDown(button);
}

auto ArchimedesKeyboard::MouseButtonUp(uint8_t button) -> void {
    PushKeyUp(button);
}

auto ArchimedesKeyboard::ReadByte(uint8_t& v) -> bool {
    if (Empty() || GetCommunicationState() != CommunicationState::Transmitting) {
        return false;
    }
    const auto& [type, nextByte, nextState] = Pop();
    SetCommunicationState(nextState);
    v = nextByte;
    return true;
}

auto ArchimedesKeyboard::WriteByte(uint8_t v) -> void {
    switch (v) {
        case HRST: return WriteHardwareRestart();
        case RAK1: return WriteHardwareRestartAcknowledge1();
        case RAK2: return WriteHardwareRestartAcknowledge2();

        case RQPDF: return WriteRequestData(0xFu);
        case RQPDE: return WriteRequestData(0xEu);
        case RQPDD: return WriteRequestData(0xDu);
        case RQPDC: return WriteRequestData(0xCu);
        case RQPDB: return WriteRequestData(0xBu);
        case RQPDA: return WriteRequestData(0xAu);
        case RQPD9: return WriteRequestData(0x9u);
        case RQPD8: return WriteRequestData(0x8u);
        case RQPD7: return WriteRequestData(0x7u);
        case RQPD6: return WriteRequestData(0x6u);
        case RQPD5: return WriteRequestData(0x5u);
        case RQPD4: return WriteRequestData(0x4u);
        case RQPD3: return WriteRequestData(0x3u);
        case RQPD2: return WriteRequestData(0x2u);
        case RQPD1: return WriteRequestData(0x1u);
        case RQPD0: return WriteRequestData(0x0u);

        case BACK: return WriteByteAcknowledge();
        case SMAK: return WriteAcknowledge(ScanMode::Both);
        case MACK: return WriteAcknowledge(ScanMode::Mouse);
        case SACK: return WriteAcknowledge(ScanMode::Keyboard);
        case NACK: return WriteAcknowledge(ScanMode::None);

        case RQMP: return WriteRequestMousePointer();
        case PRST: return WriteRequestNoOp();
        case RQID: return WriteRequestKeyboardId();

        case LED7: return WriteLed(7u);
        case LED6: return WriteLed(6u);
        case LED5: return WriteLed(5u);
        case LED4: return WriteLed(4u);
        case LED3: return WriteLed(3u);
        case LED2: return WriteLed(2u);
        case LED1: return WriteLed(1u);
        case LED0: return WriteLed(0u);

        default: return WriteError();
    }
}

auto ArchimedesKeyboard::WriteHardwareRestart() -> void {
    if (GetCommunicationState() == CommunicationState::AwaitingRestart) {
        SetCommunicationState(CommunicationState::Transmitting);
        PushProtocol(HRST, CommunicationState::AwaitingRestartAcknowledge1);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteHardwareRestartAcknowledge1() -> void {
    if (GetCommunicationState() == CommunicationState::AwaitingRestartAcknowledge1) {
        SetCommunicationState(CommunicationState::Transmitting);
        PushProtocol(RAK1, CommunicationState::AwaitingRestartAcknowledge2);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteHardwareRestartAcknowledge2() -> void {
    if (GetCommunicationState() == CommunicationState::AwaitingRestartAcknowledge2) {
        SetCommunicationState(CommunicationState::Transmitting);
        PushProtocol(RAK2, CommunicationState::AwaitingAcknowledge);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteRequestData(uint8_t v) -> void {
    if (GetScanMode() != ScanMode::Restart) {
        PushProtocol(EncodeDataRequestResponse(v), CommunicationState::AwaitingAcknowledge);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteByteAcknowledge() -> void {
    if (GetCommunicationState() == CommunicationState::AwaitingByteAcknowledge) {
        SetCommunicationState(CommunicationState::Transmitting);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteAcknowledge(ScanMode m) -> void {
    if (GetCommunicationState() == CommunicationState::AwaitingAcknowledge) {
        const auto previousKeyboardScanActive = KeyboardScanActive();
        SetScanMode(m);
        SetCommunicationState(CommunicationState::Transmitting);
        if (!KeyboardScanActive()) {
            ClearKeyboard();
        } else if (!previousKeyboardScanActive) {
            PushKeyboardState();
        }
        if (!MouseScanActive()) {
            ClearMouse();
        }
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteRequestMousePointer() -> void {
    if (GetScanMode() != ScanMode::Restart) {
        // Push fake mouse data onto queue
        PushMouseMotion(0u, 0u);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteRequestNoOp() -> void {}

auto ArchimedesKeyboard::WriteRequestKeyboardId() -> void {
    if (GetScanMode() != ScanMode::Restart) {
        PushProtocol(KEYBOARD_ID_UK, CommunicationState::AwaitingAcknowledge);
        return;
    }
    WriteError();
}

auto ArchimedesKeyboard::WriteLed(uint8_t) -> void {}

auto ArchimedesKeyboard::WriteError() -> void {
    Clear();
    SetScanMode(ScanMode::Restart);
    SetCommunicationState(CommunicationState::Transmitting);
    PushProtocol(HRST, CommunicationState::AwaitingRestart);
}

auto ArchimedesKeyboard::GetCommunicationState() const -> CommunicationState { return communicationState; }
auto ArchimedesKeyboard::SetCommunicationState(const CommunicationState& s) -> void { communicationState = s; }

auto ArchimedesKeyboard::GetScanMode() const -> ScanMode { return scanMode; }
auto ArchimedesKeyboard::SetScanMode(const ScanMode& m) -> void { scanMode = m; }

auto ArchimedesKeyboard::KeyboardScanActive() const -> bool {
    return GetScanMode() == ScanMode::Keyboard || GetScanMode() == ScanMode::Both;
}

auto ArchimedesKeyboard::MouseScanActive() const -> bool {
    return GetScanMode() == ScanMode::Mouse || GetScanMode() == ScanMode::Both;
}

auto ArchimedesKeyboard::GetKeyboardState(uint8_t code) const -> bool { return keyboardState[code]; }
auto ArchimedesKeyboard::SetKeyboardStateDown(uint8_t code) -> void { keyboardState[code] = true; }
auto ArchimedesKeyboard::SetKeyboardStateUp(uint8_t code) -> void { keyboardState[code] = false; }

auto ArchimedesKeyboard::PushProtocol(uint8_t data, CommunicationState state) -> void {
    Push({ .type=QueueEntryType::Protocol, .data=data, .state=state });
}
auto ArchimedesKeyboard::PushKeyboardState() -> void {
    for (auto code = 0u; code < keyboardState.size(); ++code) {
        if (GetKeyboardState(code)) {
            PushKeyDown(code);
        }
    }
}
auto ArchimedesKeyboard::PushKeyDown(const uint8_t code) -> void {
    if (KeyboardScanActive()) {
        PushPair(QueueEntryType::Keyboard, EncodeKeyDown(code));
    }
    SetKeyboardStateDown(code);
}
auto ArchimedesKeyboard::PushKeyUp(const uint8_t code) -> void {
    if (KeyboardScanActive()) {
        PushPair(QueueEntryType::Keyboard, EncodeKeyUp(code));
    }
    SetKeyboardStateUp(code);
}
auto ArchimedesKeyboard::PushMouseMotion(uint8_t x , uint8_t y) -> void {
    PushPair(QueueEntryType::Mouse, {x, y});
}
auto ArchimedesKeyboard::PushPair(const QueueEntryType& type, const std::pair<uint8_t, uint8_t>& p) -> void {
    Push({ .type=type, .data=p.first, .state=CommunicationState::AwaitingByteAcknowledge });
    Push({ .type=type, .data=p.second, .state=CommunicationState::AwaitingAcknowledge });
}
auto ArchimedesKeyboard::Empty() const -> bool { return queue.empty(); }
auto ArchimedesKeyboard::Push(const QueueEntry& v) -> void { queue.push_back(v); }
auto ArchimedesKeyboard::Pop() -> QueueEntry {
    const auto entry = queue.front();
    queue.pop_front();
    return entry;
}
auto ArchimedesKeyboard::Clear() -> void { queue = {}; }
auto ArchimedesKeyboard::ClearKeyboard() -> void {
    std::erase_if(queue, [](const QueueEntry& e) { return e.type == QueueEntryType::Keyboard; });
}
auto ArchimedesKeyboard::ClearMouse() -> void {
    std::erase_if(queue, [](const QueueEntry& e) { return e.type == QueueEntryType::Mouse; });
}

}