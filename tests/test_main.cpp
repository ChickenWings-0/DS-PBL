// =============================================================================
// test_main.cpp
//
// Self-contained test suite for the Expression Evaluator & Stack ADT project.
// Covers:
//   - ArrayStack (bounded capacity, overflow, underflow, toString)
//   - LinkedStack (unlimited and bounded capacity, overflow, underflow, toString)
//   - Tokenizer & Infix Validator
//   - Balanced-brackets checker (all status types, positions, error messages)
//   - Infix -> Postfix conversion
//   - Infix -> Prefix conversion (associativity flipping, reversed tokens)
//   - Postfix evaluation (standard operations, division/modulo by zero, errors)
//   - Prefix evaluation (operand order, all operators, error cases)
//   - Round-trip equivalence: eval(postfix(expr)) == eval(prefix(expr))
//
// Author: Bhavya Porwal (02318015625)
// =============================================================================
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "ArrayStack.h"
#include "ExprCommon.h"
#include "InfixToPostfix.h"
#include "InfixToPrefix.h"
#include "LinkedStack.h"
#include "ParenChecker.h"
#include "PostfixEvaluator.h"
#include "PrefixEvaluator.h"

namespace {

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {
    if (condition) {
        ++passed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++failed;
        std::cout << "  [FAIL] " << name << "\n";
    }
}

void checkEqual(const std::string& actual, const std::string& expected, const std::string& name) {
    if (actual == expected) {
        ++passed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++failed;
        std::cout << "  [FAIL] " << name << " -> Expected: \"" << expected
                  << "\", Got: \"" << actual << "\"\n";
    }
}

void checkNear(double actual, double expected, const std::string& name) {
    if (std::fabs(actual - expected) < 1e-9) {
        ++passed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++failed;
        std::cout << "  [FAIL] " << name << " -> Expected: " << expected
                  << ", Got: " << actual << "\n";
    }
}

// -----------------------------------------------------------------------------
// 1. ArrayStack tests
// -----------------------------------------------------------------------------
void testArrayStack() {
    std::cout << "\n--- 1. ArrayStack Tests ---\n";

    ArrayStack<int> s(3);
    check(s.isEmpty(), "ArrayStack initial isEmpty()");
    check(!s.isFull(), "ArrayStack initial !isFull()");
    check(s.size() == 0, "ArrayStack initial size() == 0");

    s.push(1);
    s.push(2);
    s.push(3);
    check(s.isFull(), "ArrayStack capacity 3: isFull() after 3 pushes");
    check(s.size() == 3, "ArrayStack size() == 3");
    checkEqual(s.toString(), "1 2 3", "ArrayStack toString() after push 1, 2, 3");

    bool overflowThrew = false;
    try {
        s.push(4);
    } catch (const StackOverflowError&) {
        overflowThrew = true;
    }
    check(overflowThrew, "ArrayStack throws StackOverflowError on 4th push");

    check(s.peek() == 3, "ArrayStack peek() == 3");
    check(s.pop() == 3, "ArrayStack pop() 1st == 3");
    check(s.pop() == 2, "ArrayStack pop() 2nd == 2");
    check(s.pop() == 1, "ArrayStack pop() 3rd == 1");
    check(s.isEmpty(), "ArrayStack isEmpty() after popping all");

    bool underflowThrew = false;
    try {
        s.pop();
    } catch (const StackUnderflowError&) {
        underflowThrew = true;
    }
    check(underflowThrew, "ArrayStack pop() throws StackUnderflowError when empty");

    bool peekUnderflowThrew = false;
    try {
        s.peek();
    } catch (const StackUnderflowError&) {
        peekUnderflowThrew = true;
    }
    check(peekUnderflowThrew, "ArrayStack peek() throws StackUnderflowError when empty");
}

// -----------------------------------------------------------------------------
// 2. LinkedStack tests
// -----------------------------------------------------------------------------
void testLinkedStack() {
    std::cout << "\n--- 2. LinkedStack Tests ---\n";

    LinkedStack<int> sBounded(2);
    sBounded.push(10);
    sBounded.push(20);
    check(sBounded.isFull(), "LinkedStack(maxSize=2) isFull() after 2 pushes");

    bool overflowThrew = false;
    try {
        sBounded.push(30);
    } catch (const StackOverflowError&) {
        overflowThrew = true;
    }
    check(overflowThrew, "LinkedStack throws StackOverflowError on exceeding maxSize");

    checkEqual(sBounded.toString(), "10 20", "LinkedStack toString() format bottom -> top");
    check(sBounded.pop() == 20, "LinkedStack pop() order 1st == 20");
    check(sBounded.pop() == 10, "LinkedStack pop() order 2nd == 10");

    bool underflowThrew = false;
    try {
        sBounded.pop();
    } catch (const StackUnderflowError&) {
        underflowThrew = true;
    }
    check(underflowThrew, "LinkedStack pop() throws StackUnderflowError when empty");

    bool peekUnderflowThrew = false;
    try {
        sBounded.peek();
    } catch (const StackUnderflowError&) {
        peekUnderflowThrew = true;
    }
    check(peekUnderflowThrew, "LinkedStack peek() throws StackUnderflowError when empty");

    // Unlimited LinkedStack
    LinkedStack<int> sUnlimited;
    for (int i = 1; i <= 1000; ++i) {
        sUnlimited.push(i);
    }
    check(sUnlimited.size() == 1000, "LinkedStack unlimited size() == 1000");
    check(!sUnlimited.isFull(), "LinkedStack unlimited !isFull()");
    check(sUnlimited.peek() == 1000, "LinkedStack unlimited peek() == 1000");
}

// -----------------------------------------------------------------------------
// 3. Tokenizer tests
// -----------------------------------------------------------------------------
void testTokenizer() {
    std::cout << "\n--- 3. Tokenizer Tests ---\n";

    TokenizeResult tr = tokenize("12+x1*3.5");
    check(tr.ok, "tokenize(\"12+x1*3.5\") ok");
    check(tr.tokens.size() == 5, "tokenize(\"12+x1*3.5\") token count == 5");
    if (tr.tokens.size() == 5) {
        check(tr.tokens[0].text == "12" && tr.tokens[0].position == 1 && tr.tokens[0].type == TokenType::Number, "Token 0: 12 @ 1");
        check(tr.tokens[1].text == "+" && tr.tokens[1].position == 3 && tr.tokens[1].type == TokenType::Operator, "Token 1: + @ 3");
        check(tr.tokens[2].text == "x1" && tr.tokens[2].position == 4 && tr.tokens[2].type == TokenType::Variable, "Token 2: x1 @ 4");
        check(tr.tokens[3].text == "*" && tr.tokens[3].position == 6 && tr.tokens[3].type == TokenType::Operator, "Token 3: * @ 6");
        check(tr.tokens[4].text == "3.5" && tr.tokens[4].position == 7 && tr.tokens[4].type == TokenType::Number, "Token 4: 3.5 @ 7");
    }

    TokenizeResult trBad = tokenize("a+b$");
    check(!trBad.ok, "tokenize(\"a+b$\") rejected due to invalid character");
    check(!trBad.error.empty(), "tokenize error message non-empty");
}

// -----------------------------------------------------------------------------
// 4. ParenChecker tests
// -----------------------------------------------------------------------------
void testParenChecker() {
    std::cout << "\n--- 4. ParenChecker Tests ---\n";

    // 4.1 Balanced with mixed brackets
    ParenResult r1 = checkBalanced("{[a+b]*(c-d)}");
    check(r1.balanced, "ParenChecker: {[a+b]*(c-d)} is balanced");
    check(r1.status == ParenStatus::Balanced, "ParenStatus is Balanced");
    checkEqual(r1.message, "Balanced: all brackets are properly matched.", "Balanced message check");

    // 4.2 No brackets
    ParenResult r2 = checkBalanced("abc");
    check(r2.balanced && r2.status == ParenStatus::Balanced, "ParenChecker: abc (no brackets) is balanced");

    // 4.3 Mismatched pair
    ParenResult r3 = checkBalanced("(a+b]");
    check(!r3.balanced, "ParenChecker: (a+b] is not balanced");
    check(r3.status == ParenStatus::MismatchedPair, "Status: MismatchedPair");
    check(r3.position == 5, "Mismatch position == 5");
    check(r3.found == ']', "Mismatch found == ']'");
    check(r3.expected == ')', "Mismatch expected == ')'");
    check(r3.openPosition == 1, "Mismatch openPosition == 1");
    checkEqual(r3.message, "Mismatch at position 5: found ']' but expected ')' to close '(' from position 1.", "Mismatch error message");

    // 4.4 Unexpected closing bracket
    ParenResult r4 = checkBalanced("a+b)");
    check(!r4.balanced, "ParenChecker: a+b) is not balanced");
    check(r4.status == ParenStatus::UnexpectedClosing, "Status: UnexpectedClosing");
    check(r4.position == 4, "Unexpected closing position == 4");
    check(r4.found == ')', "Unexpected closing found == ')'");
    checkEqual(r4.message, "Unexpected closing ')' at position 4: no matching opening bracket.", "UnexpectedClosing message");

    // 4.5 Unclosed opening bracket
    ParenResult r5 = checkBalanced("((a+b)");
    check(!r5.balanced, "ParenChecker: ((a+b) is not balanced");
    check(r5.status == ParenStatus::UnclosedOpening, "Status: UnclosedOpening");
    check(r5.position == 1, "Unclosed position == 1");
    check(r5.found == '(', "Unclosed found == '('");
    checkEqual(r5.message, "Unclosed '(' at position 1: no matching closing bracket.", "UnclosedOpening message (");

    // 4.6 Unclosed bracket [
    ParenResult r6 = checkBalanced("[(a+b)*c");
    check(!r6.balanced && r6.status == ParenStatus::UnclosedOpening, "ParenChecker: [(a+b)*c is UnclosedOpening");
    check(r6.position == 1 && r6.found == '[', "Unclosed position 1 '['");
    checkEqual(r6.message, "Unclosed '[' at position 1: no matching closing bracket.", "UnclosedOpening message [");

    // 4.7 Nested mismatch
    ParenResult r7 = checkBalanced("{a+(b*c})");
    check(!r7.balanced && r7.status == ParenStatus::MismatchedPair, "ParenChecker: {a+(b*c}) is MismatchedPair");
    check(r7.position == 8 && r7.found == '}' && r7.expected == ')' && r7.openPosition == 4, "Mismatch nested pos 8, open pos 4");
    checkEqual(r7.message, "Mismatch at position 8: found '}' but expected ')' to close '(' from position 4.", "Nested mismatch message");
}

// -----------------------------------------------------------------------------
// 5. Infix -> Postfix tests
// -----------------------------------------------------------------------------
void testInfixToPostfix() {
    std::cout << "\n--- 5. Infix -> Postfix Tests ---\n";

    checkEqual(infixToPostfix("a+b*c").output, "a b c * +", "Postfix: a+b*c");
    checkEqual(infixToPostfix("(A+B)*C").output, "A B + C *", "Postfix: (A+B)*C");
    checkEqual(infixToPostfix("2^3^2").output, "2 3 2 ^ ^", "Postfix: 2^3^2 (right-associative)");
    checkEqual(infixToPostfix("100-20-30").output, "100 20 - 30 -", "Postfix: 100-20-30 (left-associative)");
    checkEqual(infixToPostfix("(12+8)*3/4").output, "12 8 + 3 * 4 /", "Postfix: (12+8)*3/4");
    checkEqual(infixToPostfix("a+b*(c^d-e)^(f+g*h)-i").output, "a b c d ^ e - f g h * + ^ * + i -", "Postfix: complex expression");
    checkEqual(infixToPostfix("{a+b}*[c-d]").output, "a b + c d - *", "Postfix: mixed braces {a+b}*[c-d]");

    // Invalid expressions
    check(!infixToPostfix("a+*b").ok, "Postfix rejects a+*b");
    check(!infixToPostfix("(a+b").ok, "Postfix rejects (a+b (unclosed)");
    check(!infixToPostfix("a b").ok, "Postfix rejects adjacent operands");
    check(!infixToPostfix("()").ok, "Postfix rejects empty parentheses ()");
    check(!infixToPostfix("-5").ok, "Postfix rejects unary operator");
}

// -----------------------------------------------------------------------------
// 6. Infix -> Prefix tests
// -----------------------------------------------------------------------------
void testInfixToPrefix() {
    std::cout << "\n--- 6. Infix -> Prefix Tests ---\n";

    checkEqual(infixToPrefix("a+b*c").output, "+ a * b c", "Prefix: a+b*c");
    checkEqual(infixToPrefix("(A+B)*C").output, "* + A B C", "Prefix: (A+B)*C");
    checkEqual(infixToPrefix("2^3^2").output, "^ 2 ^ 3 2", "Prefix: 2^3^2 (right-associative)");
    checkEqual(infixToPrefix("100-20-30").output, "- - 100 20 30", "Prefix: 100-20-30 (left-associative)");
    checkEqual(infixToPrefix("(12+8)*3/4").output, "/ * + 12 8 3 4", "Prefix: (12+8)*3/4");
    checkEqual(infixToPrefix("10%4+7").output, "+ % 10 4 7", "Prefix: 10%4+7");
    checkEqual(infixToPrefix("{a+b}*[c-d]").output, "* + a b - c d", "Prefix: mixed braces {a+b}*[c-d]");
    checkEqual(infixToPrefix("a+b*(c^d-e)^(f+g*h)-i").output, "- + a * b ^ - ^ c d e + f * g h i", "Prefix: complex expression");
    checkEqual(infixToPrefix("x1 + total").output, "+ x1 total", "Prefix: variable identifiers");

    // Invalid expressions
    check(!infixToPrefix("a+*b").ok, "Prefix rejects a+*b");
    check(!infixToPrefix("a+b$").ok, "Prefix rejects a+b$");
}

// -----------------------------------------------------------------------------
// 7. Postfix Evaluation tests
// -----------------------------------------------------------------------------
void testPostfixEval() {
    std::cout << "\n--- 7. Postfix Evaluation Tests ---\n";

    VarTable emptyVars;
    checkNear(evaluatePostfix("2 3 4 * +", emptyVars).value, 14.0, "PostfixEval: 2 3 4 * + == 14");
    checkNear(evaluatePostfix("5 1 2 + 4 * + 3 -", emptyVars).value, 14.0, "PostfixEval: 5 1 2 + 4 * + 3 - == 14");
    checkNear(evaluatePostfix("12 8 + 3 * 4 /", emptyVars).value, 15.0, "PostfixEval: 12 8 + 3 * 4 / == 15");
    checkNear(evaluatePostfix("2 3 2 ^ ^", emptyVars).value, 512.0, "PostfixEval: 2 3 2 ^ ^ == 512");
    checkNear(evaluatePostfix("10 4 -", emptyVars).value, 6.0, "PostfixEval: 10 4 - == 6");
    checkNear(evaluatePostfix("7 2 /", emptyVars).value, 3.5, "PostfixEval: 7 2 / == 3.5");

    VarTable vars;
    vars["a"] = 2.0;
    vars["b"] = 3.0;
    vars["c"] = 4.0;
    checkNear(evaluatePostfix("a b c * +", vars).value, 14.0, "PostfixEval: a b c * + with variables == 14");

    // Errors
    check(!evaluatePostfix("5 0 /", emptyVars).ok, "PostfixEval: division by zero fails");
    check(!evaluatePostfix("5 +", emptyVars).ok, "PostfixEval: insufficient operands fails");
    check(!evaluatePostfix("5 6 7 +", emptyVars).ok, "PostfixEval: too many operands fails");
}

// -----------------------------------------------------------------------------
// 8. Prefix Evaluation tests
// -----------------------------------------------------------------------------
void testPrefixEval() {
    std::cout << "\n--- 8. Prefix Evaluation Tests ---\n";

    VarTable emptyVars;
    checkNear(evaluatePrefix("+ 2 3", emptyVars).value, 5.0, "PrefixEval: + 2 3 == 5");
    checkNear(evaluatePrefix("- 10 4", emptyVars).value, 6.0, "PrefixEval: - 10 4 == 6 (operand order check)");
    checkNear(evaluatePrefix("/ 20 4", emptyVars).value, 5.0, "PrefixEval: / 20 4 == 5");
    checkNear(evaluatePrefix("* + 2 3 4", emptyVars).value, 20.0, "PrefixEval: * + 2 3 4 == 20");
    checkNear(evaluatePrefix("/ * + 12 8 3 4", emptyVars).value, 15.0, "PrefixEval: / * + 12 8 3 4 == 15");
    checkNear(evaluatePrefix("^ 2 ^ 3 2", emptyVars).value, 512.0, "PrefixEval: ^ 2 ^ 3 2 == 512");
    checkNear(evaluatePrefix("- - 100 20 30", emptyVars).value, 50.0, "PrefixEval: - - 100 20 30 == 50");
    checkNear(evaluatePrefix("% 10 4", emptyVars).value, 2.0, "PrefixEval: % 10 4 == 2");
    checkNear(evaluatePrefix("/ 7 2", emptyVars).value, 3.5, "PrefixEval: / 7 2 == 3.5");

    VarTable vars;
    vars["a"] = 2.0;
    vars["b"] = 3.0;
    vars["c"] = 4.0;
    checkNear(evaluatePrefix("+ a * b c", vars).value, 14.0, "PrefixEval: + a * b c with variables == 14");

    // Specific error messages
    EvalResult rDiv0 = evaluatePrefix("/ 5 0", emptyVars);
    check(!rDiv0.ok && rDiv0.error == "Division by zero at position 1", "PrefixEval error: Division by zero at position 1");

    EvalResult rMod0 = evaluatePrefix("% 5 0", emptyVars);
    check(!rMod0.ok && rMod0.error == "Modulo by zero at position 1", "PrefixEval error: Modulo by zero at position 1");

    EvalResult rInsuff = evaluatePrefix("+ 5", emptyVars);
    check(!rInsuff.ok && rInsuff.error == "Insufficient operands for '+' at position 1", "PrefixEval error: Insufficient operands for '+' at position 1");

    EvalResult rTooMany = evaluatePrefix("+ 2 3 4", emptyVars);
    check(!rTooMany.ok && rTooMany.error == "Too many operands: 2 values left on the stack", "PrefixEval error: Too many operands: 2 values left on the stack");

    EvalResult rUndef = evaluatePrefix("+ 2 x", emptyVars);
    check(!rUndef.ok && rUndef.error == "Undefined variable 'x'", "PrefixEval error: Undefined variable 'x'");

    EvalResult rBrackets = evaluatePrefix("( + 2 3 )", emptyVars);
    check(!rBrackets.ok && rBrackets.error == "Brackets are not allowed in prefix expressions", "PrefixEval error: Brackets not allowed");

    EvalResult rEmpty = evaluatePrefix("", emptyVars);
    check(!rEmpty.ok && rEmpty.error == "Empty expression", "PrefixEval error: Empty expression");
}

// -----------------------------------------------------------------------------
// 9. Round-Trip tests
// -----------------------------------------------------------------------------
void testRoundTrip() {
    std::cout << "\n--- 9. Round-Trip Equivalence Tests ---\n";

    VarTable vars;
    vars["a"] = 1.0;
    vars["b"] = 2.0;
    vars["c"] = 3.0;
    vars["d"] = 4.0;
    vars["e"] = 5.0;
    vars["f"] = 6.0;
    vars["g"] = 7.0;
    vars["h"] = 8.0;
    vars["i"] = 9.0;

    std::vector<std::string> exprs;
    exprs.push_back("a+b*c");
    exprs.push_back("(12+8)*3/4");
    exprs.push_back("2^3^2");
    exprs.push_back("100-20-30");
    exprs.push_back("a+b*(c^d-e)^(f+g*h)-i");
    exprs.push_back("10%4+7");

    for (std::size_t k = 0; k < exprs.size(); ++k) {
        const std::string& expr = exprs[k];
        ConversionResult postConv = infixToPostfix(expr);
        ConversionResult preConv = infixToPrefix(expr);

        check(postConv.ok, "RoundTrip: infixToPostfix ok for " + expr);
        check(preConv.ok, "RoundTrip: infixToPrefix ok for " + expr);

        if (postConv.ok && preConv.ok) {
            EvalResult postEval = evaluatePostfix(postConv.output, vars);
            EvalResult preEval = evaluatePrefix(preConv.output, vars);

            check(postEval.ok, "RoundTrip: evaluatePostfix ok for " + expr);
            check(preEval.ok, "RoundTrip: evaluatePrefix ok for " + expr);

            if (postEval.ok && preEval.ok) {
                checkNear(postEval.value, preEval.value, "RoundTrip MATCH: " + expr + " -> " + formatNumber(postEval.value));
            }
        }
    }
}

} // namespace

int main() {
    std::cout << "============================================================\n";
    std::cout << "  EXPRESSION EVALUATOR & STACK ADT - TEST RUNNER\n";
    std::cout << "  Delhi Technical Campus (GGSIPU) - Data Structures Lab\n";
    std::cout << "============================================================\n";

    testArrayStack();
    testLinkedStack();
    testTokenizer();
    testParenChecker();
    testInfixToPostfix();
    testInfixToPrefix();
    testPostfixEval();
    testPrefixEval();
    testRoundTrip();

    std::cout << "\n============================================================\n";
    std::cout << "Passed: " << passed << "  Failed: " << failed << "\n";
    std::cout << "============================================================\n";

    return (failed > 0) ? 1 : 0;
}
