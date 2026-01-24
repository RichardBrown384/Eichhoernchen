#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

using namespace rbrown::arm;

auto Assembler::AssembleRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvance('R')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            r = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleWhitespaceRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        return AssembleRegisterNumber(source, r);
    }
    return false;
}

auto Assembler::AssembleWhitespaceCommaRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        return AssembleRegisterNumber(source, r);
    }
    return false;
}

auto Assembler::AssembleShiftAmountNumber(SourceLine& source, uint32_t& shiftAmount) -> bool {
    if (source.MatchAndAdvance('#')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            shiftAmount = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleImmediateNumber(SourceLine& source, uint32_t& immediate) -> bool {
    if (source.MatchAndAdvance('#')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            immediate = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleTransferOffsetNumber(SourceLine& source, uint32_t& u, uint32_t& offset) -> bool {
    if (source.MatchAndAdvance('#')) {
        uint32_t sign = 1u;
        if (source.MatchAndAdvance('-')) {
            sign = 0u;
        }
        if (unsigned long matched; source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            u = sign;
            offset = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleBranchOffsetNumber(SourceLine& source, uint32_t& offset) -> bool {
    if (source.MatchAndAdvance('#')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            offset = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorNumber(SourceLine& source, uint32_t& n) -> bool {
    if (source.MatchAndAdvance('p')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            n = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvance('c')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            r = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorOpcodeNumber(SourceLine& source, uint32_t& op) -> bool {
    if (source.MatchAndAdvance('#')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            op = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorInformationNumber(SourceLine& source, uint32_t& cp) -> bool {
    if (source.MatchAndAdvance('#')) {
        if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            cp = static_cast<uint32_t>(matched);
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleSoftwareInterruptCommentNumber(SourceLine& source, uint32_t& comment) -> bool {
    if (unsigned long matched; source.MatchAndAdvanceNumber(10, matched)) {
        // TODO range checks
        comment = static_cast<uint32_t>(matched);
        return true;
    }
    return false;
}