// =============================================================================
// InfixToPrefix.cpp
//
// Converts an infix expression to prefix (Polish) notation using LinkedStack.
//
// Algorithm (reverse -> modified postfix -> reverse):
//   1. Tokenize input and validate infix grammar.
//   2. Reverse the token vector and swap bracket types/symbols:
//      LeftParen <-> RightParen, '(' <-> ')', '[' <-> ']', '{' <-> '}'.
//   3. Run operator-stack algorithm (similar to postfix, but right-associative
//      operators pop on equal precedence, left-associative stay on stack).
//   4. Pop remaining operators into output.
//   5. Reverse the output tokens to obtain prefix notation.
//
// Time complexity:  O(n) where n = number of tokens
// Space complexity: O(n) for the operator stack and output list
//
// Author: Bhavya Porwal (02318015625)
// =============================================================================
#include "InfixToPrefix.h"

#include <algorithm>
#include <string>
#include <vector>
#include "ExprCommon.h"
#include "LinkedStack.h"

namespace {

bool isOpenBracket(const std::string& s) {
    return s == "(" || s == "[" || s == "{";
}

} // namespace

ConversionResult infixToPrefix(const std::string& infix, TraceTable* trace) {
    ConversionResult result;
    result.ok = false;

    // Step 1: Tokenize the input expression.
    TokenizeResult tr = tokenize(infix);
    if (!tr.ok) {
        result.error = tr.error;
        return result;
    }

    // Step 2: Validate infix grammar (brackets, operands, operators).
    std::string err = validateInfix(tr.tokens);
    if (!err.empty()) {
        result.error = err;
        return result;
    }

    if (trace) trace->setHeaders("Symbol", "Stack", "Output (reversed)");

    // Step 3: Reverse the token list (tokens remain intact, e.g. "12" stays "12").
    std::vector<Token> revTokens = tr.tokens;
    std::reverse(revTokens.begin(), revTokens.end());

    // Swap bracket roles and text so that '(' acts as closer and ')' as opener
    for (std::size_t i = 0; i < revTokens.size(); ++i) {
        if (revTokens[i].type == TokenType::LeftParen) {
            revTokens[i].type = TokenType::RightParen;
            if (revTokens[i].text == "(") revTokens[i].text = ")";
            else if (revTokens[i].text == "[") revTokens[i].text = "]";
            else if (revTokens[i].text == "{") revTokens[i].text = "}";
        } else if (revTokens[i].type == TokenType::RightParen) {
            revTokens[i].type = TokenType::LeftParen;
            if (revTokens[i].text == ")") revTokens[i].text = "(";
            else if (revTokens[i].text == "]") revTokens[i].text = "[";
            else if (revTokens[i].text == "}") revTokens[i].text = "{";
        }
    }

    // Step 4: Run the modified operator-stack algorithm using LinkedStack.
    LinkedStack<std::string> ops;
    std::vector<std::string> output;

    for (std::size_t i = 0; i < revTokens.size(); ++i) {
        const Token& t = revTokens[i];

        switch (t.type) {
        case TokenType::Number:
        case TokenType::Variable:
            output.push_back(t.text);
            break;

        case TokenType::LeftParen:
            ops.push(t.text);
            break;

        case TokenType::RightParen:
            // Pop until matching opening bracket
            while (!ops.isEmpty() && !isOpenBracket(ops.peek())) {
                output.push_back(ops.pop());
            }
            if (!ops.isEmpty()) {
                ops.pop(); // Discard the opening bracket
            }
            break;

        case TokenType::Operator:
            // Operator associativity rule is inverted compared to infix->postfix:
            // Right-associative operators (like ^) pop on equal precedence,
            // while left-associative operators (+, -, *, /, %) stay on stack.
            while (!ops.isEmpty() && isOperator(ops.peek()) &&
                   (precedence(ops.peek()) > precedence(t.text) ||
                    (precedence(ops.peek()) == precedence(t.text) && isRightAssociative(t.text)))) {
                output.push_back(ops.pop());
            }
            ops.push(t.text);
            break;
        }

        if (trace) trace->addRow(t.text, ops.toString(), joinTokens(output));
    }

    // Step 5: Flush any remaining operators from the stack into output.
    while (!ops.isEmpty()) {
        output.push_back(ops.pop());
    }
    if (trace) trace->addRow("(end)", "", joinTokens(output));

    // Step 6: Reverse the output to produce the final prefix expression.
    std::reverse(output.begin(), output.end());
    if (trace) trace->addRow("(reverse)", "", joinTokens(output));

    result.ok = true;
    result.output = joinTokens(output);
    return result;
}
