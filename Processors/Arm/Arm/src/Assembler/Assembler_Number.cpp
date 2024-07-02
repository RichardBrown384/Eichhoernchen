#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Shifts.h"

using namespace rbrown::arm;

auto Assembler::AssembleRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('R')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            r = (uint32_t) matched;
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
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            shiftAmount = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleImmediateNumber(SourceLine& source, uint32_t& immediate) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            immediate = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleTransferOffsetNumber(SourceLine& source, uint32_t& u, uint32_t& offset) -> bool {
    uint32_t sign = 1u;
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvance('-')) {
            sign = 0u;
        }
        if (source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            u = sign;
            offset = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleBranchOffsetNumber(SourceLine& source, uint32_t& offset) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvanceNumber(0, matched)) {
            // TODO range checks
            offset = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorNumber(SourceLine& source, uint32_t& n) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('p')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            n = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorRegisterNumber(SourceLine& source, uint32_t& r) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('c')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            r = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorOpcodeNumber(SourceLine& source, uint32_t& op) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            op = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleCoprocessorInformationNumber(SourceLine& source, uint32_t& cp) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvance('#')) {
        if (source.MatchAndAdvanceNumber(10, matched)) {
            // TODO range checks
            cp = (uint32_t) matched;
            return true;
        }
    }
    return false;
}

auto Assembler::AssembleSoftwareInterruptCommentNumber(SourceLine& source, uint32_t& comment) -> bool {
    unsigned long matched;
    if (source.MatchAndAdvanceNumber(10, matched)) {
        // TODO range checks
        comment = (uint32_t) matched;
        return true;
    }
    return false;
}