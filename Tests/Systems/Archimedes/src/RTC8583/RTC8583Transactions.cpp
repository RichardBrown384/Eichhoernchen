#include "Archimedes/RTC8583Test.h"

class RTC8583Transactions : public RTC8583Test {
public:
    ~RTC8583Transactions() override = default;
    void SetUp() override {
        Stop();
    }
};

TEST_F(RTC8583Transactions, WhenIdleReceivingTheWriteTargetAddressInitiatesATransaction) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
}

TEST_F(RTC8583Transactions, WhenIdleReceivingTheReadTargetAddressInitiatesATransaction) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
}

TEST_F(RTC8583Transactions, WhenIdleReceivingAnInvalidAddressDoesNotInitiateATransaction) {
    EXPECT_FALSE(ReceiveTargetAddress(0x11));
}

TEST_F(RTC8583Transactions, WhenNotIdleReceivingTheWriteTargetAddressStopsATransaction) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_FALSE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
}

TEST_F(RTC8583Transactions, ReceivingDataDuringAReadTransactionStopsATransaction) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
    EXPECT_FALSE(ReceiveData(0u));
}

TEST_F(RTC8583Transactions, StartTransmitDuringAWriteTransactionStopsATransaction) {
    uint8_t discard;
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_FALSE(StartTransmit(discard));
}

TEST_F(RTC8583Transactions, EndTransmitDuringAWriteTransactionStopsATransaction) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_FALSE(EndTransmit());
}

TEST_F(RTC8583Transactions, StartTransmitTwiceInSuccessionDuringAReadTransactionStopsATransaction) {
    uint8_t discard;
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
    EXPECT_TRUE(StartTransmit(discard));
    EXPECT_FALSE(StartTransmit(discard));
}

TEST_F(RTC8583Transactions, EndTransmitTwiceInSuccessionDuringAReadTransactionStopsATransaction) {
    uint8_t discard;
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
    EXPECT_TRUE(StartTransmit(discard));
    EXPECT_TRUE(EndTransmit());
    EXPECT_FALSE(EndTransmit());
}

TEST_F(RTC8583Transactions, AWriteTransactionCompletesSuccessfully) {
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_TRUE(ReceiveData(0x23u));
    EXPECT_TRUE(ReceiveData(3u));
    Stop();
    EXPECT_EQ(0x24u, ReadAddress());
    EXPECT_EQ(3u, ReadRam(0x23u));
}

TEST_F(RTC8583Transactions, AReadTransactionCompletesSuccessfully) {
    uint8_t v;
    WriteRam(0x23u, 19u);
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_TRUE(ReceiveData(0x23u));
    Stop();
    EXPECT_EQ(0x23u, ReadAddress());
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
    EXPECT_TRUE(StartTransmit(v));
    EXPECT_EQ(0x23u, ReadAddress());
    EXPECT_TRUE(EndTransmit());
    EXPECT_EQ(0x24u, ReadAddress());
    Stop();
    EXPECT_EQ(19u, v);
}

TEST_F(RTC8583Transactions, DataWrittenInAWriteTransactionCanBeReadInAReadTransaction) {
    uint8_t v1, v2, v3;
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_TRUE(ReceiveData(0x22u));
    EXPECT_TRUE(ReceiveData(9u));
    EXPECT_TRUE(ReceiveData(72u));
    EXPECT_TRUE(ReceiveData(11u));
    Stop();
    EXPECT_EQ(0x25, ReadAddress());
    EXPECT_TRUE(ReceiveTargetAddress(RTC_WRITE_ADDRESS));
    EXPECT_TRUE(ReceiveData(0x22u));
    Stop();
    EXPECT_EQ(0x22, ReadAddress());
    EXPECT_TRUE(ReceiveTargetAddress(RTC_READ_ADDRESS));
    EXPECT_TRUE(StartTransmit(v1));
    EXPECT_TRUE(EndTransmit());
    EXPECT_TRUE(StartTransmit(v2));
    EXPECT_TRUE(EndTransmit());
    EXPECT_TRUE(StartTransmit(v3));
    EXPECT_TRUE(EndTransmit());
    EXPECT_EQ(0x25, ReadAddress());
    Stop();
    EXPECT_EQ(9u, v1);
    EXPECT_EQ(72u, v2);
    EXPECT_EQ(11u, v3);
}