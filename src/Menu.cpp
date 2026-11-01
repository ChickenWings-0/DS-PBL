// =============================================================================
// Menu.cpp
// Console menu: reads and validates user input, calls the algorithm modules,
// prints results and (when trace mode is ON) the step-by-step trace table.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#include "Menu.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
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
#include "TraceTable.h"

namespace {

bool traceMode = false;   // toggled by menu option 8

// ---------------------------------------------------------------------------
// Input helpers. Everything is read line by line with std::getline so that a
// bad entry never leaves junk in the input buffer.
// ---------------------------------------------------------------------------

std::string trim(const std::string& s) {
    std::size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    std::size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

// Reads one trimmed line. Exits cleanly if input is closed (Ctrl+Z / Ctrl+D).
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
    return trim(line);
}

// Re-prompts until the user enters something non-empty.
std::string readExpression(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt);
        if (!line.empty()) return line;
        std::cout << "  [!] Expression cannot be empty. Try again.\n";
    }
}

// Re-prompts until a whole number in [minValue, maxValue] is entered.
long readInt(const std::string& prompt, long minValue, long maxValue) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t used = 0;
            long value = std::stol(line, &used);
            if (used == line.size() && value >= minValue && value <= maxValue) {
                return value;
            }
        } catch (const std::exception&) {
            // not a number / out of range -> fall through to the message
        }
        std::cout << "  [!] Enter a whole number between " << minValue
                  << " and " << maxValue << ".\n";
    }
}

// Re-prompts until a valid real number is entered.
double readDouble(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t used = 0;
            double value = std::stod(line, &used);
            if (used == line.size() && std::isfinite(value)) return value;
        } catch (const std::exception&) {
        }
        std::cout << "  [!] Enter a valid number (e.g. 5, -2, 3.75).\n";
    }
}

// Asks for the value of every variable that appears in the expression.
VarTable askVariableValues(const std::string& expr) {
    VarTable vars;
    std::vector<std::string> names = extractVariables(expr);
    if (!names.empty()) {
        std::cout << "  Variables found: " << joinTokens(names) << "\n";
    }
    for (std::size_t i = 0; i < names.size(); ++i) {
        vars[names[i]] = readDouble("  Enter value of " + names[i] + ": ");
    }
    return vars;
}

void waitForEnter() {
    readLine("\nPress Enter to continue...");
}

// ---------------------------------------------------------------------------
// Output helpers
// ---------------------------------------------------------------------------

void printTitle(const std::string& title) {
    std::cout << "\n------------------------------------------------------------\n"
              << "  " << title << "\n"
              << "------------------------------------------------------------\n";
}

// Prints the trace table only when trace mode is ON and rows were recorded.
void printTrace(const TraceTable& table, const std::string& caption) {
    if (traceMode && !table.empty()) {
        std::cout << "\n  Trace - " << caption << ":\n";
        table.print();
    }
}

// Returns the trace pointer to pass to an algorithm (nullptr when trace is OFF).
TraceTable* traceArg(TraceTable& table) {
    return traceMode ? &table : nullptr;
}

// Prints the expression with a '^' under the given 1-based position.
void printCaret(const std::string& expr, int position) {
    if (position < 1 || position > static_cast<int>(expr.size())) return;
    std::cout << "    " << expr << "\n"
              << "    " << std::string(position - 1, ' ') << "^\n";
}

// ---------------------------------------------------------------------------
// Menu actions
// ---------------------------------------------------------------------------

// Option 1
void doCheckBrackets() {
    printTitle("1. BALANCED BRACKETS CHECK  ( ) { } [ ]");
    std::string expr = readExpression("Enter expression: ");

    TraceTable table;
    ParenResult r = checkBalanced(expr, traceArg(table));
    printTrace(table, "bracket matching");

    std::cout << "\n  Result: " << (r.balanced ? "[OK] " : "[ERROR] ") << r.message << "\n";
    if (!r.balanced) printCaret(expr, r.position);
}

