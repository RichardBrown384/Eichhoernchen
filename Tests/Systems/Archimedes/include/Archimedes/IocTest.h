#include "Archimedes/ArchimedesIoc.h"
#include "Archimedes/ArchimedesMock.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

constexpr auto ADDRESS_CONTROL = 0x00u;

constexpr auto ADDRESS_SERIAL_TRANSMIT = 0x04u;
constexpr auto ADDRESS_SERIAL_RECEIVE = 0x04u;

constexpr auto ADDRESS_IRQ_A_STATUS = 0x10u;
constexpr auto ADDRESS_IRQ_A_CLEAR = 0x14u;
constexpr auto ADDRESS_IRQ_A_REQUEST = 0x14;
constexpr auto ADDRESS_IRQ_A_MASK = 0x18u;

constexpr auto ADDRESS_IRQ_B_STATUS = 0x20u;
constexpr auto ADDRESS_IRQ_B_REQUEST = 0x24;
constexpr auto ADDRESS_IRQ_B_MASK = 0x28u;

constexpr auto ADDRESS_FIQ_STATUS = 0x30u;
constexpr auto ADDRESS_FIQ_REQUEST = 0x34;
constexpr auto ADDRESS_FIQ_MASK = 0x38u;

constexpr auto ADDRESS_TIMER_0_LATCH_LOW = 0x40u;
constexpr auto ADDRESS_TIMER_0_LATCH_HIGH = 0x44u;
constexpr auto ADDRESS_TIMER_0_GO_COMMAND = 0x48u;
constexpr auto ADDRESS_TIMER_0_LATCH_COMMAND = 0x4Cu;
constexpr auto ADDRESS_TIMER_0_COUNT_LOW = 0x40u;
constexpr auto ADDRESS_TIMER_0_COUNT_HIGH = 0x44u;

constexpr auto ADDRESS_TIMER_1_LATCH_LOW = 0x50u;
constexpr auto ADDRESS_TIMER_1_LATCH_HIGH = 0x54u;
constexpr auto ADDRESS_TIMER_1_GO_COMMAND = 0x58u;
constexpr auto ADDRESS_TIMER_1_LATCH_COMMAND = 0x5Cu;
constexpr auto ADDRESS_TIMER_1_COUNT_LOW = 0x50u;
constexpr auto ADDRESS_TIMER_1_COUNT_HIGH = 0x54u;

constexpr auto ADDRESS_TIMER_2_LATCH_LOW = 0x60u;
constexpr auto ADDRESS_TIMER_2_LATCH_HIGH = 0x64u;
constexpr auto ADDRESS_TIMER_2_GO_COMMAND = 0x68u;
constexpr auto ADDRESS_TIMER_2_LATCH_COMMAND = 0x6Cu;
constexpr auto ADDRESS_TIMER_2_COUNT_LOW = 0x60u;
constexpr auto ADDRESS_TIMER_2_COUNT_HIGH = 0x64u;

constexpr auto ADDRESS_TIMER_3_LATCH_LOW = 0x70u;
constexpr auto ADDRESS_TIMER_3_LATCH_HIGH = 0x74u;
constexpr auto ADDRESS_TIMER_3_GO_COMMAND = 0x78u;
constexpr auto ADDRESS_TIMER_3_LATCH_COMMAND = 0x7Cu;
constexpr auto ADDRESS_TIMER_3_COUNT_LOW = 0x70u;
constexpr auto ADDRESS_TIMER_3_COUNT_HIGH = 0x74u;

constexpr auto CYCLE_SLOW = 0u;
constexpr auto CYCLE_MEDIUM = 1u;
constexpr auto CYCLE_FAST = 2u;
constexpr auto CYCLE_SYNC = 3u;

struct IocAddress {
    uint32_t bank;
    uint32_t cycle;
    uint32_t address;
};

class IocTest: public testing::Test {
public:
    IocTest() : mediator{}, ioc{mediator} {}
    ~IocTest() override = default;

    auto StartFlyBack() -> void;
    auto EndFlyBack() -> void;

    auto StartSoundInterrupt() -> void;
    auto EndSoundInterrupt() -> void;

    auto StartFloppyDiscDataRequest() -> void;
    auto StartFloppyDiscInterruptRequest() -> void;

    auto EndFloppyDiscDataRequest() -> void;
    auto EndFloppyDiscInterruptRequest() -> void;

    auto ReadWordInternalRegister(uint32_t) -> uint32_t;
    auto ReadByteInternalRegister(uint32_t) -> uint32_t;

    auto ReadWord(const IocAddress&) -> uint32_t;
    auto ReadByte(const IocAddress&) -> uint32_t;

    auto WriteWordInternalRegister(uint32_t, uint32_t) -> void;
    auto WriteByteInternalRegister(uint32_t, uint32_t) -> void;

    auto WriteByte(const IocAddress&, uint32_t) -> void;
    auto WriteWord(const IocAddress&, uint32_t) -> void;
protected:
    testing::NiceMock<ArchimedesMock> mediator;
private:
    rbrown::acorn::archimedes::ArchimedesIoc ioc;
};