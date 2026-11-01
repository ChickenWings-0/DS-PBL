// =============================================================================
// PrefixEvaluator.cpp
//
// Evaluates prefix (Polish) expressions using LinkedStack.
//
// Algorithm (scan tokens right to left):
//   operand  -> push its value onto the stack
//   operator -> left = pop(), right = pop()  (FIRST pop is LEFT operand)
//               apply operator: push (left op right)
//   end      -> exactly one value must remain on the stack
//
// Time complexity:  O(n) where n = number of tokens
// Space complexity: O(n) for the operand stack
//
// Author: Bhavya Porwal (02318015625)
// =============================================================================
#include "PrefixEvaluator.h"

#include <stdexcept>
#include <string>
#include "ExprCommon.h"
#include "LinkedStack.h"

namespace {

// Fills an error result and records the error as a final trace row when tracing.
EvalResult fail(const std::string& message, TraceTable* trace,
                const std::string& symbol, const std::string& stack) {
    if (trace) trace->addRow(symbol, stack, "ERROR: " + message);
    EvalResult r;
    r.ok = false;
    r.value = 0.0;
    r.error = message;
    return r;
}

} // namespace

EvalResult evaluatePrefix(const std::string& prefix, const VarTable& vars,
                          TraceTable* trace) {
    // Step 1: Tokenize and reject invalid prefix expressions.
    TokenizeResult tr = tokenize(prefix);
    if (!tr.ok) return fail(tr.error, nullptr, "", "");
    if (tr.tokens.empty()) return fail("Empty expression", nullptr, "", "");

    for (std::size_t i = 0; i < tr.tokens.size(); ++i) {
        if (tr.tokens[i].type == TokenType::LeftParen ||
            tr.tokens[i].type == TokenType::RightParen) {
            return fail("Brackets are not allowed in prefix expressions", nullptr, "", "");
        }
    }

    if (trace) trace->setHeaders("Symbol", "Stack", "Action");

    // Step 2: LinkedStack for evaluating values, scanning tokens right-to-left.
    LinkedStack<double> values;

    for (int i = static_cast<int>(tr.tokens.size()) - 1; i >= 0; --i) {
        const Token& t = tr.tokens[static_cast<std::size_t>(i)];
        std::string action;

        if (t.type == TokenType::Number) {
            double v = 0.0;
            try {
                v = std::stod(t.text);
            } catch (const std::out_of_range&) {
                return fail("Number too large at position " + std::to_string(t.position),
                            trace, t.text, values.toString());
            }
            values.push(v);
            action = "push " + formatNumber(v);
        } else if (t.type == TokenType::Variable) {
            VarTable::const_iterator it = vars.find(t.text);
            if (it == vars.end()) {
                return fail("Undefined variable '" + t.text + "'",
                            trace, t.text, values.toString());
            }
            values.push(it->second);
            action = "push " + t.text + "=" + formatNumber(it->second);
        } else {
            // Operator: requires two operands on the stack.
            if (values.size() < 2) {
                return fail("Insufficient operands for '" + t.text + "' at position "
                            + std::to_string(t.position), trace, t.text, values.toString());
            }

            // In prefix evaluation, the FIRST popped value is the LEFT operand,
            // and the SECOND popped value is the RIGHT operand.
            double left = values.pop();
            double right = values.pop();
            double result = 0.0;
            std::string err;

            if (!applyOperator(t.text, left, right, result, err)) {
                return fail(err + " at position " + std::to_string(t.position),
                            trace, t.text, values.toString());
            }

            values.push(result);
            action = formatNumber(left) + " " + t.text + " " + formatNumber(right)
                   + " = " + formatNumber(result);
        }

        if (trace) trace->addRow(t.text, values.toString(), action);
    }

    // Step 3: A valid prefix expression must leave exactly one value on the stack.
    if (values.size() != 1) {
        return fail("Too many operands: " + std::to_string(values.size())
                    + " values left on the stack", trace, "(end)", values.toString());
    }

    EvalResult r;
    r.ok = true;
    r.value = values.pop();
    return r;
}
