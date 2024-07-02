#include "Assembler/Assembler_Impl.h"
#include "Assembler/SourceLine.h"

#include "Common/Constants.h"

using namespace rbrown::arm;

auto Assembler::AssembleConditionCode(SourceLine& source, uint32_t& conditionCode) -> bool {
    if (source.MatchAndAdvance('A')) {
        if (source.MatchAndAdvance('L')) {
            conditionCode = CONDITION_CODE_AL;
            return true;
        }
    } else {
        if (source.MatchAndAdvance('C')) {
            if (source.MatchAndAdvance('C')) {
                conditionCode = CONDITION_CODE_CC;
                return true;
            } else if (source.MatchAndAdvance('S')) {
                conditionCode = CONDITION_CODE_CS;
                return true;
            }
        } else if (source.MatchAndAdvance('E')) {
            if (source.MatchAndAdvance('Q')) {
                conditionCode = CONDITION_CODE_EQ;
                return true;
            }
        } else if (source.MatchAndAdvance('G')) {
            if (source.MatchAndAdvance('E')) {
                conditionCode = CONDITION_CODE_GE;
                return true;
            } else if (source.MatchAndAdvance('T')) {
                conditionCode = CONDITION_CODE_GT;
                return true;
            }
        } else if (source.MatchAndAdvance('H')) {
            if (source.MatchAndAdvance('I')) {
                conditionCode = CONDITION_CODE_HI;
                return true;
            }
        } else if (source.MatchAndAdvance('L')) {
            if (source.MatchAndAdvance('E')) {
                conditionCode = CONDITION_CODE_LE;
                return true;
            } else if (source.MatchAndAdvance('S')) {
                conditionCode = CONDITION_CODE_LS;
                return true;
            } else if (source.MatchAndAdvance('T')) {
                conditionCode = CONDITION_CODE_LT;
                return true;
            }
        } else if (source.MatchAndAdvance('M')) {
            if (source.MatchAndAdvance('I')) {
                conditionCode = CONDITION_CODE_MI;
                return true;
            }
        } else if (source.MatchAndAdvance('N')) {
            if (source.MatchAndAdvance('E')) {
                conditionCode = CONDITION_CODE_NE;
                return true;
            } else if (source.MatchAndAdvance('V')) {
                conditionCode = CONDITION_CODE_NV;
                return true;
            }
        } else if (source.MatchAndAdvance('P')) {
            if (source.MatchAndAdvance('L')) {
                conditionCode = CONDITION_CODE_PL;
                return true;
            }
        } else if (source.MatchAndAdvance('V')) {
            if (source.MatchAndAdvance('C')) {
                conditionCode = CONDITION_CODE_VC;
                return true;
            } else if (source.MatchAndAdvance('S')) {
                conditionCode = CONDITION_CODE_VS;
                return true;
            }
        }
    }
    return false;
}