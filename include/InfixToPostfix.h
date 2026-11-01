// =============================================================================
// InfixToPostfix.h
// Infix -> postfix conversion (operator-stack / shunting-yard algorithm).
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

// Tokenizes + validates infix, then converts with the operator-stack algorithm
// using ArrayStack<std::string>. Output tokens are space-separated.
ConversionResult infixToPostfix(const std::string& infix, TraceTable* trace = nullptr);

#endif // INFIX_TO_POSTFIX_H
