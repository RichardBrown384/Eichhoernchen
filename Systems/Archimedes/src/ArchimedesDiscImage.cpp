#include "Archimedes/ArchimedesDiscImage.h"
#include "Archimedes/WDC1772.h"

#include <array>
#include <utility>

namespace rbrown::acorn::archimedes {

enum class SectorSize : char {
    Size128 = 0,
    Size256,
    Size512,
    Size1024
};

constexpr auto GetSectorSizeBytes(SectorSize size) -> uint32_t {
    return LogicalShiftLeft(1u, 7u + std::to_underlying(size));
}

struct DiscImageFormat {
public:
    constexpr DiscImageFormat(uint8_t sides, uint8_t tracks, uint8_t sectors, SectorSize sectorSize) :
        m_sides{ sides },
        m_tracks{ tracks },
        m_sectors{ sectors },
        m_sectorSize{ sectorSize } {};
    uint8_t m_sides;
    uint8_t m_tracks;
    uint8_t m_sectors;
    SectorSize m_sectorSize;
};

constexpr auto GetImageSizeBytes(const DiscImageFormat& format) {
    const auto& [sides, tracks, sectors, sectorSize] = format;
    return sides * tracks * sectors * GetSectorSizeBytes(sectorSize);
}

constexpr DiscImageFormat FORMAT_ADFS_800_KB = DiscImageFormat(2u, 80u, 5u, SectorSize::Size1024);

constexpr std::array FORMATS = {
    FORMAT_ADFS_800_KB
};

constexpr auto DOUBLE_DENSITY_SECTOR_GAP_1_BYTES = 60u;
constexpr auto DOUBLE_DENSITY_SECTOR_GAP_2_BYTES = 12u + 3u;
constexpr auto DOUBLE_DENSITY_SECTOR_IDAM_BYTES = 1u;
constexpr auto DOUBLE_DENSITY_SECTOR_IDRECORD_BYTES = 6u;
constexpr auto DOUBLE_DENSITY_SECTOR_GAP_3_BYTES = 22u + 12u + 3u;
constexpr auto DOUBLE_DENSITY_SECTOR_DAM_BYTES = 1u;
constexpr auto DOUBLE_DENSITY_SECTOR_DCRC_BYTES = 2u;
constexpr auto DOUBLE_DENSITY_SECTOR_GAP_4_BYTES = 40u;
constexpr auto DOUBLE_DENSITY_SECTOR_GAP_5_BYTES = 668u;

constexpr auto DOUBLE_DENSITY_SECTOR_NON_USER_DATA_BYTES =
    DOUBLE_DENSITY_SECTOR_GAP_1_BYTES +
    DOUBLE_DENSITY_SECTOR_GAP_2_BYTES +
    DOUBLE_DENSITY_SECTOR_IDAM_BYTES +
    DOUBLE_DENSITY_SECTOR_IDRECORD_BYTES +
    DOUBLE_DENSITY_SECTOR_GAP_3_BYTES +
    DOUBLE_DENSITY_SECTOR_DAM_BYTES +
    DOUBLE_DENSITY_SECTOR_DCRC_BYTES +
    DOUBLE_DENSITY_SECTOR_GAP_4_BYTES +
    DOUBLE_DENSITY_SECTOR_GAP_5_BYTES;

constexpr auto ADDRESS_RECORD_OFFSET_TRACK_NUMBER = 0u;
constexpr auto ADDRESS_RECORD_OFFSET_SIDE_NUMBER = 1u;
constexpr auto ADDRESS_RECORD_OFFSET_SECTOR_NUMBER = 2u;
constexpr auto ADDRESS_RECORD_OFFSET_SECTOR_SIZE = 3u;
constexpr auto ADDRESS_RECORD_OFFSET_CRC_1 = 4u;
constexpr auto ADDRESS_RECORD_OFFSET_CRC_2 = 5u;
constexpr auto ADDRESS_RECORD_SIZE_BYTES = 6u;

class EmptyDiscImage final: public DiscImage {
public:
    ~EmptyDiscImage() final = default;
    [[nodiscard]] auto IsWriteProtected() const -> bool final { return true; }
    [[nodiscard]] auto GetTracks() const -> uint32_t final { return 0u; }
    [[nodiscard]] auto GetSectors() const -> uint32_t final { return 0u; }
    [[nodiscard]] auto GetUserSectorSizeBytes() const -> uint32_t final { return 0u; }
    [[nodiscard]] auto GetTotalSectorSizeBytes() const -> uint32_t final { return 0u; }
    [[nodiscard]] auto GetAddressSizeBytes() const -> uint32_t final { return 0u; }
    auto SeekToData(uint32_t, uint32_t, uint32_t) -> bool final { return false; }
    auto ReadByteData() -> uint8_t final { return 0u; }
    auto WriteByteData(uint8_t) -> void final {};
    auto SeekToAddress(uint32_t, uint32_t, uint32_t) -> bool final { return false; }
    auto ReadByteAddress() -> uint8_t final { return 0u; }
};

class ArchimedesDiscImage final: public DiscImage {
public:
    ArchimedesDiscImage(std::vector<uint8_t> data, const DiscImageFormat& format) :
        m_data{ std::move(data) },
        m_address{},
        m_format{ format },
        m_position{} {}
    ~ArchimedesDiscImage() final = default;
    [[nodiscard]] auto IsWriteProtected() const -> bool final { return false; }
    [[nodiscard]] auto GetTracks() const -> uint32_t final { return m_format.m_tracks; }
    [[nodiscard]] auto GetSectors() const -> uint32_t final { return m_format.m_sectors; }
    [[nodiscard]] auto GetUserSectorSizeBytes() const -> uint32_t final {
        return GetSectorSizeBytes(m_format.m_sectorSize);
    }
    [[nodiscard]] auto GetTotalSectorSizeBytes() const -> uint32_t final {
        return GetUserSectorSizeBytes() + DOUBLE_DENSITY_SECTOR_NON_USER_DATA_BYTES;
    }
    [[nodiscard]] auto GetAddressSizeBytes() const -> uint32_t final {
        return m_address.size();
    }
    auto SeekToData(uint32_t side, uint32_t track, uint32_t sector) -> bool final {
        const auto&[sides, tracks, sectors, sectorSize] = m_format;
        if (side < sides && track < tracks && sector < sectors) {
            const auto logicalTrack = track * sides + side;
            const auto logicalSector = logicalTrack * sectors + sector;
            m_position = logicalSector * GetSectorSizeBytes(sectorSize);
            return true;
        }
        return false;
    }
    auto ReadByteData() -> uint8_t final {
        return (m_position < m_data.size()) ? m_data[m_position++] : 0u;
    }
    auto WriteByteData(uint8_t v) -> void final {
        if (m_position < m_data.size()) {
            m_data[m_position++] = v;
        }
    }
    auto SeekToAddress(uint32_t side, uint32_t track, uint32_t sector) -> bool final {
        const auto&[sides, tracks, sectors, sectorSize] = m_format;
        if (side < sides && track < tracks && sector < sectors) {
            m_address[ADDRESS_RECORD_OFFSET_TRACK_NUMBER] = track;
            m_address[ADDRESS_RECORD_OFFSET_SIDE_NUMBER] = side;
            m_address[ADDRESS_RECORD_OFFSET_SECTOR_NUMBER] = sector;
            m_address[ADDRESS_RECORD_OFFSET_SECTOR_SIZE] = std::to_underlying(sectorSize);
            m_address[ADDRESS_RECORD_OFFSET_CRC_1] = 0x00u;
            m_address[ADDRESS_RECORD_OFFSET_CRC_2] = 0x00u;
            m_position = 0u;
            return true;
        }
        return false;
    }
    auto ReadByteAddress() -> uint8_t final {
        return (m_position < m_address.size()) ? m_address[m_position++] : 0u;
    }
private:
    std::vector<uint8_t> m_data;
    std::array<uint8_t, ADDRESS_RECORD_SIZE_BYTES> m_address;
    DiscImageFormat m_format;
    uint32_t m_position;
};

auto CreateDiscImage(std::vector<uint8_t> data) -> std::unique_ptr<DiscImage> {
    for (const auto& format : FORMATS) {
        if (data.size() == GetImageSizeBytes(format)) {
            return std::make_unique<ArchimedesDiscImage>(std::move(data), format);
        }
    }
    return std::make_unique<EmptyDiscImage>();
}
}