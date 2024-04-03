#include "info_reason.h"

bool IsLegalMoveReason(SurakartaIllegalMoveReason reason) {
    switch (reason) {
    case SurakartaIllegalMoveReason::LEGAL:
    case SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE:
    case SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE:
        return true;
    default:
        return false;
    }
}

bool IsEndReason(SurakartaEndReason reason) {
    switch (reason) {
    case SurakartaEndReason::NONE:
        return false;
    default:
        return true;
    }
}
