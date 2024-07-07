#include "Archimedes/KeyboardTest.h"

class NoScanTest : public KeyboardTest {
public:
    ~NoScanTest() override = default;
    void SetUp() override {
        uint8_t data;
        WriteReset();
        Read(data);
        WriteResetAcknowledge1();
        Read(data);
        WriteResetAcknowledge2();
        Read(data);
        WriteNoScanAcknowledge();
    }
};

TEST_F(NoScanTest, WhenAResetRequestIsSentTheResetProcessIsRestarted) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenAByteAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenABothScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenAMouseScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenAKeyboardScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenANoScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(NoScanTest, WhenDataIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestData(0x03u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0xE3u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenMouseDataIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    EXPECT_FALSE(Read(data));
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenANoOpIsRequestedItProducesNothing) {
    uint8_t data;
    WriteNoOp();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenTheKeyboardIdIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestKeyboardId();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_ID_UK, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenALedIsRequestedItProducesNothing) {
    uint8_t data;
    WriteLed(0u);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenKeysAreDepressedTheyAreNotTransmitted) {
    uint8_t data;
    KeyDown(11u);
    KeyDown(19u);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenKeysAreReleasedTheyAreNotTransmitted) {
    uint8_t data;
    KeyUp(11u);
    KeyUp(19u);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenTheMouseMovesTheyAreNotTransmitted) {
    uint8_t data;
    MouseMotion(12u, 12u);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenMouseButtonsAreDepressedTheyAreNotTransmitted) {
    uint8_t data;
    MouseButtonDown(MOUSE_BUTTON_LEFT);
    MouseButtonDown(MOUSE_BUTTON_MIDDLE);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenMouseButtonsAreReleasedTheyAreNotTransmitted) {
    uint8_t data;
    MouseButtonUp(MOUSE_BUTTON_MIDDLE);
    MouseButtonUp(MOUSE_BUTTON_RIGHT);
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenBothScanningIsReEnabledDepressedKeysAreTransmitted) {
    uint8_t data;
    KeyDown(0x11u);
    KeyDown(0x12u);
    KeyUp(0x11u);
    EXPECT_FALSE(Read(data));
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x12u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(0x12u), data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenKeyboardScanningIsReEnabledDepressedKeysAreTransmitted) {
    uint8_t data;
    KeyDown(0x11u);
    KeyDown(0x12u);
    KeyUp(0x11u);
    EXPECT_FALSE(Read(data));
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x12u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(0x12u), data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenMouseScanningIsReEnabledDepressedKeysAreNotTransmitted) {
    uint8_t data;
    KeyDown(0x11u);
    KeyDown(0x12u);
    KeyUp(0x11u);
    EXPECT_FALSE(Read(data));
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteMouseScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(NoScanTest, WhenMouseScanningIsReEnabledMouseMotionEventsAreNotTransmitted) {
    uint8_t data;
    MouseMotion(7u, 7u);
    EXPECT_FALSE(Read(data));
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteMouseScanAcknowledge();
    EXPECT_FALSE(Read(data));
}