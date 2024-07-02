#include "Archimedes/MemcTest.h"

class FlyBackTest: public MemcTest {
public:
    FlyBackTest() : MemcTest(0u) {}
    ~FlyBackTest() override = default;
};

TEST_F(FlyBackTest, EndingFlyBackResetsVideoAndCursorPointersAppropriately) {
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x12u
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0x18u
    }));
    EXPECT_NE(0x12u, GetVideoPointer());
    EXPECT_NE(0x18u, GetCursorPointer());
    EndFlyBack();
    EXPECT_EQ(0x12u, GetVideoPointer());
    EXPECT_EQ(0x18u, GetCursorPointer());
}