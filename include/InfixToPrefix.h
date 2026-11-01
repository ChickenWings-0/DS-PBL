// =============================================================================
// InfixToPrefix.h
// Infix -> prefix conversion (reverse, convert, reverse) using LinkedStack.
//
// Interface: agreed in PLAN.md section 5.9 (do not change without both members)
// Implemented by: Bhavya Porwal (02318015625) in src/InfixToPrefix.cpp
// =============================================================================
#ifndef INFIX_TO_PREFIX_H
#define INFIX_TO_PREFIX_H

#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

// Tokenize + validateInfix (from ExprCommon), reverse tokens, swap brackets,
// run the operator-stack algorithm with LinkedStack<std::string>, then reverse output.
ConversionResult infixToPrefix(const std::string& infix, TraceTable* trace = nullptr);

#endif // INFIX_TO_PREFIX_H
