#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

using namespace rbrown::arm;

auto Assembler::Assemble(SourceLine& source, uint32_t& result) -> bool {
    if (source.MatchAndAdvance('A')) {
        if (source.MatchAndAdvance('D')) {
            if (source.MatchAndAdvance('C')) {
                return AssembleAdc(source, result);
            } else {
                if (source.MatchAndAdvance('D')) {
                    return AssembleAdd(source, result);
                }
            }
        } else {
            if (source.MatchAndAdvance('N')) {
                if (source.MatchAndAdvance('D')) {
                    return AssembleAnd(source, result);
                }
            }
        }
    } else {
        if (source.MatchAndAdvance('B')) {
            if (source.MatchAndAdvance('I')) {
                if (source.MatchAndAdvance('C')) {
                    return AssembleBic(source, result);
                }
            } else {
                return AssembleB(source, result);
            }
        } else if (source.MatchAndAdvance('C')) {
            if (source.MatchAndAdvance('D')) {
                if (source.MatchAndAdvance('P')) {
                    return AssembleCdp(source, result);
                }
            } else if (source.MatchAndAdvance('M')) {
                if (source.MatchAndAdvance('N')) {
                    return AssembleCmn(source, result);
                } else if (source.MatchAndAdvance('P')) {
                    return AssembleCmp(source, result);
                }
            }
        } else if (source.MatchAndAdvance('E')) {
            if (source.MatchAndAdvance('O')) {
                if (source.MatchAndAdvance('R')) {
                    return AssembleEor(source, result);
                }
            }
        } else if (source.MatchAndAdvance('L')) {
            if (source.MatchAndAdvance('D')) {
                if (source.MatchAndAdvance('C')) {
                    return AssembleLdc(source, result);
                } else if (source.MatchAndAdvance('R')) {
                    return AssembleLdr(source, result);
                } else if (source.MatchAndAdvance('M')) {
                    return AssembleLdm(source, result);
                }
            }
        } else if (source.MatchAndAdvance('M')) {
            if (source.MatchAndAdvance('C')) {
                if (source.MatchAndAdvance('R')) {
                    return AssembleMcr(source, result);
                }
            } else if (source.MatchAndAdvance('L')) {
                if (source.MatchAndAdvance('A')) {
                    return AssembleMla(source, result);
                }
            } else if (source.MatchAndAdvance('O')) {
                if (source.MatchAndAdvance('V')) {
                    return AssembleMov(source, result);
                }
            } else if (source.MatchAndAdvance('R')) {
                if (source.MatchAndAdvance('C')) {
                    return AssembleMrc(source, result);
                }
            } else if (source.MatchAndAdvance('U')) {
                if (source.MatchAndAdvance('L')) {
                    return AssembleMul(source, result);
                }
            } else if (source.MatchAndAdvance('V')) {
                if (source.MatchAndAdvance('N')) {
                    return AssembleMvn(source, result);
                }
            }
        } else if (source.MatchAndAdvance('O')) {
            if (source.MatchAndAdvance('R')) {
                if (source.MatchAndAdvance('R')) {
                    return AssembleOrr(source, result);
                }
            }
        } else if (source.MatchAndAdvance('R')) {
            if (source.MatchAndAdvance('S')) {
                if (source.MatchAndAdvance('B')) {
                    return AssembleRsb(source, result);
                } else if (source.MatchAndAdvance('C')) {
                    return AssembleRsc(source, result);
                }
            }
        } else if (source.MatchAndAdvance('S')) {
            if (source.MatchAndAdvance('B')) {
                if (source.MatchAndAdvance('C')) {
                    return AssembleSbc(source, result);
                }
            } else if (source.MatchAndAdvance('T')) {
                if (source.MatchAndAdvance('C')) {
                    return AssembleStc(source, result);
                } else if (source.MatchAndAdvance('R')) {
                    return AssembleStr(source, result);
                } else if (source.MatchAndAdvance('M')) {
                    return AssembleStm(source, result);
                }
            } else if (source.MatchAndAdvance('U')) {
                if (source.MatchAndAdvance('B')) {
                    return AssembleSub(source, result);
                }
            } else if (source.MatchAndAdvance('W')) {
                if (source.MatchAndAdvance('I')) {
                    return AssembleSwi(source, result);
                } else if (source.MatchAndAdvance('P')) {
                    return AssembleSwp(source, result);
                }
            }
        } else if (source.MatchAndAdvance('T')) {
            if (source.MatchAndAdvance('E')) {
                if (source.MatchAndAdvance('Q')) {
                    return AssembleTeq(source, result);
                }
            } else if (source.MatchAndAdvance('S')) {
                if (source.MatchAndAdvance('T')) {
                    return AssembleTst(source, result);
                }
            }
        }
    }
    return false;
}

auto Assembler::Assemble(const std::string& assembly, uint32_t& result) -> bool {
    if (assembly.empty()) return false;
    SourceLine source(assembly);
    return Assemble(source, result);
}