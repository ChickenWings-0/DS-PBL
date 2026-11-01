// =============================================================================
// ParenChecker.h
// Balanced-brackets checker for (), {}, [] using LinkedStack.
//
// Interface: fixed by the team; do not change without both members agreeing
// Implemented by: Bhavya Porwal (02318015625) in src/ParenChecker.cpp
// =============================================================================
#ifndef PAREN_CHECKER_H
#define PAREN_CHECKER_H

#include <string>
#include "TraceTable.h"

enum class ParenStatus { Balanced, UnexpectedClosing, MismatchedPair, UnclosedOpening };

struct ParenResult {
    bool        balanced;
    ParenStatus status;
    int         position;      // 1-based position of the offending bracket; -1 if balanced
    char        found;         // offending bracket character; '\0' if balanced
    char        expected;      // closing bracket that was expected; '\0' if none
    int         openPosition;  // position of the related opening bracket; -1 if none
    std::string message;       // full human-readable message
};

// Scans every character; only ( ) [ ] { } matter, everything else is ignored.
// Uses LinkedStack. Stops at the FIRST error.
ParenResult checkBalanced(const std::string& expr, TraceTable* trace = nullptr);

#endif // PAREN_CHECKER_H
