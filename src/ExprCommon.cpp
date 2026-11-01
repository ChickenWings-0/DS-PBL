// =============================================================================
// ExprCommon.cpp
// Tokenizer, infix validator and shared helpers (see ExprCommon.h).
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#include "ExprCommon.h"

#include <cctype>
#include <cmath>
#include <sstream>
#include "ArrayStack.h"

namespace {

bool isDigit(char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }
bool isIdentStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool isIdentChar(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

// Builds "'x' at position 4" - used in many error messages.
std::string at(const Token& t) {
    return "'" + t.text + "' at position " + std::to_string(t.position);
}

} // namespace

// -----------------------------------------------------------------------------
// Tokenizer: one left-to-right scan. Multi-digit numbers and multi-letter
// variables are read as ONE token, so "12+x1" -> [12] [+] [x1].
// -----------------------------------------------------------------------------
TokenizeResult tokenize(const std::string& expr) {
    TokenizeResult result;
    result.ok = true;

    std::size_t i = 0;
    const std::size_t n = expr.size();

    while (i < n) {
        char c = expr[i];
        int position = static_cast<int>(i) + 1;   // 1-based for the user

        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        Token token;
        token.position = position;

        if (isDigit(c)) {
            // Number: digits [ '.' digits ]
            std::size_t start = i;
            while (i < n && isDigit(expr[i])) ++i;
            if (i < n && expr[i] == '.') {
                ++i;
                if (i >= n || !isDigit(expr[i])) {
                    result.ok = false;
                    result.error = "Invalid number '" + expr.substr(start, i - start)
                                 + "' at position " + std::to_string(position);
                    return result;
                }
                while (i < n && isDigit(expr[i])) ++i;
                if (i < n && expr[i] == '.') {
                    result.ok = false;
                    result.error = "Invalid number (two decimal points) at position "
                                 + std::to_string(position);
                    return result;
                }
            }
            token.type = TokenType::Number;
            token.text = expr.substr(start, i - start);
        } else if (isIdentStart(c)) {
            // Variable: letter/_ followed by letters/digits/_
            std::size_t start = i;
            while (i < n && isIdentChar(expr[i])) ++i;
            token.type = TokenType::Variable;
            token.text = expr.substr(start, i - start);
        } else if (std::string("+-*/%^").find(c) != std::string::npos) {
            token.type = TokenType::Operator;
            token.text = std::string(1, c);
            ++i;
        } else if (c == '(' || c == '[' || c == '{') {
            token.type = TokenType::LeftParen;
            token.text = std::string(1, c);
            ++i;
        } else if (c == ')' || c == ']' || c == '}') {
            token.type = TokenType::RightParen;
            token.text = std::string(1, c);
            ++i;
        } else {
            result.ok = false;
            result.error = std::string("Invalid character '") + c + "' at position "
                         + std::to_string(position);
            return result;
        }

        result.tokens.push_back(token);
    }
    return result;
}

// -----------------------------------------------------------------------------
// Infix validator. Walks the tokens with one flag:
//   expectOperand == true  -> next must be a number, variable or opening bracket
//   expectOperand == false -> next must be an operator or closing bracket
// Opening brackets are kept on an ArrayStack (by token index) to check nesting.
// -----------------------------------------------------------------------------
std::string validateInfix(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return "Empty expression";
    }

    ArrayStack<int> openBrackets(static_cast<int>(tokens.size()));
    bool expectOperand = true;

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const Token& t = tokens[i];
        const Token* prev = (i > 0) ? &tokens[i - 1] : nullptr;