// Option 2
void doInfixToPostfix() {
    printTitle("2. INFIX -> POSTFIX");
    std::string expr = readExpression("Enter infix expression: ");

    TraceTable table;
    ConversionResult r = infixToPostfix(expr, traceArg(table));
    printTrace(table, "infix to postfix");

    if (r.ok) std::cout << "\n  Postfix: " << r.output << "\n";
    else      std::cout << "\n  [ERROR] " << r.error << "\n";
}

// Option 3
void doInfixToPrefix() {
    printTitle("3. INFIX -> PREFIX");
    std::string expr = readExpression("Enter infix expression: ");

    TraceTable table;
    ConversionResult r = infixToPrefix(expr, traceArg(table));
    printTrace(table, "infix to prefix");

    if (r.ok) std::cout << "\n  Prefix: " << r.output << "\n";
    else      std::cout << "\n  [ERROR] " << r.error << "\n";
}

// Options 4 and 5 share this: isPostfix selects which evaluator to call.
void doEvaluate(bool isPostfix) {
    printTitle(isPostfix ? "4. EVALUATE POSTFIX" : "5. EVALUATE PREFIX");
    std::cout << "  Separate numbers/variables with spaces, e.g. "
              << (isPostfix ? "12 8 + 3 *" : "* + 12 8 3") << "\n";
    std::string expr = readExpression("Enter expression: ");
    VarTable vars = askVariableValues(expr);

    TraceTable table;
    EvalResult r = isPostfix ? evaluatePostfix(expr, vars, traceArg(table))
                             : evaluatePrefix(expr, vars, traceArg(table));
    printTrace(table, isPostfix ? "postfix evaluation" : "prefix evaluation");

    if (r.ok) std::cout << "\n  Result = " << formatNumber(r.value) << "\n";
    else      std::cout << "\n  [ERROR] " << r.error << "\n";
}

// Option 6: bracket check -> both conversions -> evaluate both -> compare.
void doPipeline() {
    printTitle("6. FULL PIPELINE");
    std::string expr = readExpression("Enter infix expression: ");

    // Step 1: brackets
    TraceTable parenTable;
    ParenResult pr = checkBalanced(expr, traceArg(parenTable));
    printTrace(parenTable, "bracket matching");
    std::cout << "\n  Step 1  Brackets : " << pr.message << "\n";
    if (!pr.balanced) {
        printCaret(expr, pr.position);
        std::cout << "  Pipeline stopped: fix the brackets first.\n";
        return;
    }

    // Step 2 and 3: conversions
    TraceTable postTable;
    ConversionResult post = infixToPostfix(expr, traceArg(postTable));
    printTrace(postTable, "infix to postfix");
    std::cout << "  Step 2  Postfix  : " << (post.ok ? post.output : "[ERROR] " + post.error) << "\n";

    TraceTable preTable;
    ConversionResult pre = infixToPrefix(expr, traceArg(preTable));
    printTrace(preTable, "infix to prefix");
    std::cout << "  Step 3  Prefix   : " << (pre.ok ? pre.output : "[ERROR] " + pre.error) << "\n";

    if (!post.ok && !pre.ok) {
        std::cout << "  Pipeline stopped: the expression could not be converted.\n";
        return;
    }

    // Step 4: variable values (asked once, used by both evaluators)
    std::cout << "  Step 4  Values\n";
    VarTable vars = askVariableValues(expr);
    if (vars.empty()) std::cout << "          (no variables)\n";

    // Step 5: evaluate both forms
    EvalResult postVal, preVal;
    postVal.ok = preVal.ok = false;
    if (post.ok) {
        TraceTable t;
        postVal = evaluatePostfix(post.output, vars, traceArg(t));
        printTrace(t, "postfix evaluation");
        std::cout << "  Step 5a Postfix value: "
                  << (postVal.ok ? formatNumber(postVal.value) : "[ERROR] " + postVal.error) << "\n";
    }
    if (pre.ok) {
        TraceTable t;
        preVal = evaluatePrefix(pre.output, vars, traceArg(t));
        printTrace(t, "prefix evaluation");
        std::cout << "  Step 5b Prefix value : "
                  << (preVal.ok ? formatNumber(preVal.value) : "[ERROR] " + preVal.error) << "\n";
    }

    if (postVal.ok && preVal.ok) {
        bool same = formatNumber(postVal.value) == formatNumber(preVal.value);
        std::cout << "  Check: postfix and prefix results "
                  << (same ? "MATCH" : "DO NOT MATCH") << "\n";
    }
}

