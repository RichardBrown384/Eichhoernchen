#include "Assembler/SourceLine.h"

using namespace rbrown::arm;

SourceLine::SourceLine(const std::string& line_) :
    line{ line_ }, len{ line.size() }, pos{ 0u } {}

auto SourceLine::GetCurrentCharacter(char& c) const -> bool {
    if (pos < len) {
        c = line[pos];
        return true;
    }
    return false;
}

auto SourceLine::Advance() -> void {
    pos = pos + 1u;
}

auto SourceLine::Match(char c) const -> bool {
    char m;
    if (GetCurrentCharacter(m)) {
        return m == c;
    }
    return false;
}

auto SourceLine::MatchWhitespace() const -> bool {
    return Match(' ');
}

auto SourceLine::MatchAndAdvance(char c) -> bool {
    if (Match(c)) {
        Advance();
        return true;
    }
    return false;
}

auto SourceLine::MatchAndAdvanceWhitespace() -> bool {
    return MatchAndAdvance(' ');
}

auto SourceLine::MatchAndAdvanceCommaWhitespace() -> bool {
    if (MatchAndAdvance(',')) {
        return MatchAndAdvanceWhitespace();
    }
    return false;
}

auto SourceLine::MatchAndAdvanceNumber(int base, unsigned long& result) -> bool {
    if (pos < len) {
        std::string sub = line.substr(pos, len - pos);
        size_t idx;
        result = std::stoul(sub, &idx, base);
        pos += idx;
        return true;
    }
    return false;
}