        switch (t.type) {
        case TokenType::Number:
        case TokenType::Variable:
            if (!expectOperand) {
                return "Missing operator between '" + prev->text + "' and " + at(t);
            }
            expectOperand = false;
            break;

        case TokenType::Operator:
            if (expectOperand) {
                // An operator where an operand was expected: either unary or missing operand.
                if (t.text == "+" || t.text == "-") {
                    return "Unary " + at(t) + " is not supported; write (0" + t.text + "x) instead";
                }
                return "Operator " + at(t) + " is missing its left operand";
            }
            expectOperand = true;
            break;

        case TokenType::LeftParen:
            if (!expectOperand) {
                return "Missing operator before " + at(t);
            }
            openBrackets.push(static_cast<int>(i));
            break;

        case TokenType::RightParen:
            if (openBrackets.isEmpty()) {
                return "Unmatched closing " + at(t);
            }
            if (expectOperand) {
                if (prev->type == TokenType::LeftParen) {
                    return "Empty brackets at position " + std::to_string(prev->position);
                }
                return "Operator " + at(*prev) + " is missing its right operand";
            }
            {
                const Token& open = tokens[openBrackets.pop()];
                if (matchingOpen(t.text[0]) != open.text[0]) {
                    return "Bracket mismatch: " + at(open) + " is closed by " + at(t);
                }
            }
            expectOperand = false;
            break;
        }
    }

    if (expectOperand) {
        return "Expression ends with operator " + at(tokens.back())
             + " - missing right operand";
    }
    if (!openBrackets.isEmpty()) {
        return "Unclosed " + at(tokens[openBrackets.peek()]);
    }
    return "";
}

// -----------------------------------------------------------------------------
// Operator helpers
// -----------------------------------------------------------------------------
bool isOperator(const std::string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "%" || s == "^";
}

int precedence(const std::string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool isRightAssociative(const std::string& op) {
    return op == "^";   // 2^3^2 = 2^(3^2); all others group left to right
}

char matchingOpen(char close) {
    switch (close) {
    case ')': return '(';
    case ']': return '[';
    case '}': return '{';
    default:  return '\0';
    }
}

char matchingClose(char open) {
    switch (open) {
    case '(': return ')';
    case '[': return ']';
    case '{': return '}';
    default:  return '\0';
    }
}

// -----------------------------------------------------------------------------
// Safe arithmetic. Returns false (with a message) instead of crashing or
// producing inf/nan.
// -----------------------------------------------------------------------------
bool applyOperator(const std::string& op, double left, double right,
                   double& result, std::string& error) {
    if (op == "+") {
        result = left + right;
    } else if (op == "-") {
        result = left - right;
    } else if (op == "*") {
        result = left * right;
    } else if (op == "/") {
        if (right == 0.0) {
            error = "Division by zero";
            return false;
        }
        result = left / right;
    } else if (op == "%") {
        if (right == 0.0) {
            error = "Modulo by zero";
            return false;
        }
        if (std::floor(left) != left || std::floor(right) != right) {
            error = "Modulo requires integer operands";
            return false;
        }
        result = std::fmod(left, right);
    } else if (op == "^") {
        result = std::pow(left, right);
    } else {
        error = "Unknown operator '" + op + "'";
        return false;
    }

    if (!std::isfinite(result)) {
        error = "Result is not a finite number";
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Formatting helpers
// -----------------------------------------------------------------------------
std::vector<std::string> extractVariables(const std::string& expr) {
    std::vector<std::string> names;
    TokenizeResult tr = tokenize(expr);
    for (std::size_t i = 0; i < tr.tokens.size(); ++i) {
        if (tr.tokens[i].type != TokenType::Variable) continue;
        bool seen = false;
        for (std::size_t j = 0; j < names.size(); ++j) {
            if (names[j] == tr.tokens[i].text) { seen = true; break; }
        }
        if (!seen) names.push_back(tr.tokens[i].text);
    }
    return names;
}

std::string formatNumber(double value) {
    if (value == 0.0) return "0";   // also turns -0 into "0"
    // Whole numbers are printed without a decimal point.
    if (std::floor(value) == value && std::fabs(value) < 1e15) {
        return std::to_string(static_cast<long long>(value));
    }
    std::ostringstream out;
    out.precision(6);
    out << value;
    return out.str();
}

std::string joinTokens(const std::vector<std::string>& parts) {
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) result += ' ';
        result += parts[i];
    }
    return result;
}
