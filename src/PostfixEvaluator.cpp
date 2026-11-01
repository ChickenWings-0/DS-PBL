// =============================================================================
// PostfixEvaluator.cpp
//
// Algorithm (scan tokens left to right):
//   operand  -> push its value
//   operator -> right = pop, left = pop   (FIRST pop is the RIGHT operand)
//               push (left op right)
//   end      -> exactly one value must remain: that is the answer
//
// Time O(n), space O(n).
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#include "PostfixEvaluator.h"

#include <stdexcept>
#include "ArrayStack.h"

namespace {

// Fills an error result and, when tracing, records the error as a final row.
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

EvalResult evaluatePostfix(const std::string& postfix, const VarTable& vars,
                           TraceTable* trace) {
    // Step 1: tokenize and reject inputs that cannot be postfix.
    TokenizeResult tr = tokenize(postfix);
    if (!tr.ok) return fail(tr.error, nullptr, "", "");
    if (tr.tokens.empty()) return fail("Empty expression", nullptr, "", "");
    for (std::size_t i = 0; i < tr.tokens.size(); ++i) {
        if (tr.tokens[i].type == TokenType::LeftParen ||
            tr.tokens[i].type == TokenType::RightParen) {
            return fail("Brackets are not allowed in postfix expressions", nullptr, "", "");
        }
    }

    if (trace) trace->setHeaders("Symbol", "Stack", "Action");

    // Step 2: the value stack never holds more items than there are tokens.
    ArrayStack<double> values(static_cast<int>(tr.tokens.size()));

    for (std::size_t i = 0; i < tr.tokens.size(); ++i) {
        const Token& t = tr.tokens[i];
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
                return fail("Undefined variable '" + t.text + "'", trace, t.text, values.toString());
            }
            values.push(it->second);
            action = "push " + t.text + "=" + formatNumber(it->second);
        } else {
            // Operator: needs two operands on the stack.
            if (values.size() < 2) {
                return fail("Insufficient operands for '" + t.text + "' at position "
                            + std::to_string(t.position), trace, t.text, values.toString());
            }
            double right = values.pop();   // popped first = right operand
            double left = values.pop();
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

    // Step 3: a valid postfix expression leaves exactly one value.
    if (values.size() != 1) {
        return fail("Too many operands: " + std::to_string(values.size())
                    + " values left on the stack", trace, "(end)", values.toString());
    }

    EvalResult r;
    r.ok = true;
    r.value = values.pop();
    return r;
}
