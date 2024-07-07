#include "Archimedes/KeyboardTest.h"

class ResetProtocolTest : public KeyboardTest {
public:
    ~ResetProtocolTest() override = default;
};

TEST_F(ResetProtocolTest, TheResetSequenceConcludingWithABothScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteBothScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(ResetProtocolTest, TheResetSequenceConcludingWithAMouseScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteMouseScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(ResetProtocolTest, TheResetSequenceConcludingWithAKeyboardScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteKeyboardScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(ResetProtocolTest, TheResetSequenceConcludingWithANoScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteNoScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(ResetProtocolTest, TheResetSequenceConcludingWithAByteAcknowledgeDoesNotConcludesSuccessfully) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteByteAcknowledge();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(ResetProtocolTest, AnIncorrectResponseAfterReceiptOfTheResetCodeRestartsTheResetSequence) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    Write(KBD_CODE_INVALID);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(ResetProtocolTest, AnIncorrectResponseAfterReceiptOfTheResetAcknowledge1CodeRestartsTheResetSequence) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    Write(KBD_CODE_INVALID);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(ResetProtocolTest, AnIncorrectResponseAfterReceiptOfTheResetAcknowledge2CodeRestartsTheResetSequence) {
    uint8_t data;
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    Write(KBD_CODE_INVALID);
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
}

TEST_F(ResetProtocolTest, TheResetSequenceInterleavedWithLedCommandsConcludingWithABothScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteLed(0u);
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteLed(0u);
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteLed(0u);
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteLed(0u);
    WriteBothScanAcknowledge();
    EXPECT_FALSE(Read(data));
}

TEST_F(ResetProtocolTest, TheResetSequenceInterleavedWithNoOpCommandsConcludingWithABothScanAcknowledgeConcludesSuccessfully) {
    uint8_t data;
    WriteNoOp();
    WriteReset();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_HRST, data);
    WriteNoOp();
    WriteResetAcknowledge1();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK1, data);
    WriteNoOp();
    WriteResetAcknowledge2();
    EXPECT_TRUE(Read(data));
    EXPECT_EQ(KBD_CODE_RAK2, data);
    WriteNoOp();
    WriteBothScanAcknowledge();
    EXPECT_FALSE(Read(data));
}