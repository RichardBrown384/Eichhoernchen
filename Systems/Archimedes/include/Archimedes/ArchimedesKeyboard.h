#pragma once

#include <cstdint>
#include <queue>

namespace rbrown::acorn::archimedes {

class ArchimedesKeyboard {
public:
    ArchimedesKeyboard();
    auto KeyDown(uint8_t) -> void;
    auto KeyUp(uint8_t) -> void;
    auto MouseMotion(uint8_t, uint8_t) -> void;
    auto MouseButtonDown(uint8_t) -> void;
    auto MouseButtonUp(uint8_t) -> void;
    auto ReadByte(uint8_t&) -> bool;
    auto WriteByte(uint8_t) -> void;
private:
    enum class ScanMode {
        Restart,
        None,
        Keyboard,
        Mouse,
        Both,
    };
    enum class CommunicationState {
        AwaitingRestart,
        AwaitingRestartAcknowledge1,
        AwaitingRestartAcknowledge2,
        AwaitingByteAcknowledge,
        AwaitingAcknowledge,
        Transmitting,
    };
    struct QueueEntry {
        uint8_t data;
        CommunicationState state;
    };

    auto WriteHardwareRestart() -> void;
    auto WriteHardwareRestartAcknowledge1() -> void;
    auto WriteHardwareRestartAcknowledge2() -> void;

    auto WriteRequestData(uint8_t) -> void;

    auto WriteByteAcknowledge() -> void;
    auto WriteAcknowledge(ScanMode) -> void;

    auto WriteRequestMousePointer() -> void;
    static auto WriteRequestNoOp() -> void;
    auto WriteRequestKeyboardId() -> void;

    static auto WriteLed(uint8_t) -> void;

    auto WriteError() -> void;

    [[nodiscard]] auto GetCommunicationState() const -> CommunicationState;
    [[nodiscard]] auto GetScanMode() const -> ScanMode;
    auto SetCommunicationState(const CommunicationState&) -> void;
    auto SetScanMode(const ScanMode&) -> void;

    [[nodiscard]] auto KeyboardScanActive() const -> bool;
    [[nodiscard]] auto MouseScanActive() const -> bool;

    [[nodiscard]] auto Empty() const -> bool;

    auto PushKeyDown(uint8_t) -> void;
    auto PushKeyUp(uint8_t) -> void;
    auto PushPair(const std::pair<uint8_t, uint8_t>&) -> void;
    auto Push(const QueueEntry&) -> void;
    auto Pop() -> QueueEntry;
    auto Clear() -> void;
    
    CommunicationState communicationState;
    ScanMode scanMode;
    std::queue<QueueEntry> queue;

};

}