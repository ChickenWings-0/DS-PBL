// =============================================================================
// PostfixEvaluator.h
// Evaluation of postfix (Reverse Polish) expressions.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef POSTFIX_EVALUATOR_H
#define POSTFIX_EVALUATOR_H

#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

// Evaluates space/operator-separated postfix using ArrayStack<double>.
// Variables are looked up in vars. Errors: empty input, invalid char,
// brackets present, undefined variable, insufficient operands,
// too many operands, and arithmetic errors from applyOperator.
EvalResult evaluatePostfix(const std::string& postfix, const VarTable& vars,
                           TraceTable* trace = nullptr);

#endif // POSTFIX_EVALUATOR_H
