#include "Archimedes/WDC1772.h"
#include "Archimedes/ArchimedesMediator.h"

#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

using namespace rbrown::acorn::archimedes;

// Express durations as nanoseconds
constexpr auto Seconds(uint32_t v) -> uint32_t { return 1'000'000'000u * v; }
constexpr auto MilliSeconds(uint32_t v) -> uint32_t { return 1'000'000u * v; }
constexpr auto MicroSeconds(uint32_t v) -> uint32_t { return 1'000u * v; }

constexpr auto MOTOR_RPM = 300u;
constexpr auto MOTOR_RPS = MOTOR_RPM / 60u;
constexpr auto MOTOR_ROTATION_PERIOD_NS = Seconds(1) / MOTOR_RPS;
constexpr auto MOTOR_SPIN_UP_TIME_NS = 6u * MOTOR_ROTATION_PERIOD_NS;

// Datasheet specifies transfer times in kilobits per second
// Multiply by 1024 to give the actual number of bits
// Divide by 8 to get the number of bytes (1024/8=128)
constexpr std::array BYTE_TRANSFER_DELAY_NS = {
    Seconds(1u) / (128u * 125u),
    Seconds(1u) / (128u * 250u)
};
constexpr std::array COMMAND_DELAY_NS = {
    MicroSeconds(48u), MicroSeconds(24u)
};
constexpr std::array STEP_DELAY_NS = {
    MilliSeconds(6u), MilliSeconds(12u), MilliSeconds(20u), MilliSeconds(30u),
    MilliSeconds(6u), MilliSeconds(12u), MilliSeconds(2u), MilliSeconds(3u)
};
constexpr std::array VERIFY_DELAY_NS = {
    MilliSeconds(30u), MilliSeconds(15u)
};
constexpr std::array SETTLING_DELAY_NS = {
    MilliSeconds(30u), MilliSeconds(15u)
};

constexpr auto BYTE_GAP_DELAY_DATA_REQUEST = 2u;
constexpr auto BYTE_GAP_DELAY_WRITE_GATE = 9u;
constexpr auto BYTE_GAP_DELAY_DATA_ADDRESS_MARK = 6u;

constexpr auto FORCE_INTERRUPT = 0xDu;

constexpr auto STATUS_MOTOR_ON = 0x80u;
constexpr auto STATUS_WRITE_PROTECT = 0x40u;
constexpr auto STATUS_SPIN_UP = 0x20u;
constexpr auto STATUS_RECORD_NOT_FOUND = 0x10u;
constexpr auto STATUS_CRC_ERROR = 0x08u;
constexpr auto STATUS_TRACK_00 = 0x04u;
constexpr auto STATUS_DATA_REQUEST = 0x02u;
constexpr auto STATUS_BUSY = 0x01u;

constexpr auto STATUS_RECORD_TYPE = 0x20u;
constexpr auto STATUS_LOST_DATA = 0x04u;

constexpr uint8_t STEP_OUT = 0xFFu;
constexpr uint8_t STEP_IN = 1u;

constexpr auto ExtractRegisterAddress(uint32_t v) { return ExtractBitField(v, 0u, 2u); }

constexpr auto ExtractCommandType(uint8_t v) -> uint32_t { return ExtractBitField(v, 4u, 4u); }
constexpr auto ExtractUpdateTrackRegister(uint8_t v) -> bool { return BitSet(v, 4u); }
constexpr auto ExtractMotorOn(uint8_t v) -> bool { return !BitSet(v, 3u); }
constexpr auto ExtractVerify(uint8_t v) -> bool { return BitSet(v, 2u); }
constexpr auto ExtractSteppingRate(uint8_t v) -> uint32_t { return ExtractBitField(v, 0u, 2u); }

constexpr auto ExtractMultipleSectors(uint8_t v) -> bool { return BitSet(v, 4u); }
constexpr auto ExtractSettlingDelay(uint8_t v) -> bool { return BitSet(v, 2u); }

constexpr auto ExtractInterruptCondition(uint8_t v) -> uint32_t { return ExtractBitField(v, 0u, 4u); }

WDC1772::WDC1772(ArchimedesMediator& m, DiscImage& d) :
    m_mediator{ m },
    m_discImage{ d },
    m_timer{},
    m_state{ State::Idle },
    m_command{ Command::Idle },
    m_updateFunction{ &WDC1772::UpdateIdle },
    m_status{ STATUS_TRACK_00 },
    m_control{},
    m_track{},
    m_sector{},
    m_data{},
    m_direction{ STEP_OUT },
    m_drive{},
    m_side{},
    m_motor{},
    m_density{ true },
    m_byteCount{},
    m_headPositionTrack{},
    m_headPositionSector{} {}

auto WDC1772::GetState() const -> State { return m_state; }
auto WDC1772::GetCommand() const -> Command { return m_command; }
auto WDC1772::GetUpdateFunction() const -> UpdateFunction { return m_updateFunction; }

auto WDC1772::GetDrive() const -> uint8_t { return m_drive; }
auto WDC1772::GetSide() const-> bool { return m_side; }
auto WDC1772::GetMotor() const-> bool { return m_motor; }
auto WDC1772::GetDensity() const->  bool { return m_density; }
auto WDC1772::GetByteCount() const -> uint32_t { return m_byteCount; }
auto WDC1772::GetHeadPositionTrack() const -> uint32_t { return m_headPositionTrack; }
auto WDC1772::GetHeadPositionSector() const -> uint32_t { return m_headPositionSector; }

auto WDC1772::SetState(State state) -> void { m_state = state; }
auto WDC1772::SetCommand(Command command) -> void { m_command = command; }
auto WDC1772::SetUpdateFunction(UpdateFunction function) -> void { m_updateFunction = function; }

auto WDC1772::SetDrive(uint8_t v) -> void { m_drive = v; }
auto WDC1772::SetSide(bool v) -> void { m_side = v; }
auto WDC1772::SetMotor(bool v) -> void { m_motor = v; }
auto WDC1772::SetDensity(bool v) -> void { m_density = v; }
auto WDC1772::SetByteCount(uint32_t v) -> void { m_byteCount = v; }
auto WDC1772::SetHeadPositionTrack(uint32_t v) -> void { m_headPositionTrack = v; }
auto WDC1772::SetHeadPositionSector(uint32_t v, uint32_t sectors) -> void {
    m_headPositionSector = v % std::max(1u, sectors);
}

auto WDC1772::ReadRegisterControl() const -> uint8_t { return m_control; }
auto WDC1772::ReadRegisterStatus() const -> uint8_t { return m_status; }
auto WDC1772::ReadRegisterTrack()  const -> uint8_t { return m_track; }
auto WDC1772::ReadRegisterSector() const -> uint8_t { return m_sector; }
auto WDC1772::ReadRegisterData() const -> uint8_t { return m_data; }
auto WDC1772::ReadRegisterDirection() const -> uint8_t { return m_direction; }

auto WDC1772::WriteRegisterControl(uint8_t v) -> void { m_control = v; }
auto WDC1772::WriteRegisterStatus(uint8_t v) -> void { m_status = v; }
auto WDC1772::WriteRegisterTrack(uint8_t v) -> void { m_track = v; }
auto WDC1772::WriteRegisterSector(uint8_t v) -> void { m_sector = v; }
auto WDC1772::WriteRegisterData(uint8_t v) -> void { m_data = v; }
auto WDC1772::WriteRegisterDirection(uint8_t v) -> void { m_direction = v; }

auto WDC1772::SetBitsStatus(uint8_t v) -> void { m_status |= v; }
auto WDC1772::ClearBitsStatus(uint8_t v) -> void { m_status &= ~v; }

auto WDC1772::SetIdle() -> void {
    SetUpdateFunction(&WDC1772::UpdateIdle);
    SetState(State::Idle);
    SetCommand(Command::Idle);
    ClearBitsStatus(STATUS_BUSY);
}

auto WDC1772::StartInterruptRequest() -> void {
    SetIdle();
    m_mediator.StartFloppyDiscInterruptRequest();
}
auto WDC1772::StartDataRequest() -> void {
    SetBitsStatus(STATUS_DATA_REQUEST);
    m_mediator.StartFloppyDiscDataRequest();
}
auto WDC1772::EndInterruptRequest() -> void {
    m_mediator.EndFloppyDiscInterruptRequest();
}
auto WDC1772::EndDataRequest() -> void {
    m_mediator.EndFloppyDiscDataRequest();
}

// Note: we wrap all the calls to the disc in case we decide to support multiple drives
auto WDC1772::IsWriteProtected() const -> bool { return m_discImage.IsWriteProtected(); }
auto WDC1772::GetTracks() const -> uint32_t { return m_discImage.GetTracks(); }
auto WDC1772::GetSectors() const -> uint32_t { return m_discImage.GetSectors(); }
auto WDC1772::GetUserSectorSizeBytes() const -> uint32_t { return m_discImage.GetUserSectorSizeBytes(); }
auto WDC1772::GetTotalSectorSizeBytes() const -> uint32_t { return m_discImage.GetTotalSectorSizeBytes(); }
auto WDC1772::GetAddressSizeBytes() const -> uint32_t { return m_discImage.GetAddressSizeBytes(); }
auto WDC1772::SeekToData(uint32_t side, uint32_t track, uint32_t sector) -> bool {
    return m_discImage.SeekToData(side, track, sector);
}
auto WDC1772::ReadByteData() -> uint8_t { return m_discImage.ReadByteData(); }
auto WDC1772::WriteByteData(uint8_t v) -> void { m_discImage.WriteByteData(v); }
auto WDC1772::SeekToAddress(uint32_t side, uint32_t track, uint32_t sector) -> bool {
    return m_discImage.SeekToAddress(side, track, sector);
}
auto WDC1772::ReadByteAddress() -> uint8_t { return m_discImage.ReadByteAddress(); }

auto WDC1772::StepTrack() -> void {
    const auto current = GetHeadPositionTrack();
    if (ReadRegisterDirection() == STEP_IN) {
        SetHeadPositionTrack(current + 1u);
    } else if (current) {
        SetHeadPositionTrack(current - 1u);
    }
}

auto WDC1772::StepSector() -> void {
    SetHeadPositionSector(GetHeadPositionSector() + 1u, GetSectors());
}

auto WDC1772::ReadByteRegister(uint32_t address) -> uint8_t {
    switch (ExtractBitField(address, 0u, 2u)) {
        case 0u: {
            EndInterruptRequest();
            return ReadRegisterStatus();
        }
        case 1u: return ReadRegisterTrack();
        case 2u: return ReadRegisterSector();
        case 3u: {
            ClearBitsStatus(STATUS_DATA_REQUEST);
            EndDataRequest();
            return ReadRegisterData();
        }
            [[unlikely]] default: return 0xFFu;
    }
}

auto WDC1772::WriteByteRegister(uint32_t address, uint8_t v) -> void {
    if (GetState() == State::Idle) {
        switch (ExtractRegisterAddress(address)) {
            case 0u: {
                WriteRegisterControl(v);
                SetUpCommand();
                return;
            }
            case 1u: return WriteRegisterTrack(v);
            case 2u: return WriteRegisterSector(v);
            case 3u: {
                ClearBitsStatus(STATUS_DATA_REQUEST);
                EndDataRequest();
                return WriteRegisterData(v);
            }
                [[unlikely]] default: return;
        }
    }
    switch (ExtractRegisterAddress(address)) {
        case 0u: {
            if (ExtractCommandType(v) == FORCE_INTERRUPT) {
                WriteRegisterControl(v);
                SetUpCommand();
            } else {
                spdlog::debug("WDC1772 write to control while busy");
            }
            return;
        }
        case 1u: [[fallthrough]];
        case 2u: {
            spdlog::debug("WDC1772 write to track or sector while busy");
        }
        case 3u: {
            ClearBitsStatus(STATUS_DATA_REQUEST);
            EndDataRequest();
            return WriteRegisterData(v);
        }
            [[unlikely]] default: return;
    }
}

auto WDC1772::Update(uint32_t time) -> void {
    m_timer.Update(time, [this]() { (this->*GetUpdateFunction())(); });
}

auto WDC1772::ScheduleUpdate(uint32_t delay) -> void { m_timer.Go(delay); }

auto WDC1772::GetCommandType() const -> uint32_t { return ExtractCommandType(ReadRegisterControl()); }
auto WDC1772::GetUpdateTrackFlag() const -> bool {
    return ExtractUpdateTrackRegister(ReadRegisterControl());
}
auto WDC1772::GetMotorOnFlag() const -> bool { return ExtractMotorOn(ReadRegisterControl()); }
auto WDC1772::GetVerifyFlag() const -> bool { return ExtractVerify(ReadRegisterControl()); }
auto WDC1772::GetSteppingRate() const -> uint32_t { return ExtractSteppingRate(ReadRegisterControl()); }

auto WDC1772::GetMultipleSectorsFlag() const -> bool { return ExtractMultipleSectors(ReadRegisterControl()); }
auto WDC1772::GetSettlingDelayFlag() const -> bool { return ExtractSettlingDelay(ReadRegisterControl()); }

auto WDC1772::GetInterruptCondition() const -> uint32_t {
    return ExtractInterruptCondition(ReadRegisterControl());
}

auto WDC1772::GetCommandDelay() const -> uint32_t { return COMMAND_DELAY_NS[GetDensity()]; }
auto WDC1772::GetByteTransferDelay() const -> uint32_t { return BYTE_TRANSFER_DELAY_NS[GetDensity()]; }
auto WDC1772::GetByteGapDelay(uint32_t v)  -> uint32_t {
    // Note: the documentation implies that the WDC1772 waits the same amount of time regardless of density
    // In single density mode a 6 byte gap is 6 bytes, in double density mode a 6 byte gap is in fact 12
    return v * BYTE_TRANSFER_DELAY_NS[0u];
}
auto WDC1772::GetSteppingDelay() const -> uint32_t {
    return STEP_DELAY_NS[4u * GetDensity() + GetSteppingRate()];
}
auto WDC1772::GetVerifyDelay() const -> uint32_t { return VERIFY_DELAY_NS[GetDensity()]; }
auto WDC1772::GetSettlingDelay() const -> uint32_t { return SETTLING_DELAY_NS[GetDensity()]; }

auto WDC1772::SetUpCommand() -> void {
    EndInterruptRequest();
    EndDataRequest();
    switch (GetCommandType()) {
        case 0x0u: return SetUpSeek(Command::Restore);
        case 0x1u: return SetUpSeek(Command::Seek);
        case 0x2u:
        case 0x3u: return SetUpSeek(Command::Step);
        case 0x4u:
        case 0x5u: return SetUpSeek(Command::StepIn);
        case 0x6u:
        case 0x7u: return SetUpSeek(Command::StepOut);
        case 0x8u:
        case 0x9u: return SetUpSector(Command::ReadSector);
        case 0xAu:
        case 0xBu: return SetUpSector(Command::WriteSector);
        case 0xCu: return SetUpReadAddress();
        case 0xDu: return SetUpForceInterrupt();
        case 0xEu: return SetUpReadTrack();
        case 0xFu: return SetUpWriteTrack();
            [[unlikely]] default: return;
    }
}

auto WDC1772::SetUpSeek(Command command) -> void {
    SetCommand(command);
    SetUpdateFunction(&WDC1772::UpdateSeek);
    SetState(State::SeekSpinUpStart);
    ScheduleUpdate(GetCommandDelay());
}
auto WDC1772::SetUpSector(Command command) -> void {
    SetCommand(command);
    SetUpdateFunction(&WDC1772::UpdateSector);
    SetState(State::SectorSpinUpStart);
    ScheduleUpdate(GetCommandDelay());
}
auto WDC1772::SetUpReadAddress() -> void {
    SetCommand(Command::ReadAddress);
    SetUpdateFunction(&WDC1772::UpdateReadAddress);
    SetState(State::ReadAddressSpinUpStart);
    ScheduleUpdate(GetCommandDelay());
}
auto WDC1772::SetUpForceInterrupt() -> void {
    const auto condition = GetInterruptCondition();
    if (condition) {
        spdlog::debug("WDC1772 Force Interrupt {:02x}", condition);
    }
    SetCommand(Command::ForceInterrupt);
    SetUpdateFunction(&WDC1772::UpdateForceInterrupt);
    if (GetState() == State::Idle) {
        SetState(State::ForceInterruptIdleStart);
    } else {
        SetState(State::ForceInterruptStart);
    }
    ScheduleUpdate(GetCommandDelay());
}
auto WDC1772::SetUpReadTrack() -> void { spdlog::debug("WDC1772 Read Track"); }
auto WDC1772::SetUpWriteTrack() -> void { spdlog::debug("WDC1772 Write Track"); }

auto WDC1772::UpdateIdle() -> void {}

auto WDC1772::UpdateSeek() -> void {
    if (GetState() == State::SeekSpinUpStart) {
        ClearBitsStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
        SetBitsStatus(STATUS_MOTOR_ON | STATUS_BUSY);
        if (!GetMotor() && GetMotorOnFlag()) {
            SetState(State::SeekSpinUpDone);
            ScheduleUpdate(MOTOR_SPIN_UP_TIME_NS);
            return;
        }
        SetState(State::SeekSpinUpDone);
    }
    if (GetState() == State::SeekSpinUpDone) {
        SetBitsStatus(STATUS_SPIN_UP);
        if (GetCommand() == Command::Restore) {
            WriteRegisterTrack(0xFFu);
            WriteRegisterData(0u);
            SetState(State::SeekCompareTrackAndDataRegisters);
        } else if (GetCommand() == Command::Seek) {
            SetState(State::SeekCompareTrackAndDataRegisters);
        } else if (GetCommand() == Command::StepOut) {
            WriteRegisterDirection(STEP_OUT);
            SetState(State::SeekUpdateTrackRegister);
        } else if (GetCommand() == Command::StepIn) {
            WriteRegisterDirection(STEP_IN);
            SetState(State::SeekUpdateTrackRegister);
        } else if (GetCommand() == Command::Step) {
            SetState(State::SeekUpdateTrackRegister);
        }
    }
    if (GetState() == State::SeekCompareTrackAndDataRegisters) {
        if (ReadRegisterTrack() == ReadRegisterData()) {
            SetState(State::SeekVerifyTrackStart);
        } else if (ReadRegisterTrack() > ReadRegisterData()) {
            WriteRegisterDirection(STEP_OUT);
            SetState(State::SeekUpdateTrackRegister);
        } else {
            WriteRegisterDirection(STEP_IN);
            SetState(State::SeekUpdateTrackRegister);
        }
    }
    if (GetState() == State::SeekUpdateTrackRegister) {
        if (GetCommand() == Command::Restore ||
            GetCommand() == Command::Seek ||
            GetUpdateTrackFlag()) {
            WriteRegisterTrack(ReadRegisterTrack() + ReadRegisterDirection());
        }
        SetState(State::SeekCheckHeadPosition);
    }
    if (GetState() == State::SeekCheckHeadPosition) {
        if (GetHeadPositionTrack() == 0u && ReadRegisterDirection() == STEP_OUT) {
            SetBitsStatus(STATUS_TRACK_00);
            WriteRegisterTrack(0u);
            SetState(State::SeekVerifyTrackStart);
        } else if (GetCommand() == Command::Restore || GetCommand() == Command::Seek) {
            StepTrack();
            SetState(State::SeekCompareTrackAndDataRegisters);
            ScheduleUpdate(GetSteppingDelay());
            return;
        } else {
            StepTrack();
            SetState(State::SeekVerifyTrackStart);
            ScheduleUpdate(GetSteppingDelay());
            return;
        }
    }
    if (GetState() == State::SeekVerifyTrackStart) {
        if (GetVerifyFlag()) {
            SetState(State::SeekVerifyTrackDone);
            ScheduleUpdate(GetVerifyDelay());
            return;
        }
        SetState(State::SeekDone);
    }
    if (GetState() == State::SeekVerifyTrackDone) {
         if (GetHeadPositionTrack() != ReadRegisterTrack() ||
            ReadRegisterTrack() >= GetTracks()) {
            SetBitsStatus(STATUS_RECORD_NOT_FOUND);
        }
        SetState(State::SeekDone);
    }
    if (GetState() == State::SeekDone) {
        StartInterruptRequest();
    }
}

auto WDC1772::UpdateSector() -> void {
    if (GetState() == State::SectorSpinUpStart) {
        ClearBitsStatus(
            STATUS_WRITE_PROTECT |
            STATUS_RECORD_TYPE |
            STATUS_RECORD_NOT_FOUND |
            STATUS_CRC_ERROR |
            STATUS_LOST_DATA |
            STATUS_DATA_REQUEST);
        SetBitsStatus(STATUS_MOTOR_ON | STATUS_BUSY);
        if (!GetMotor() && GetMotorOnFlag()) {
            SetState(State::SectorSpinUpDone);
            ScheduleUpdate(MOTOR_SPIN_UP_TIME_NS);
            return;
        }
        SetState(State::SectorSpinUpDone);
    }
    if (GetState() == State::SectorSpinUpDone) {
        if (GetSettlingDelayFlag()) {
            SetState(State::SectorVerifyWriteProtect);
            ScheduleUpdate(GetSettlingDelay());
            return;
        }
        SetState(State::SectorVerifyWriteProtect);
    }
    write_protect:
    if (GetState() == State::SectorVerifyWriteProtect) {
        if (GetCommand() == Command::WriteSector && IsWriteProtected()) {
            SetBitsStatus(STATUS_WRITE_PROTECT);
            SetState(State::SectorDone);
        } else {
            SetState(State::SectorVerifyTrackAndSector);
        }
    }
    if (GetState() == State::SectorVerifyTrackAndSector) {
        if (GetHeadPositionTrack() == ReadRegisterTrack() &&
            SeekToData(GetSide(), ReadRegisterTrack(), ReadRegisterSector())) {
            SetHeadPositionSector(ReadRegisterSector(), GetSectors());
            if (GetCommand() == Command::ReadSector) {
                SetState(State::SectorReadByteSetup);
            } else {
                SetState(State::SectorWriteByteSetUp);
            }
        } else {
            SetBitsStatus(STATUS_RECORD_NOT_FOUND);
            SetState(State::SectorDone);
        }
    }
    // Read related
    if (GetState() == State::SectorReadByteSetup) {
        // Note: we're supposed to flag if the sector is deleted by setting status register bit 5
        // however, the disc images we're working with only contain the user data portions,
        // so we don't know if the sector is deleted or not
        SetByteCount(GetUserSectorSizeBytes());
        SetState(State::SectorReadByte);
        ScheduleUpdate(GetByteTransferDelay());
        return;
    }
    if (GetState() == State::SectorReadByte) {
        if (ReadRegisterStatus() & STATUS_DATA_REQUEST) {
            SetBitsStatus(STATUS_LOST_DATA);
        }
        WriteRegisterData(ReadByteData());
        StartDataRequest();
        SetByteCount(GetByteCount() - 1u);
        if (GetByteCount()) {
            SetState(State::SectorReadByte);
            ScheduleUpdate(GetByteTransferDelay());
            return;
        } else if (GetMultipleSectorsFlag()) {
            WriteRegisterSector(ReadRegisterSector() + 1u);
            StepSector();
            SetState(State::SectorVerifyWriteProtect);
            goto write_protect;
        } else {
            StepSector();
            SetState(State::SectorDone);
        }
    }
    // Write related
    if (GetState() == State::SectorWriteByteSetUp) {
        SetByteCount(GetUserSectorSizeBytes());
        SetState(State::SectorWriteByteFirstDataRequest);
        ScheduleUpdate(GetByteGapDelay(BYTE_GAP_DELAY_DATA_REQUEST));
        return;
    }
    if (GetState() == State::SectorWriteByteFirstDataRequest) {
        StartDataRequest();
        SetState(State::SectorWriteByteWriteGate);
        ScheduleUpdate(GetByteGapDelay(BYTE_GAP_DELAY_WRITE_GATE));
        return;
    }
    if (GetState() == State::SectorWriteByteWriteGate) {
        if (ReadRegisterStatus() & STATUS_DATA_REQUEST) {
            StepSector();
            SetBitsStatus(STATUS_LOST_DATA);
            SetState(State::SectorDone);
        } else {
            // Note: since the file format we're working with only contains user data
            // The deleted data mark isn't implementable, so we skip the data mark step
            // and go directly to writing after the appropriate delay
            SetState(State::SectorWriteByte);
            ScheduleUpdate(GetByteGapDelay(BYTE_GAP_DELAY_DATA_ADDRESS_MARK));
            return;
        }
    }
    if (GetState() == State::SectorWriteByte) {
        if (ReadRegisterStatus() & STATUS_DATA_REQUEST) {
            SetBitsStatus(STATUS_LOST_DATA);
            WriteByteData(0u);
        } else {
            WriteByteData(ReadRegisterData());
        }
        SetByteCount(GetByteCount() - 1u);
        if (GetByteCount()) {
            StartDataRequest();
            SetState(State::SectorWriteByte);
            ScheduleUpdate(GetByteTransferDelay());
            return;
        } else if (GetMultipleSectorsFlag()) {
            WriteRegisterSector(ReadRegisterSector() + 1u);
            StepSector();
            SetState(State::SectorVerifyWriteProtect);
            goto write_protect;
        } else {
            StepSector();
            SetState(State::SectorDone);
        }
    }
    if (GetState() == State::SectorDone) {
        StartInterruptRequest();
    }
}

auto WDC1772::UpdateReadAddress() -> void {
    if (GetState() == State::ReadAddressSpinUpStart) {
        ClearBitsStatus(
            STATUS_RECORD_NOT_FOUND |
            STATUS_CRC_ERROR |
            STATUS_LOST_DATA |
            STATUS_DATA_REQUEST);
        SetBitsStatus(STATUS_MOTOR_ON | STATUS_BUSY);
        if (!GetMotor() && GetMotorOnFlag()) {
            SetState(State::ReadAddressSpinUpDone);
            ScheduleUpdate(MOTOR_SPIN_UP_TIME_NS);
            return;
        }
        SetState(State::ReadAddressSpinUpDone);
    }
    if (GetState() == State::ReadAddressSpinUpDone) {
        if (GetSettlingDelayFlag()) {
            SetState(State::ReadAddressVerifyTrackAndSector);
            ScheduleUpdate(GetSettlingDelay());
            return;
        }
        SetState(State::ReadAddressVerifyTrackAndSector);
    }
    if (GetState() == State::ReadAddressVerifyTrackAndSector) {
        if (SeekToAddress(GetSide(), GetHeadPositionTrack(), GetHeadPositionSector())) {
            SetState(State::ReadAddressReadByteSetup);
        } else {
            SetBitsStatus(STATUS_RECORD_NOT_FOUND);
            SetState(State::ReadAddressDone);
        }
    }
    if (GetState() == State::ReadAddressReadByteSetup) {
        SetByteCount(GetAddressSizeBytes());
        SetState(State::ReadAddressReadByte);
        ScheduleUpdate(GetByteTransferDelay());
        return;
    }
    if (GetState() == State::ReadAddressReadByte) {
        if (ReadRegisterStatus() & STATUS_DATA_REQUEST) {
            SetBitsStatus(STATUS_LOST_DATA);
        }
        WriteRegisterData(ReadByteAddress());
        StartDataRequest();
        SetByteCount(GetByteCount() - 1u);
        if (GetByteCount()) {
            SetState(State::ReadAddressReadByte);
            ScheduleUpdate(GetByteTransferDelay());
            return;
        } else {
            // Note: RISC OS 3's first interaction with a disc is a read address command
            // After the first command completes it sets up a counter to expire in .21s
            // (Slightly more than the time for a single revolution at 300RPM) and then issues
            // a set number of further read address commands.
            //
            // The commands will only successfully terminate if the counter expires.
            //
            // Therefore, the read address command must simulate the actual drive timings to work, and
            // immediately returning the 6 requested bytes do not give sufficient time for the counter to expire.
            //
            // The following approaches were attempted and not found suitable:
            // 1. User bytes in sector * Byte transfer delay (too fast)
            // 2. Single motor revolution time / sectors per track (too fast)
            //
            // What was found to work for ADFS-E format discs was to take the number of non-user bytes in the sector,
            // add to that the number of user bytes in the sector, and multiply that by the byte transfer rate
            // This is slightly odd because this gives you about 3.45 sectors per track.
            //
            // Note: the documentation states that a sector is the smallest unit
            // the WDC1772 can work with. On assumes therefore that the rest of the
            // sector has to be read and thrown away.
            StepSector();
            WriteRegisterSector(GetHeadPositionTrack());
            SetState(State::ReadAddressDone);
            const auto remaining = GetTotalSectorSizeBytes() - GetAddressSizeBytes();
            ScheduleUpdate(remaining * GetByteTransferDelay());
            return;
        }
    }
    if (GetState() == State::ReadAddressDone) {
        StartInterruptRequest();
    }
}

auto WDC1772::UpdateForceInterrupt() -> void {
    if (GetState() == State::ForceInterruptIdleStart) {
        // Restore the Track 00 bit in the status register
        SetIdle();
        if (GetHeadPositionTrack() == 0x00u) {
            SetBitsStatus(STATUS_TRACK_00);
        }
        return;
    }
    if (GetState() == State::ForceInterruptStart) {
        SetIdle();
        return;
    }
}