#pragma once

#include "Archimedes/ArchimedesTimer.h"

#include "Common/Util/Shifts.h"

#include <cstdint>
#include <array>

namespace rbrown::acorn::archimedes {

class ArchimedesMediator;

class DiscImage {
public:
    virtual ~DiscImage() = default;
    [[nodiscard]] virtual auto IsWriteProtected() const -> bool = 0;
    [[nodiscard]] virtual auto GetTracks() const -> uint32_t = 0;
    [[nodiscard]] virtual auto GetSectors() const -> uint32_t = 0;
    [[nodiscard]] virtual auto GetUserSectorSizeBytes() const -> uint32_t = 0;
    [[nodiscard]] virtual auto GetTotalSectorSizeBytes() const -> uint32_t = 0;
    [[nodiscard]] virtual auto GetAddressSizeBytes() const -> uint32_t = 0;
    virtual auto SeekToData(uint32_t, uint32_t, uint32_t) -> bool = 0;
    virtual auto ReadByteData() -> uint8_t = 0;
    virtual auto WriteByteData(uint8_t) -> void = 0;
    virtual auto SeekToAddress(uint32_t, uint32_t, uint32_t) -> bool = 0;
    virtual auto ReadByteAddress() -> uint8_t = 0;
};

class WDC1772 {
public:
    WDC1772(ArchimedesMediator&, DiscImage&);

    auto Reset() -> void {}

    auto ReadByteRegister(uint32_t) -> uint8_t;
    auto WriteByteRegister(uint32_t, uint8_t) -> void;

    auto Update(uint32_t) -> void;

    [[nodiscard]] auto ReadRegisterStatus() const -> uint8_t;
    [[nodiscard]] auto ReadRegisterControl() const -> uint8_t;

    auto WriteRegisterStatus(uint8_t) -> void;

    auto SetDrive(uint8_t) -> void;
    auto SetSide(bool) -> void;
    auto SetMotor(bool) -> void;
    auto SetDensity(bool) -> void;

    [[nodiscard]] auto GetHeadPositionTrack() const -> uint32_t;
    [[nodiscard]] auto GetHeadPositionSector() const -> uint32_t;
    auto SetHeadPositionTrack(uint32_t) -> void;
    auto SetHeadPositionSector(uint32_t, uint32_t) -> void;

private:
    using UpdateFunction = void (WDC1772::*)();

    enum class Command {
        Idle,

        Restore,
        Seek,
        Step,
        StepIn,
        StepOut,
        ReadSector,
        WriteSector,
        ReadAddress,
        ForceInterrupt,
    };

    enum class State {
        Idle,

        SeekSpinUpStart,
        SeekSpinUpDone,
        SeekCompareTrackAndDataRegisters,
        SeekUpdateTrackRegister,
        SeekCheckHeadPosition,
        SeekVerifyTrackStart,
        SeekVerifyTrackDone,
        SeekDone,

        SectorSpinUpStart,
        SectorSpinUpDone,
        SectorVerifyWriteProtect,
        SectorVerifyTrackAndSector,
        SectorReadByteSetup,
        SectorReadByte,
        SectorWriteByteSetUp,
        SectorWriteByteFirstDataRequest,
        SectorWriteByteWriteGate,
        SectorWriteByte,
        SectorDone,

        ReadAddressSpinUpStart,
        ReadAddressSpinUpDone,
        ReadAddressVerifyTrackAndSector,
        ReadAddressReadByteSetup,
        ReadAddressReadByte,
        ReadAddressDone,

        ForceInterruptStart,
        ForceInterruptIdleStart,
    };

    [[nodiscard]] auto GetCommand() const -> Command;
    [[nodiscard]] auto GetState() const -> State;
    [[nodiscard]] auto GetUpdateFunction() const -> UpdateFunction;
    [[nodiscard]] auto GetDrive() const -> uint8_t;
    [[nodiscard]] auto GetSide() const -> bool;
    [[nodiscard]] auto GetMotor() const -> bool;
    [[nodiscard]] auto GetDensity() const -> bool;
    [[nodiscard]] auto GetByteCount() const -> uint32_t;

    auto SetCommand(Command) -> void;
    auto SetState(State) -> void;
    auto SetUpdateFunction(UpdateFunction) -> void;
    auto SetByteCount(uint32_t) -> void;

