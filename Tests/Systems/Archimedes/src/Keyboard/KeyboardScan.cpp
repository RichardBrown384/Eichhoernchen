#include "Archimedes/KeyboardTest.h"

class KeyboardScanTest : public KeyboardTest {
public:
    ~KeyboardScanTest() override = default;
    void SetUp() override {
        uint8_t data;
        WriteReset();
        Read(data);
        WriteResetAcknowledge1();
        Read(data);
        WriteResetAcknowledge2();
        Read(data);
        WriteKeyboardScanAcknowledge();
    }
};

TEST_F(KeyboardScanTest, WhenAResetRequestIsSentTheResetProcessIsRestarted) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAByteAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenABothScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAMouseScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyboardScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenANoScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenDataIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestData(0x03u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0xE3u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenMouseDataIsRequestedItIsReturned) {
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

TEST_F(KeyboardScanTest, WhenANoOpIsRequestedItProducesNothing) {
    uint8_t data;
    WriteNoOp();
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenTheKeyboardIdIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestKeyboardId();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_ID_UK, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenALedIsRequestedItProducesNothing) {
    uint8_t data;
    WriteLed(0u);
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenKeysAreDepressedTheyAreTransmitted) {
    uint8_t data;
    KeyDown(0x11u);
    KeyDown(0x19u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x19u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(0x19u), data);
    WriteBothScanAcknowledge();
}

TEST_F(KeyboardScanTest, WhenAKeyIsDepressedAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsDepressedAndAMouseAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsDepressedAndAKeyboardAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsDepressedAndABothAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenKeysAreReleasedTheyAreTransmitted) {
    uint8_t data;
    KeyUp(0x11u);
    KeyUp(0x19u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpSecond(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x19u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpSecond(0x19u), data);
    WriteBothScanAcknowledge();
}

TEST_F(KeyboardScanTest, WhenAKeyIsReleasedAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsReleasedAndAMouseAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsReleasedAndAKeyboardAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenAKeyIsReleasedAndABothAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(KeyboardScanTest, WhenTheMouseMovesTheyAreNotTransmitted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenMouseButtonsAreDepressedTheyAreTransmitted) {
    uint8_t data;
    MouseButtonDown(MOUSE_BUTTON_LEFT);
    MouseButtonDown(MOUSE_BUTTON_MIDDLE);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(MOUSE_BUTTON_LEFT), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(MOUSE_BUTTON_LEFT), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(MOUSE_BUTTON_MIDDLE), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(MOUSE_BUTTON_MIDDLE), data);
    WriteBothScanAcknowledge();
}

TEST_F(KeyboardScanTest, WhenMouseButtonsAreReleasedTheyAreTransmitted) {
    uint8_t data;
    MouseButtonUp(MOUSE_BUTTON_MIDDLE);
    MouseButtonUp(MOUSE_BUTTON_RIGHT);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(MOUSE_BUTTON_MIDDLE), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpSecond(MOUSE_BUTTON_MIDDLE), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(MOUSE_BUTTON_RIGHT), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpSecond(MOUSE_BUTTON_RIGHT), data);
    WriteBothScanAcknowledge();
}

TEST_F(KeyboardScanTest, WhenKeyboardScanningIsDisabledDepressedKeysAreNotTransmitted) {
    uint8_t data;
    KeyDown(0x13u);
    KeyDown(0x12u);
    KeyDown(0x15u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x13u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownSecond(0x13u), data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenKeyboardScanningIsDisabledReleasedKeysAreNotTransmitted) {
    uint8_t data;
    KeyUp(0x13u);
    KeyUp(0x14u);
    KeyUp(0x15u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x13u), data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpSecond(0x13u), data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(KeyboardScanTest, WhenMouseScanningIsReEnabledMouseMotionsAreNotTransmitted) {
    uint8_t data;
    MouseMotion(11u, 19u);
    MouseMotion(12u, 20u);
    MouseMotion(13u, 21u);
    WriteRequestMouseData();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0, data);
    WriteMouseScanAcknowledge();
    EXPECT_FALSE(Read(data));
}