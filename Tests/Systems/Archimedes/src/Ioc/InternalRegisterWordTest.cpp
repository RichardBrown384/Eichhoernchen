#include "Archimedes/IocTest.h"

class IocInternalRegisterWordTest : public IocTest {
public:
    ~IocInternalRegisterWordTest() override = default;
};

TEST_F(IocInternalRegisterWordTest, WhenTheControlRegisterIsWrittenToItDrivesTheI2CBusAndSetsTheFiqBitsAppropriately) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    WriteWordInternalRegister(ADDRESS_CONTROL, 0x55E3u);
    EXPECT_EQ(0x98u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
}

TEST_F(IocInternalRegisterWordTest, WhenTheControlRegisterIsWrittenToWithBits6And7ClearThenThoseBitsArePreserved) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_CALL(mediator, ReadDataI2C())
        .WillOnce(testing::Return(1u));
    WriteWordInternalRegister(ADDRESS_CONTROL, 0x553Fu);
    EXPECT_EQ(0xFFu, ReadWordInternalRegister(ADDRESS_CONTROL));
}

TEST_F(IocInternalRegisterWordTest, WhenTheControlRegisterIsReadFromTheLeastSignificantBitIsTakenFromTheI2CBus) {
    EXPECT_CALL(mediator, WriteClockDataI2C(testing::_));
    EXPECT_CALL(mediator, ReadDataI2C())
        .WillOnce(testing::Return(1u))
        .WillOnce(testing::Return(0u));
    WriteWordInternalRegister(ADDRESS_CONTROL, 0x55FFu);
    EXPECT_EQ(0xFFu, ReadWordInternalRegister(ADDRESS_CONTROL));
    EXPECT_EQ(0xFEu, ReadWordInternalRegister(ADDRESS_CONTROL));
}

TEST_F(IocInternalRegisterWordTest, WhenTheIrqMaskRegistersAreWrittenToTheyAreSetAppropriately) {
    WriteWordInternalRegister(ADDRESS_IRQ_A_MASK, 0x6867u);
    WriteWordInternalRegister(ADDRESS_IRQ_B_MASK, 0xA55Au);
    WriteWordInternalRegister(ADDRESS_FIQ_MASK, 0x7890u);
    EXPECT_EQ(0x67u, ReadWordInternalRegister(ADDRESS_IRQ_A_MASK));
    EXPECT_EQ(0x5Au, ReadWordInternalRegister(ADDRESS_IRQ_B_MASK));
    EXPECT_EQ(0x90u, ReadWordInternalRegister(ADDRESS_FIQ_MASK));
}

TEST_F(IocInternalRegisterWordTest, IrqARegistersWorkAsExpected) {
    WriteWordInternalRegister(ADDRESS_IRQ_A_CLEAR, 0x11FFu);
    WriteWordInternalRegister(ADDRESS_IRQ_A_MASK, 0x33FFu);
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_IRQ_A_REQUEST));
    StartFlyBack();
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_IRQ_A_REQUEST));
    WriteWordInternalRegister(ADDRESS_IRQ_A_MASK, 0x110Fu);
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x08u, ReadWordInternalRegister(ADDRESS_IRQ_A_REQUEST));
    WriteWordInternalRegister(ADDRESS_IRQ_A_CLEAR, 0x3388u); // Check the Force IRQ bit is preserved
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_IRQ_A_STATUS));
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_A_REQUEST));
}

TEST_F(IocInternalRegisterWordTest, IrqBRegistersWorkAsExpected) {
    WriteWordInternalRegister(ADDRESS_IRQ_B_MASK, 0x55FFu);
    WriteWordInternalRegister(ADDRESS_SERIAL_TRANSMIT, 0x11FFu);
    ReadWordInternalRegister(ADDRESS_SERIAL_RECEIVE);
    EndSoundInterrupt();
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_REQUEST));
    StartSoundInterrupt();
    EXPECT_EQ(0x02u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x02u, ReadWordInternalRegister(ADDRESS_IRQ_B_REQUEST));
    WriteWordInternalRegister(ADDRESS_IRQ_B_MASK, 0x43F0u);
    EXPECT_EQ(0x02u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_REQUEST));
    EndSoundInterrupt();
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_STATUS));
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_IRQ_B_REQUEST));
}

