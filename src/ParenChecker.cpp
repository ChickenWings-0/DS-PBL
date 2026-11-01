// =============================================================================
// ParenChecker.cpp
//
// Balanced-brackets checker for (), {}, and [] using LinkedStack.
// Scans characters left to right, reporting the exact position and nature
// of any mismatch, unexpected closing bracket, or unclosed opening bracket.
//
// Time complexity:  O(n) where n = length of expression
// Space complexity: O(n) for the stack
//
// Author: Bhavya Porwal (02318015625)
// =============================================================================
#include "ParenChecker.h"

#include <iostream>
#include <string>
#include "ExprCommon.h"
#include "LinkedStack.h"

namespace {

// Stores an opening bracket symbol along with its 1-based position in the input.
struct OpenBracket {
    char symbol;
    int  position;
};

// LinkedStack::toString() uses operator<<. Print only the bracket character.
std::ostream& operator<<(std::ostream& out, const OpenBracket& b) {
    return out << b.symbol;
}

bool isOpenBracket(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool isCloseBracket(char c) {
    return c == ')' || c == ']' || c == '}';
}

} // namespace

ParenResult checkBalanced(const std::string& expr, TraceTable* trace) {
    if (trace) trace->setHeaders("Symbol", "Stack", "Action");

    LinkedStack<OpenBracket> stack;

    for (std::size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        int position = static_cast<int>(i) + 1; // 1-based index for user messages

        if (isOpenBracket(c)) {
            // Push opening bracket with its position
            OpenBracket ob;
            ob.symbol = c;
            ob.position = position;
            stack.push(ob);

            if (trace) {
                trace->addRow(std::string(1, c) + " @" + std::to_string(position),
                              stack.toString(), "push");
            }
        } else if (isCloseBracket(c)) {
            // Closing bracket without any preceding opening bracket
            if (stack.isEmpty()) {
                if (trace) {
                    trace->addRow(std::string(1, c) + " @" + std::to_string(position),
                                  stack.toString(),
                                  "ERROR: no opening bracket for '" + std::string(1, c) + "'");
                }
                ParenResult res;
                res.balanced = false;
                res.status = ParenStatus::UnexpectedClosing;
                res.position = position;
                res.found = c;
                res.expected = '\0';
                res.openPosition = -1;
                res.message = "Unexpected closing '" + std::string(1, c) + "' at position "
                            + std::to_string(position) + ": no matching opening bracket.";
                return res;
            }

            OpenBracket top = stack.peek();
            char expectedClose = matchingClose(top.symbol);

            // Mismatched bracket type (e.g., '(' closed by ']')
            if (top.symbol != matchingOpen(c)) {
                if (trace) {
                    trace->addRow(std::string(1, c) + " @" + std::to_string(position),
                                  stack.toString(),
                                  "ERROR: expected '" + std::string(1, expectedClose)
                                  + "' but found '" + std::string(1, c) + "'");
                }
                ParenResult res;
                res.balanced = false;
                res.status = ParenStatus::MismatchedPair;
                res.position = position;
                res.found = c;
                res.expected = expectedClose;
                res.openPosition = top.position;
                res.message = "Mismatch at position " + std::to_string(position)
                            + ": found '" + std::string(1, c)
                            + "' but expected '" + std::string(1, expectedClose)
                            + "' to close '" + std::string(1, top.symbol)
                            + "' from position " + std::to_string(top.position) + ".";
                return res;
            }

            // Matched pair: pop and record trace
            stack.pop();
            if (trace) {
                trace->addRow(std::string(1, c) + " @" + std::to_string(position),
                              stack.toString(), "pop - matched");
            }
        }
        // Non-bracket characters are ignored (no trace row)
    }

    // After scanning all characters, check for unclosed opening brackets
    if (!stack.isEmpty()) {
        OpenBracket top = stack.peek(); // Innermost unclosed bracket is at the top
        char expectedClose = matchingClose(top.symbol);

        if (trace) {
            trace->addRow("(end)", stack.toString(),
                          "ERROR: unclosed '" + std::string(1, top.symbol) + "'");
        }

        ParenResult res;
        res.balanced = false;
        res.status = ParenStatus::UnclosedOpening;
        res.position = top.position;
        res.found = top.symbol;
        res.expected = expectedClose;
        res.openPosition = top.position;
        res.message = "Unclosed '" + std::string(1, top.symbol)
                    + "' at position " + std::to_string(top.position)
                    + ": no matching closing bracket.";
        return res;
    }

    // All brackets balanced
    ParenResult res;
    res.balanced = true;
    res.status = ParenStatus::Balanced;
    res.position = -1;
    res.found = '\0';
    res.expected = '\0';
    res.openPosition = -1;
    res.message = "Balanced: all brackets are properly matched.";
    return res;
}
