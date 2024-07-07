#include "Archimedes/KeyboardTest.h"

class MouseScanTest : public KeyboardTest {
public:
    ~MouseScanTest() override = default;
    void SetUp() override {
        uint8_t data;
        WriteReset();
        Read(data);
        WriteResetAcknowledge1();
        Read(data);
        WriteResetAcknowledge2();
        Read(data);
        WriteMouseScanAcknowledge();
    }
};

TEST_F(MouseScanTest, WhenAResetRequestIsSentTheResetProcessIsRestarted) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenAByteAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenABothScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenAMouseScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenAKeyboardScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenANoScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenDataIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestData(0x03u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0xE3u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenMouseDataIsRequestedItIsReturned) {
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

TEST_F(MouseScanTest, WhenANoOpIsRequestedItProducesNothing) {
    uint8_t data;
    WriteNoOp();
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenTheKeyboardIdIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestKeyboardId();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_ID_UK, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenALedIsRequestedItProducesNothing) {
    uint8_t data;
    WriteLed(0u);
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenKeysAreDepressedTheyAreNotTransmitted) {
    uint8_t data;
    KeyDown(11u);
    KeyDown(19u);
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenKeysAreReleasedTheyAreNotTransmitted) {
    uint8_t data;
    KeyUp(11u);
    KeyUp(19u);
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenTheMouseMovesTheyAreTransmitted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(16u, data);
    WriteMouseScanAcknowledge();
}

TEST_F(MouseScanTest, WhenTheMouseMovesAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenTheMouseMovesAndAMouseScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenTheMouseMovesAndAKeyboardScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenTheMouseMovesAndABothScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(MouseScanTest, WhenMouseButtonsAreDepressedTheyAreNotTransmitted) {
    uint8_t data;
    MouseButtonDown(MOUSE_BUTTON_LEFT);
    MouseButtonDown(MOUSE_BUTTON_MIDDLE);
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenMouseButtonsAreReleasedTheyAreNotTransmitted) {
    uint8_t data;
    MouseButtonUp(MOUSE_BUTTON_MIDDLE);
    MouseButtonUp(MOUSE_BUTTON_RIGHT);
    EXPECT_FALSE(Read(data));
}

TEST_F(MouseScanTest, WhenBothScanningIsReEnabledDepressedKeysAreTransmitted) {
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

TEST_F(MouseScanTest, WhenKeyboardScanningIsReEnabledDepressedKeysAreTransmitted) {
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

TEST_F(MouseScanTest, WhenMouseScanningIsReEnabledDepressedKeysAreNotTransmitted) {
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

TEST_F(MouseScanTest, WhenMouseScanningIsDisabledMouseMovementsAreNotTransmitted) {
    uint8_t data;
    MouseMotion(11u, 15u);
    MouseMotion(13u, 16u);
    MouseMotion(14u, 17u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(11u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(15u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}