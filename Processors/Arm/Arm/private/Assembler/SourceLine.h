#pragma once

#include <cstdint>
#include <string>

namespace rbrown::arm {

class SourceLine {
public:
    explicit SourceLine(const std::string&);
public:
    [[nodiscard]] auto Match(char) const -> bool;
    [[nodiscard]] auto MatchWhitespace() const -> bool;
    auto MatchAndAdvance(char) -> bool;
    auto MatchAndAdvanceWhitespace() -> bool;
    auto MatchAndAdvanceCommaWhitespace() -> bool;
    auto MatchAndAdvanceNumber(int, unsigned long&) -> bool;
private:
    auto GetCurrentCharacter(char&) const -> bool;
    auto Advance() -> void;
    const std::string& line;
    const size_t len;
    size_t pos;
};
}