// Option 7: interactive Stack ADT playground (both implementations).
void doStackPlayground() {
    printTitle("7. STACK ADT PLAYGROUND (int stack)");
    std::cout << "  1. Array-based stack\n"
              << "  2. Linked-list-based stack\n";
    long type = readInt("Choose implementation: ", 1, 2);

    // A base-class pointer lets the same code drive either implementation.
    std::unique_ptr<StackADT<int> > stack;
    std::string name;
    if (type == 1) {
        long cap = readInt("Capacity (1-1000): ", 1, 1000);
        stack.reset(new ArrayStack<int>(static_cast<int>(cap)));
        name = "ArrayStack (capacity " + std::to_string(cap) + ")";
    } else {
        long cap = readInt("Max size (0 = unlimited, up to 1000): ", 0, 1000);
        stack.reset(new LinkedStack<int>(static_cast<int>(cap)));
        name = "LinkedStack (" + (cap == 0 ? std::string("unlimited") : "max " + std::to_string(cap)) + ")";
    }

    while (true) {
        std::cout << "\n  [" << name << "]\n"
                  << "  1. push   2. pop   3. peek   4. isEmpty   5. isFull\n"
                  << "  6. size   7. display        0. back to main menu\n";
        long op = readInt("  Operation: ", 0, 7);
        if (op == 0) return;

        // Overflow / underflow are reported, never crash the program.
        try {
            switch (op) {
            case 1: {
                long v = readInt("  Value to push: ", -1000000000L, 1000000000L);
                stack->push(static_cast<int>(v));
                std::cout << "  Pushed " << v << ".\n";
                break;
            }
            case 2: std::cout << "  Popped " << stack->pop() << ".\n"; break;
            case 3: std::cout << "  Top = " << stack->peek() << "\n"; break;
            case 4: std::cout << "  isEmpty = " << (stack->isEmpty() ? "true" : "false") << "\n"; break;
            case 5: std::cout << "  isFull = " << (stack->isFull() ? "true" : "false") << "\n"; break;
            case 6: std::cout << "  size = " << stack->size() << "\n"; break;
            case 7: std::cout << "  "; stack->display(); break;
            }
        } catch (const StackOverflowError& e) {
            std::cout << "  [OVERFLOW] " << e.what() << "\n";
        } catch (const StackUnderflowError& e) {
            std::cout << "  [UNDERFLOW] " << e.what() << "\n";
        }
    }
}

void printMenu() {
    std::cout << "\n==========================================================\n"
              << "   EXPRESSION EVALUATOR & BALANCED-PARENTHESES CHECKER\n"
              << "          (Stack ADT - Array & Linked List)\n"
              << "==========================================================\n"
              << " 1. Check balanced brackets\n"
              << " 2. Convert infix  -> postfix\n"
              << " 3. Convert infix  -> prefix\n"
              << " 4. Evaluate postfix expression\n"
              << " 5. Evaluate prefix expression\n"
              << " 6. Full pipeline (infix -> postfix + prefix -> evaluate both)\n"
              << " 7. Stack ADT playground (array / linked list)\n"
              << " 8. Toggle trace mode            [currently: "
              << (traceMode ? "ON" : "OFF") << "]\n"
              << " 0. Exit\n"
              << "----------------------------------------------------------\n";
}

} // namespace

void runMenu() {
    while (true) {
        printMenu();
        long choice = readInt("Enter choice: ", 0, 8);

        switch (choice) {
        case 0:
            std::cout << "Goodbye!\n";
            return;
        case 1: doCheckBrackets(); break;
        case 2: doInfixToPostfix(); break;
        case 3: doInfixToPrefix(); break;
        case 4: doEvaluate(true); break;
        case 5: doEvaluate(false); break;
        case 6: doPipeline(); break;
        case 7: doStackPlayground(); break;
        case 8:
            traceMode = !traceMode;
            std::cout << "  Trace mode is now " << (traceMode ? "ON" : "OFF") << ".\n";
            break;
        }

        if (choice != 7 && choice != 8) waitForEnter();
    }
}
