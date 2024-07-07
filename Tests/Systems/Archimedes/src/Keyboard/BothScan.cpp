#include "Archimedes/KeyboardTest.h"

class BothScanTest : public KeyboardTest {
public:
    ~BothScanTest() override = default;
    void SetUp() override {
        uint8_t data;
        WriteReset();
        Read(data);
        WriteResetAcknowledge1();
        Read(data);
        WriteResetAcknowledge2();
        Read(data);
        WriteBothScanAcknowledge();
    }
};

TEST_F(BothScanTest, WhenAResetRequestIsSentTheResetProcessIsRestarted) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAByteAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenABothScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAMouseScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyboardScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenANoScanAcknowledgeIsSentWhenNoneIsExpectedTheResetProcessIsRestarted) {
    uint8_t data;
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenDataIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestData(0x03u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(0xE3u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(BothScanTest, WhenMouseDataIsRequestedItIsReturned) {
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

TEST_F(BothScanTest, WhenANoOpIsRequestedItProducesNothing) {
    uint8_t data;
    WriteNoOp();
    EXPECT_FALSE(Read(data));
}

TEST_F(BothScanTest, WhenTheKeyboardIdIsRequestedItIsReturned) {
    uint8_t data;
    WriteRequestKeyboardId();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_ID_UK, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(BothScanTest, WhenALedIsRequestedItProducesNothing) {
    uint8_t data;
    WriteLed(0u);
    EXPECT_FALSE(Read(data));
}

TEST_F(BothScanTest, WhenKeysAreDepressedTheyAreTransmitted) {
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

TEST_F(BothScanTest, WhenAKeyIsDepressedAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsDepressedAndAMouseAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsDepressedAndAKeyboardAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsDepressedAndABothAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyDown(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyDownFirst(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenKeysAreReleasedTheyAreTransmitted) {
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

TEST_F(BothScanTest, WhenAKeyIsReleasedAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsReleasedAndAMouseAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsReleasedAndAKeyboardAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenAKeyIsReleasedAndABothAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    KeyUp(0x11u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KeyUpFirst(0x11u), data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenTheMouseMovesTheyAreTransmitted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(16u, data);
    WriteMouseScanAcknowledge();
}

TEST_F(BothScanTest, WhenTheMouseMovesAndANoScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteNoScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenTheMouseMovesAndAMouseScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteMouseScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenTheMouseMovesAndAKeyboardScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteKeyboardScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenTheMouseMovesAndABothScanAcknowledgeIsReceivedAfterTheFirstByteTheResetProcessIsRestarted) {
    uint8_t data;
    MouseMotion(12u, 16u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(12u, data);
    WriteBothScanAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(BothScanTest, WhenMouseButtonsAreDepressedTheyAreTransmitted) {
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

TEST_F(BothScanTest, WhenMouseButtonsAreReleasedTheyAreTransmitted) {
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

TEST_F(BothScanTest, WhenKeyboardScanningIsDisabledDepressedKeysAreNotTransmitted) {
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

TEST_F(BothScanTest, WhenKeyboardScanningIsDisabledReleasedKeysAreNotTransmitted) {
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

TEST_F(BothScanTest, WhenMouseScanningIsDisabledMouseMotionsAreNotTransmitted) {
    uint8_t data;
    MouseMotion(11u, 19u);
    MouseMotion(12u, 20u);
    MouseMotion(13u, 21u);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(11u, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(19u, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}