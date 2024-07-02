#include "Archimedes/ArchimedesVidc.h"
#include "Archimedes/ArchimedesMediator.h"

namespace rbrown::acorn::archimedes {

VideoFifo::VideoFifo(ArchimedesMediator& m) :
    buffer{},
    readPos{},
    writePos{},
    dmaRequest{ 3u },
    bits{ 1u },
    mask{ 1u },
    mediator{ m } {}

auto VideoFifo::Reset() -> void {
    readPos = 0u;
    writePos = 0u;
    mediator.StartVideoDmaRequest();
    mediator.StartVideoDmaRequest();
}

auto VideoFifo::SetBitsPerPixel(uint32_t bpp) -> void {
    writePos = 0u;
    readPos = 0u;
    bits = (1u << bpp);
    mask = (1u << bits) - 1u;
}

auto VideoFifo::SetDmaRequest(uint32_t v) -> void {
    dmaRequest = v;
}

auto VideoFifo::WriteWord(uint32_t v) -> void {
    buffer[writePos] = v;
    writePos += 1u;
    writePos &= buffer.size() - 1u;
}

auto VideoFifo::ReadNext() -> uint32_t {
    const auto word = readPos >> 5u;
    const auto bit = readPos & 0x1Fu;
    const auto pixel = (buffer[word] >> bit) & mask;
    // VTI -VDRQ can be set low can be set low half-way through reading
    // the last byte of any word
    if ((word & 3u) == dmaRequest &&
        ((bits == 8u && bit == 24u) || bit == 28u)) {
        mediator.StartVideoDmaRequest();
    }
    readPos += bits;
    readPos &= 0xFFu;
    return pixel;
}

CursorFifo::CursorFifo(ArchimedesMediator& m) :
    buffer{},
    readPos{},
    writePos{},
    mediator{ m } {}

auto CursorFifo::Reset() -> void {
    readPos = 0u;
    writePos = 0u;
    mediator.StartCursorDmaRequest();
}

auto CursorFifo::WriteWord(uint32_t v) -> void {
    buffer[writePos] = v;
    writePos += 1u;
    writePos &= buffer.size() - 1u;
}

auto CursorFifo::ReadNext() -> uint32_t {
    const auto word = readPos >> 5u;
    const auto bit = readPos & 0x1Fu;
    const auto pixel = (buffer[word] >> bit) & CursorFifo::MASK;
    readPos += CursorFifo::BITS;
    readPos &= 0x7Fu;
    return pixel;
}

}