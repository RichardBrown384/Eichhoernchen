#pragma once

#include "Archimedes/ArchimedesMediator.h"

#include <gmock/gmock.h>

class ArchimedesMock : public rbrown::acorn::archimedes::ArchimedesMediator {
public:
    ~ArchimedesMock() override = default;

    MOCK_METHOD(void, FrameCompleted, (), (override));
    MOCK_METHOD(void, WritePixel, (uint32_t, uint32_t, uint32_t), (override));
    MOCK_METHOD(void, SetTextureSource, (uint32_t, uint32_t, uint32_t, uint32_t), (override));

    MOCK_METHOD(uint32_t, ReadWordIoc, (uint32_t), (override));
    MOCK_METHOD(void, WriteWordIoc, (uint32_t, uint32_t), (override));

    MOCK_METHOD(void, WriteWordVidcRegister, (uint32_t), (override));
    MOCK_METHOD(void, WriteWordVidcVideo, (uint32_t), (override));
    MOCK_METHOD(void, WriteWordVidcCursor, (uint32_t), (override));
    MOCK_METHOD(void, WriteWordVidcSound, (uint32_t), (override));

    MOCK_METHOD(bool, ReadByteKeyboard, (uint8_t&), (override));
    MOCK_METHOD(void, WriteByteKeyboard, (uint8_t), (override));

    MOCK_METHOD(bool, ReadDataI2C, (), (override));
    MOCK_METHOD(void, WriteClockDataI2C, (uint32_t), (override));

    MOCK_METHOD(uint8_t, ReadByteFloppyDiscController, (uint32_t), (override));
    MOCK_METHOD(void, WriteByteFloppyDiscController, (uint32_t, uint8_t), (override));

    MOCK_METHOD(void, WriteByteLatchA, (uint8_t), (override));
    MOCK_METHOD(void, WriteByteLatchB, (uint8_t), (override));

    MOCK_METHOD(void, Update, (uint32_t), (override));

    MOCK_METHOD(void, StartFlyBack, (), (override));
    MOCK_METHOD(void, EndFlyBack, (), (override));

    MOCK_METHOD(void, StartSoundInterrupt, (), (override));
    MOCK_METHOD(void, EndSoundInterrupt, (), (override));

    MOCK_METHOD(void, StartVideoDmaRequest, (), (override));
    MOCK_METHOD(void, StartCursorDmaRequest, (), (override));
    MOCK_METHOD(void, StartSoundDmaRequest, (), (override));

    MOCK_METHOD(void, EndVideoDmaRequest, (), (override));
    MOCK_METHOD(void, EndCursorDmaRequest, (), (override));
    MOCK_METHOD(void, EndSoundDmaRequest, (), (override));

    MOCK_METHOD(void, StartFloppyDiscDataRequest, (), (override));
    MOCK_METHOD(void, StartFloppyDiscInterruptRequest, (), (override));

    MOCK_METHOD(void, EndFloppyDiscDataRequest, (), (override));
    MOCK_METHOD(void, EndFloppyDiscInterruptRequest, (), (override));
};