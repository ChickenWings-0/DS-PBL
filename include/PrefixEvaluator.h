// =============================================================================
// PrefixEvaluator.h
// Evaluation of prefix (Polish) expressions using LinkedStack.
//
// Interface: agreed in PLAN.md section 5.10 (do not change without both members)
// Implemented by: Bhavya Porwal (02318015625) in src/PrefixEvaluator.cpp
// =============================================================================
#ifndef PREFIX_EVALUATOR_H
#define PREFIX_EVALUATOR_H

#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

// Scans prefix tokens right-to-left using LinkedStack<double>.
// First value popped = LEFT operand. Same errors as evaluatePostfix.
EvalResult evaluatePrefix(const std::string& prefix, const VarTable& vars,
                          TraceTable* trace = nullptr);

#endif // PREFIX_EVALUATOR_H