TEST_F(IocInternalRegisterWordTest, FiqRegistersWorkAsExpected) {
    WriteWordInternalRegister(ADDRESS_FIQ_MASK, 0x91FFu);
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_REQUEST));
    WriteWordInternalRegister(ADDRESS_FIQ_MASK, 0x110Fu);
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x00u, ReadWordInternalRegister(ADDRESS_FIQ_REQUEST));
    WriteWordInternalRegister(ADDRESS_FIQ_MASK, 0x55F0u);
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_STATUS));
    EXPECT_EQ(0x80u, ReadWordInternalRegister(ADDRESS_FIQ_REQUEST));
}

TEST_F(IocInternalRegisterWordTest, WhenTheTimer0RegistersAreWrittenToTheyProgramTimer0Correctly) {
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_LOW, 0x7839u);
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_HIGH, 0x9981u);
    WriteWordInternalRegister(ADDRESS_TIMER_0_GO_COMMAND, 0u);
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x39u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_LOW, 0xAA88u);
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_HIGH, 0xBB89u);
    EXPECT_EQ(0x39u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_0_GO_COMMAND, 0u);
    EXPECT_EQ(0x39u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x81u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_0_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x88u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_LOW));
    EXPECT_EQ(0x89u, ReadWordInternalRegister(ADDRESS_TIMER_0_COUNT_HIGH));
}

TEST_F(IocInternalRegisterWordTest, WhenTheTimer1RegistersAreWrittenToTheyProgramTimer1Correctly) {
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_LOW, 0x1393u);
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_HIGH, 0x1218u);
    WriteWordInternalRegister(ADDRESS_TIMER_1_GO_COMMAND, 0u);
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x93u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_LOW, 0x2199u);
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_HIGH, 0x1277u);
    EXPECT_EQ(0x93u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_1_GO_COMMAND, 0u);
    EXPECT_EQ(0x93u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x18u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_1_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x99u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadWordInternalRegister(ADDRESS_TIMER_1_COUNT_HIGH));
}

TEST_F(IocInternalRegisterWordTest, WhenTheTimer2RegistersAreWrittenToTheyProgramTimer2Correctly) {
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_LOW, 0xAA11u);
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_HIGH, 0x5577u);
    WriteWordInternalRegister(ADDRESS_TIMER_2_GO_COMMAND, 0u);
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_COMMAND, 0u);
    EXPECT_EQ(0x11u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_LOW, 0x55CEu);
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_HIGH, 0xAAFAu);
    EXPECT_EQ(0x11u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_2_GO_COMMAND, 0u);
    EXPECT_EQ(0x11u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0x77u, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_2_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xCEu, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_LOW));
    EXPECT_EQ(0xFAu, ReadWordInternalRegister(ADDRESS_TIMER_2_COUNT_HIGH));
}

TEST_F(IocInternalRegisterWordTest, WhenTheTimer3RegistersAreWrittenToTheyProgramTimer3Correctly) {
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_LOW, 0x11ABu);
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_HIGH, 0x88DCu);
    WriteWordInternalRegister(ADDRESS_TIMER_3_GO_COMMAND, 0u);
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xABu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_LOW, 0x88ECu);
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_HIGH, 0x11AFu);
    EXPECT_EQ(0xABu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_3_GO_COMMAND, 0u);
    EXPECT_EQ(0xABu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xDCu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
    WriteWordInternalRegister(ADDRESS_TIMER_3_LATCH_COMMAND, 0u);
    EXPECT_EQ(0xECu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_LOW));
    EXPECT_EQ(0xAFu, ReadWordInternalRegister(ADDRESS_TIMER_3_COUNT_HIGH));
}