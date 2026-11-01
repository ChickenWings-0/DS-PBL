// =============================================================================
// InfixToPostfix.cpp
//
// Algorithm (scan tokens left to right):
//   operand            -> append to output
//   opening bracket    -> push
//   closing bracket    -> pop to output until the opening bracket; discard it
//   operator o         -> while top is an operator with
//                           prec(top) > prec(o)  OR
//                           prec(top) == prec(o) AND o is left-associative
//                         pop top to output;  then push o
//   end of input       -> pop everything left to output
//
// Time O(n), space O(n), where n = number of tokens.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#include "InfixToPostfix.h"

#include <vector>
#include "ArrayStack.h"

namespace {

bool isOpenBracket(const std::string& s) {
    return s == "(" || s == "[" || s == "{";
}

} // namespace

ConversionResult infixToPostfix(const std::string& infix, TraceTable* trace) {
    ConversionResult result;
    result.ok = false;

    // Step 1: tokenize and validate. Any error is returned to the caller as-is.
    TokenizeResult tr = tokenize(infix);
    if (!tr.ok) {
        result.error = tr.error;
        return result;
    }
    std::string err = validateInfix(tr.tokens);
    if (!err.empty()) {
        result.error = err;
        return result;
    }

    if (trace) trace->setHeaders("Symbol", "Stack", "Postfix");

    // Step 2: the operator stack can never hold more items than there are tokens,
    // so this capacity guarantees no overflow.
    ArrayStack<std::string> ops(static_cast<int>(tr.tokens.size()));
    std::vector<std::string> output;

    for (std::size_t i = 0; i < tr.tokens.size(); ++i) {
        const Token& t = tr.tokens[i];

        switch (t.type) {
        case TokenType::Number:
        case TokenType::Variable:
            output.push_back(t.text);
            break;

        case TokenType::LeftParen:
            ops.push(t.text);
            break;

        case TokenType::RightParen:
            // Pop until the matching opening bracket (validator guarantees it exists).
            while (!ops.isEmpty() && !isOpenBracket(ops.peek())) {
                output.push_back(ops.pop());
            }
            if (!ops.isEmpty()) ops.pop();   // discard the opening bracket
            break;

        case TokenType::Operator:
            // Pop operators that must be applied before this one.
            while (!ops.isEmpty() && isOperator(ops.peek()) &&
                   (precedence(ops.peek()) > precedence(t.text) ||
                    (precedence(ops.peek()) == precedence(t.text) &&
                     !isRightAssociative(t.text)))) {
                output.push_back(ops.pop());
            }
            ops.push(t.text);
            break;
        }

        if (trace) trace->addRow(t.text, ops.toString(), joinTokens(output));
    }

    // Step 3: flush the remaining operators.
    while (!ops.isEmpty()) {
        output.push_back(ops.pop());
    }
    if (trace) trace->addRow("(end)", "", joinTokens(output));

    result.ok = true;
    result.output = joinTokens(output);
    return result;
}
