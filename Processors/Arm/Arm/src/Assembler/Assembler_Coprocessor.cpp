#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Encode.h"

using namespace rbrown::arm;

auto Assembler::AssembleCoprocessorCoprocessorArgument(SourceLine& source, uint32_t& n) -> bool {
    if (source.MatchAndAdvanceWhitespace()) {
        return AssembleCoprocessorNumber(source, n);
    }
    return false;
}

auto Assembler::AssembleCoprocessorOpcodeArgument(SourceLine& source, uint32_t& op) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        return AssembleCoprocessorOpcodeNumber(source, op);
    }
    return false;
}

auto Assembler::AssembleCoprocessorProcessorRegisterArgument(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        return AssembleRegisterNumber(source, r);
    }
    return false;
}

auto Assembler::AssembleCoprocessorCoprocessorRegisterArgument(SourceLine& source, uint32_t& r) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        return AssembleCoprocessorRegisterNumber(source, r);
    }
    return false;
}

auto Assembler::AssembleCoprocessorInformationArgument(SourceLine& source, uint32_t& cp) -> bool {
    if (source.MatchAndAdvanceCommaWhitespace()) {
        return AssembleCoprocessorInformationNumber(source, cp);
    }
    return true;
}
