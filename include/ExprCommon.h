// =============================================================================
// ExprCommon.h
// Types and helper functions shared by every expression module:
// tokenizer, infix validator, operator precedence/associativity,
// safe arithmetic (division by zero etc.) and formatting helpers.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef EXPR_COMMON_H
#define EXPR_COMMON_H

#include <map>
#include <string>
#include <vector>

enum class TokenType { Number, Variable, Operator, LeftParen, RightParen };

struct Token {
    TokenType   type;
    std::string text;      // "12", "x", "+", "(", "]" ...
    int         position;  // 1-based column in the original input
};

struct TokenizeResult {
    bool               ok;
    std::vector<Token> tokens;
    std::string        error;   // e.g. "Invalid character '$' at position 4"
};

struct ConversionResult {
    bool        ok;
    std::string output;   // space-separated tokens, e.g. "a b c * +"
    std::string error;
};

struct EvalResult {
    bool        ok;
    double      value;
    std::string error;
};

typedef std::map<std::string, double> VarTable;   // variable name -> value

// Splits an expression into tokens. Whitespace is skipped.
//   Number   : digits, optionally '.' followed by digits  (7, 123, 3.5)
//   Variable : letter or '_' followed by letters/digits/'_' (a, x1, total)
//   Operator : + - * / % ^
//   Brackets : ( [ {  -> LeftParen,   ) ] }  -> RightParen
//   Anything else -> ok=false, error names the character and its position.
TokenizeResult tokenize(const std::string& expr);

// Checks infix grammar: non-empty, operands and operators alternate,
// brackets nest and match by type, no empty "()", no unary operators.
// Returns "" when valid, otherwise a human-readable error message.
std::string validateInfix(const std::vector<Token>& tokens);

bool isOperator(const std::string& s);          // one of + - * / % ^
int  precedence(const std::string& op);         // ^ -> 3,  * / % -> 2,  + - -> 1,  other -> 0
bool isRightAssociative(const std::string& op); // true only for "^"
char matchingOpen(char close);                  // ')'->'(', ']'->'[', '}'->'{', else '\0'
char matchingClose(char open);                  // '('->')', '['->']', '{'->'}', else '\0'

// Applies a binary operator. Returns false and sets error on:
//   "Division by zero", "Modulo by zero",
//   "Modulo requires integer operands", "Result is not a finite number".
bool applyOperator(const std::string& op, double left, double right,
                   double& result, std::string& error);

// Unique variable names in order of first appearance, e.g. "a+b*a" -> {"a","b"}.
std::vector<std::string> extractVariables(const std::string& expr);

// 12 -> "12", 3.5 -> "3.5", 1/3 -> "0.333333" (up to 6 significant digits).
std::string formatNumber(double value);

// Joins strings with single spaces: {"a","b","+"} -> "a b +".
std::string joinTokens(const std::vector<std::string>& parts);

#endif // EXPR_COMMON_H
