#include "Archimedes/IocTest.h"

#include "Common/Util/BitUtil.h"

constexpr auto EncodeIocAddress(const IocAddress& address) -> uint32_t {
    return EncodeBitField(address.address, 0u, 7u) +
           EncodeBitField(address.bank, 16u, 3u) +
           EncodeBitField(address.cycle, 19u, 2u) +
           EncodeBitField(1u, 21u, 1u);
}

constexpr auto EncodeIocInternalRegisterAddress(const uint32_t address) -> uint32_t {
    return EncodeBitField(address, 0u, 7u) +
           EncodeBitField(1u, 21u, 1u);
}

auto IocTest::StartFlyBack() -> void { ioc.StartFlyBack(); }
auto IocTest::EndFlyBack() -> void { ioc.EndFlyBack(); }

auto IocTest::StartSoundInterrupt() -> void { ioc.StartSoundInterrupt(); }
auto IocTest::EndSoundInterrupt() -> void { ioc.EndSoundInterrupt(); }

auto IocTest::StartFloppyDiscDataRequest() -> void { ioc.StartFloppyDiscDataRequest(); }
auto IocTest::StartFloppyDiscInterruptRequest() -> void { ioc.StartFloppyDiscInterruptRequest(); }

auto IocTest::EndFloppyDiscDataRequest() -> void { ioc.EndFloppyDiscDataRequest(); }
auto IocTest::EndFloppyDiscInterruptRequest() -> void { ioc.EndFloppyDiscInterruptRequest(); }

auto IocTest::ReadWordInternalRegister(uint32_t address) -> uint32_t {
    return ioc.Read(EncodeIocInternalRegisterAddress(address));
}
auto IocTest::ReadByteInternalRegister(uint32_t address) -> uint32_t {
    return ioc.Read(EncodeIocInternalRegisterAddress(address));
}
auto IocTest::ReadWord(const IocAddress& address) -> uint32_t {
    return ioc.Read(EncodeIocAddress(address));
}
auto IocTest::ReadByte(const IocAddress& address) -> uint32_t {
    return ioc.Read(EncodeIocAddress(address));
}
auto IocTest::WriteWordInternalRegister(uint32_t address, uint32_t v) -> void {
    ioc.Write(EncodeIocInternalRegisterAddress(address), ReplicateHalfWordInWord(v));
}
auto IocTest::WriteByteInternalRegister(uint32_t address, uint32_t v) -> void {
    ioc.Write(EncodeIocInternalRegisterAddress(address), ReplicateByteInWord(v));
}
auto IocTest::WriteWord(const IocAddress& address, uint32_t v) -> void {
    ioc.Write(EncodeIocAddress(address), ReplicateHalfWordInWord(v));
}
auto IocTest::WriteByte(const IocAddress& address, uint32_t v) -> void {
    ioc.Write(EncodeIocAddress(address), ReplicateByteInWord(v));
}