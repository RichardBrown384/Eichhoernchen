#include "Archimedes/IocTest.h"

class InterruptEventsTest: public IocTest {
public:
    ~InterruptEventsTest() override = default;
};

TEST_F(InterruptEventsTest, FlyBackEventsSetTheAppropriateBitsInIrqA) {
    const auto irqb = ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS);
    const auto fiq = ReadByteInternalRegister(ADDRESS_FIQ_STATUS);
    WriteByteInternalRegister(ADDRESS_IRQ_A_CLEAR, 0xFFu);
    WriteByteInternalRegister(ADDRESS_IRQ_A_MASK, 0xFFu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    StartFlyBack();
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(fiq, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    EndFlyBack();
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(fiq, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
}

TEST_F(InterruptEventsTest, SoundInterruptsSetTheAppropriateBitsInIrqB) {
    const auto irqa = ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS);
    const auto fiq = ReadByteInternalRegister(ADDRESS_FIQ_STATUS);
    WriteByteInternalRegister(ADDRESS_IRQ_B_MASK, 0xFFu);
    WriteByteInternalRegister(ADDRESS_SERIAL_TRANSMIT, 0xFFu);
    ReadByteInternalRegister(ADDRESS_SERIAL_RECEIVE);
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    StartSoundInterrupt();
    EXPECT_EQ(0x02u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(fiq, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    EndSoundInterrupt();
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(fiq, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
}

TEST_F(InterruptEventsTest, FloppyDiscDataRequestsSetTheAppropriateBitsInFiq) {
    const auto irqa = ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS);
    const auto irqb = ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS);
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0xFFu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    StartFloppyDiscDataRequest();
    EXPECT_EQ(0x81u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EndFloppyDiscDataRequest();
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
}

TEST_F(InterruptEventsTest, FloppyDiscInterruptRequestsSetTheAppropriateBitsInFiq) {
    const auto irqa = ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS);
    const auto irqb = ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS);
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0xFFu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    StartFloppyDiscInterruptRequest();
    EXPECT_EQ(0x82u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EndFloppyDiscInterruptRequest();
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(irqa, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(irqb, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
}