    [[nodiscard]] auto ReadRegisterTrack() const -> uint8_t;
    [[nodiscard]] auto ReadRegisterSector() const -> uint8_t;
    [[nodiscard]] auto ReadRegisterData() const -> uint8_t;
    [[nodiscard]] auto ReadRegisterDirection() const -> uint8_t;

    auto WriteRegisterControl(uint8_t) -> void;
    auto WriteRegisterTrack(uint8_t) -> void;
    auto WriteRegisterSector(uint8_t) -> void;
    auto WriteRegisterData(uint8_t) -> void;
    auto WriteRegisterDirection(uint8_t) -> void;

    auto SetBitsStatus(uint8_t v) -> void;
    auto ClearBitsStatus(uint8_t v) -> void;

    auto SetIdle() -> void;

    auto StartInterruptRequest() -> void;
    auto StartDataRequest() -> void;

    auto EndInterruptRequest() -> void;
    auto EndDataRequest() -> void;

    [[nodiscard]] auto IsWriteProtected() const -> bool;
    [[nodiscard]] auto GetTracks() const -> uint32_t;
    [[nodiscard]] auto GetSectors() const -> uint32_t;
    [[nodiscard]] auto GetUserSectorSizeBytes() const -> uint32_t;
    [[nodiscard]] auto GetTotalSectorSizeBytes() const -> uint32_t;
    [[nodiscard]] auto GetAddressSizeBytes() const -> uint32_t;
    auto SeekToData(uint32_t, uint32_t, uint32_t) -> bool;
    auto ReadByteData() -> uint8_t;
    auto WriteByteData(uint8_t) -> void;
    auto SeekToAddress(uint32_t, uint32_t, uint32_t) -> bool;
    auto ReadByteAddress() -> uint8_t;

    auto StepTrack() -> void;
    auto StepSector() -> void;

    auto ScheduleUpdate(uint32_t) -> void;

    [[nodiscard]] auto GetCommandType() const -> uint32_t;
    [[nodiscard]] auto GetUpdateTrackFlag() const -> bool;
    [[nodiscard]] auto GetMotorOnFlag() const -> bool;
    [[nodiscard]] auto GetVerifyFlag() const -> bool;
    [[nodiscard]] auto GetSteppingRate() const -> uint32_t;
    [[nodiscard]] auto GetSettlingDelayFlag() const -> bool;
    [[nodiscard]] auto GetMultipleSectorsFlag() const -> bool;
    [[nodiscard]] auto GetInterruptCondition() const -> uint32_t;

    [[nodiscard]] auto GetCommandDelay() const -> uint32_t;
    [[nodiscard]] auto GetByteTransferDelay() const -> uint32_t;
    [[nodiscard]] auto static GetByteGapDelay(uint32_t)-> uint32_t;
    [[nodiscard]] auto GetSteppingDelay() const -> uint32_t;
    [[nodiscard]] auto GetVerifyDelay() const -> uint32_t;
    [[nodiscard]] auto GetSettlingDelay() const -> uint32_t;

    auto SetUpCommand() -> void;
    auto SetUpSeek(Command) -> void;
    auto SetUpSector(Command) -> void;
    auto SetUpReadAddress() -> void;
    auto SetUpForceInterrupt() -> void;
    auto SetUpReadTrack() -> void;
    auto SetUpWriteTrack() -> void;

    auto UpdateIdle() -> void;
    auto UpdateSeek() -> void;
    auto UpdateSector() -> void;
    auto UpdateReadAddress() -> void;
    auto UpdateForceInterrupt() -> void;

    ArchimedesMediator& m_mediator;
    DiscImage& m_discImage;

    OneShotTimer<1u> m_timer;
    State m_state;
    Command m_command;
    UpdateFunction m_updateFunction;

    uint8_t m_status;
    uint8_t m_control;
    uint8_t m_track;
    uint8_t m_sector;
    uint8_t m_data;
    uint8_t m_direction;
    uint8_t m_drive;
    uint8_t m_side;
    uint8_t m_motor;
    uint8_t m_density;

    uint32_t m_byteCount;
    uint32_t m_headPositionTrack;
    uint32_t m_headPositionSector;
};

}