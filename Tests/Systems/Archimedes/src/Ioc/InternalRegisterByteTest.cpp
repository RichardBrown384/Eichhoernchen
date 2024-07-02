#include "Archimedes/IocTest.h"

class IocInternalRegisterByteTest : public IocTest {
public:
    ~IocInternalRegisterByteTest() override = default;
};

TEST_F(IocInternalRegisterByteTest, WhenTheControlRegisterIsWrittenToItDrivesTheI2CBusAndSetsTheFiqBitsAppropriately) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    WriteByteInternalRegister(ADDRESS_CONTROL, 0xE3u);
    EXPECT_EQ(0x98u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
}

TEST_F(IocInternalRegisterByteTest, WhenTheControlRegisterIsWrittenToWithBits6And7ClearThenThoseBitsArePreserved) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_CALL(mediator, ReadDataI2C())
        .WillOnce(testing::Return(1u));
    WriteByteInternalRegister(ADDRESS_CONTROL, 0x3Fu);
    EXPECT_EQ(0xFFu, ReadByteInternalRegister(ADDRESS_CONTROL));
}

TEST_F(IocInternalRegisterByteTest, WhenTheControlRegisterIsReadFromTheLeastSignificantBitIsTakenFromTheI2CBus) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_CALL(mediator, ReadDataI2C())
        .WillOnce(testing::Return(1u))
        .WillOnce(testing::Return(0u));
    WriteByteInternalRegister(ADDRESS_CONTROL, 0xFFu);
    EXPECT_EQ(0xFFu, ReadByteInternalRegister(ADDRESS_CONTROL));
    EXPECT_EQ(0xFEu, ReadByteInternalRegister(ADDRESS_CONTROL));
}

TEST_F(IocInternalRegisterByteTest, WhenTheIrqMaskRegistersAreWrittenToTheyAreSetAppropriately) {
    WriteByteInternalRegister(ADDRESS_IRQ_A_MASK, 0x5Au);
    WriteByteInternalRegister(ADDRESS_IRQ_B_MASK, 0xA5u);
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0x61u);
    EXPECT_EQ(0x5Au, ReadByteInternalRegister(ADDRESS_IRQ_A_MASK));
    EXPECT_EQ(0xA5u, ReadByteInternalRegister(ADDRESS_IRQ_B_MASK));
    EXPECT_EQ(0x61u, ReadByteInternalRegister(ADDRESS_FIQ_MASK));
}

TEST_F(IocInternalRegisterByteTest, IrqARegistersWorkAsExpected) {
    WriteByteInternalRegister(ADDRESS_IRQ_A_CLEAR, 0xFFu);
    WriteByteInternalRegister(ADDRESS_IRQ_A_MASK, 0xFFu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_IRQ_A_REQUEST));
    StartFlyBack();
    EXPECT_EQ(0x88u, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x88u, ReadByteInternalRegister(ADDRESS_IRQ_A_REQUEST));
    WriteByteInternalRegister(ADDRESS_IRQ_A_MASK, 0x0Fu);
    EXPECT_EQ(0x88u, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x08u, ReadByteInternalRegister(ADDRESS_IRQ_A_REQUEST));
    WriteByteInternalRegister(ADDRESS_IRQ_A_CLEAR, 0x88u); // Check the Force IRQ bit is preserved
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_A_REQUEST));
}

TEST_F(IocInternalRegisterByteTest, IrqBRegistersWorkAsExpected) {
    WriteByteInternalRegister(ADDRESS_IRQ_B_MASK, 0xFFu);
    WriteByteInternalRegister(ADDRESS_SERIAL_TRANSMIT, 0xFFu);
    ReadByteInternalRegister(ADDRESS_SERIAL_RECEIVE);
    EndSoundInterrupt();
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_REQUEST));
    StartSoundInterrupt();
    EXPECT_EQ(0x02u, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x02u, ReadByteInternalRegister(ADDRESS_IRQ_B_REQUEST));
    WriteByteInternalRegister(ADDRESS_IRQ_B_MASK, 0xF0u);
    EXPECT_EQ(0x02u, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_REQUEST));
    EndSoundInterrupt();
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_IRQ_B_REQUEST));
}

TEST_F(IocInternalRegisterByteTest, FiqRegistersWorkAsExpected) {
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0xFFu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_REQUEST));
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0x0Fu);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x00u, ReadByteInternalRegister(ADDRESS_FIQ_REQUEST));
    WriteByteInternalRegister(ADDRESS_FIQ_MASK, 0xF0u);
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x80u, ReadByteInternalRegister(ADDRESS_FIQ_REQUEST));
}

TEST_F(IocInternalRegisterByteTest, WhenTheTimer0RegistersAreWrittenToTheyProgramTimer0Correctly) {
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_LOW, 0x39u);
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_HIGH, 0x81u);
    WriteByteInternalRegister(ADDRESS_TIMER_0_GO_COMMAND, 0u);
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x39u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_LOW, 0x88u);
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_HIGH, 0x89u);
    EXPECT_EQ(0x39u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_0_GO_COMMAND, 0u);
    EXPECT_EQ(0x39u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_0_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x88u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x89u, ReadByteInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
}

TEST_F(IocInternalRegisterByteTest, WhenTheTimer1RegistersAreWrittenToTheyProgramTimer1Correctly) {
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_LOW, 0x93u);
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_HIGH, 0x18u);
    WriteByteInternalRegister(ADDRESS_TIMER_1_GO_COMMAND, 0u);
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x93u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_LOW, 0x99u);
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_HIGH, 0x77u);
    EXPECT_EQ(0x93u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_1_GO_COMMAND, 0u);
    EXPECT_EQ(0x93u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_1_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x99u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadByteInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
}

TEST_F(IocInternalRegisterByteTest, WhenTheTimer2RegistersAreWrittenToTheyProgramTimer2Correctly) {
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_LOW, 0x11u);
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_HIGH, 0x77u);
    WriteByteInternalRegister(ADDRESS_TIMER_2_GO_COMMAND, 0u);
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x11u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_LOW, 0xCEu);
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_HIGH, 0xFAu);
    EXPECT_EQ(0x11u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_2_GO_COMMAND, 0u);
    EXPECT_EQ(0x11u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_2_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xCEu, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0xFAu, ReadByteInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
}

TEST_F(IocInternalRegisterByteTest, WhenTheTimer3RegistersAreWrittenToTheyProgramTimer3Correctly) {
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_LOW, 0xABu);
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_HIGH, 0xDCu);
    WriteByteInternalRegister(ADDRESS_TIMER_3_GO_COMMAND, 0u);
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xABu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_LOW, 0xECu);
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_HIGH, 0xAFu);
    EXPECT_EQ(0xABu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_3_GO_COMMAND, 0u);
    EXPECT_EQ(0xABu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteByteInternalRegister(ADDRESS_TIMER_3_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xECu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xAFu, ReadByteInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
}