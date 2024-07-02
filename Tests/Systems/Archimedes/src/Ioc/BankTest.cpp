#include "Archimedes/IocTest.h"

class BankTest : public IocTest {
public:
    ~BankTest() override = default;
};

TEST_F(BankTest, ReadingFromAnUnsupportedBankAddressesReturnsAllBitsSet) {
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 1u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 2u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 3u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 4u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 5u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 6u, .cycle = CYCLE_SLOW, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 7u, .cycle = CYCLE_SLOW, .address = 0u }));

    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 1u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 2u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 3u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 4u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 5u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 6u, .cycle = CYCLE_MEDIUM, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 7u, .cycle = CYCLE_MEDIUM, .address = 0u }));

    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 16u })); // 0 is mapped to Floppy Disc Controller
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 2u, .cycle = CYCLE_FAST, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 3u, .cycle = CYCLE_FAST, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 4u, .cycle = CYCLE_FAST, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 5u, .cycle = CYCLE_FAST, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 6u, .cycle = CYCLE_FAST, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 7u, .cycle = CYCLE_FAST, .address = 0u }));

    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 1u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 2u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 3u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 4u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 5u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 6u, .cycle = CYCLE_SYNC, .address = 0u }));
    EXPECT_EQ(0xFFFF'FFFFu, ReadByte({ .bank = 7u, .cycle = CYCLE_SYNC, .address = 0u }));
}

TEST_F(BankTest, Bank1WritesWhereAppropriateAreMappedToTheFloppyDiscController) {
    EXPECT_CALL(mediator, WriteByteFloppyDiscController(0u, 0xCCu));
    EXPECT_CALL(mediator, WriteByteFloppyDiscController(1u, 0xDDu));
    EXPECT_CALL(mediator, WriteByteFloppyDiscController(2u, 0xAAu));
    EXPECT_CALL(mediator, WriteByteFloppyDiscController(3u, 0x77u));
    WriteByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 0u }, 0xCCu);
    WriteByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 4u }, 0xDDu);
    WriteByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 8u }, 0xAAu);
    WriteByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 12u }, 0x77u);
    WriteByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 16u }, 0x11u);
}

TEST_F(BankTest, Bank1ReadsWhereAppropriateAreMappedToTheFloppyDiscController) {
    EXPECT_CALL(mediator, ReadByteFloppyDiscController(0u));
    EXPECT_CALL(mediator, ReadByteFloppyDiscController(1u));
    EXPECT_CALL(mediator, ReadByteFloppyDiscController(2u));
    EXPECT_CALL(mediator, ReadByteFloppyDiscController(3u));
    ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 0u });
    ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 4u });
    ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 8u });
    ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 12u });
    ReadByte({ .bank = 1u, .cycle = CYCLE_FAST, .address = 18u });
}

TEST_F(BankTest, Bank5WritesWhereAppropriateAreMappedToLatchesAAndB) {
    EXPECT_CALL(mediator, WriteByteLatchA(0xDCu));
    EXPECT_CALL(mediator, WriteByteLatchB(0xEFu));
    WriteByte({ .bank = 5u, .cycle = CYCLE_FAST, .address = 0x18u }, 0xEFu);
    WriteByte({ .bank = 5u, .cycle = CYCLE_FAST, .address = 0x40u }, 0xDCu);
}