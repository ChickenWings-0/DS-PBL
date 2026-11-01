# TEAMMATE TASKS — Bhavya Porwal (02318015625)

> **Standalone brief.** This file is all you need to do your part. You can give it to an AI coding tool as-is.

## 0. Project context (read first)

- **Project:** "Expression Evaluator / Balanced-Parentheses Checker using Stack Operations"
- **Course:** Data Structures Lab, B.Tech CSE-AIML, Sem 3, Delhi Technical Campus (GGSIPU), Section G, Group G1. Faculty: Akash Bhardwaj.
- **Team:** Aksh Rastogi (~60%) and Bhavya Porwal (~40%).
- **App:** C++17 console menu program. The stack is written by hand: **`std::stack` is forbidden**. Aksh has already written:
  - `ArrayStack<T>` and `LinkedStack<T>` (templates, same API)
  - a shared tokenizer/validator (`ExprCommon`)
  - a `TraceTable` printer
  - infix→postfix and postfix evaluation
  - the menu, which already calls your functions
- **Your job:** implement **3 modules + a test file + a sample-inputs file**. The menu already calls your functions. Today they are stubs that print `not implemented yet`. You replace the stub bodies, and nothing else in the project needs to change.

### Hard rules

1. **Do not edit any header (`include/*.h`)** or any file not listed in §1. The signatures are fixed so your code plugs in without changes.
2. **Compiler compatibility: GCC 6.3 (MinGW).** Write C++11/C++14-style code. **Do not use:** `<optional>`, `<variant>`, `<string_view>`, `<filesystem>`, structured bindings (`auto [a, b] = ...`), `if constexpr`, or `if (init; cond)`. It is compiled with `-std=c++17`.
3. Standard headers only. **No `std::stack`.** Use `LinkedStack<T>` from `include/LinkedStack.h` in all three of your modules.
4. Your functions must **not throw** to the caller and must **not print anything**. They return result structs. Menu.cpp prints the results and the trace table.
5. Code style: 4-space indent, `camelCase` names, `std::` prefix (no `using namespace std;`), a short comment above each function and at each key step of the algorithm. **You must be able to explain every line in the viva.**
6. Commit under your own git name/email so GitHub credits you.

---

## 1. Your files

| File | What to do |
|---|---|
| `src/ParenChecker.cpp` | Replace the stub. Implement `checkBalanced`. |
| `src/InfixToPrefix.cpp` | Replace the stub. Implement `infixToPrefix`. |
| `src/PrefixEvaluator.cpp` | Replace the stub. Implement `evaluatePrefix`. |
| `tests/test_main.cpp` | Replace the stub. Write the test runner (spec in §6). |
| `sample_inputs.txt` | Create the demo input list (spec in §7). |

Headers you implement against (already in the repo, **do not modify**): `include/ParenChecker.h`, `include/InfixToPrefix.h`, `include/PrefixEvaluator.h`.

---

## 2. APIs you can use (written by Aksh, read-only)

### 2.1 Stack — `#include "LinkedStack.h"`

```cpp
template <typename T>
class LinkedStack : public StackADT<T> {
public:
    explicit LinkedStack(int maxSize = 0);   // 0 = unlimited  ← use the default
    void push(const T& value);               // throws StackOverflowError if full
    T    pop();                              // throws StackUnderflowError if empty
    T    peek() const;                       // throws StackUnderflowError if empty
    bool isEmpty() const;
    bool isFull() const;
    int  size() const;
    void display() const;                    // prints to std::cout — DON'T call it in your modules
    std::string toString() const;            // "x y z" bottom→top, "" if empty — use this for trace rows
};
```

- Exceptions `StackOverflowError` and `StackUnderflowError` are declared in `StackADT.h` (both derive from `std::runtime_error`). **Check `isEmpty()` before `pop()`/`peek()`** so they are never thrown in normal use.
- `toString()` uses `operator<<` on `T`. If you store a custom struct, you must define `operator<<` for it (see §3).

### 2.2 Shared helpers — `#include "ExprCommon.h"`

```cpp
enum class TokenType { Number, Variable, Operator, LeftParen, RightParen };

struct Token {
    TokenType   type;
    std::string text;      // "12", "x", "+", "(", "]" ...
    int         position;  // 1-based column in the original input
};

struct TokenizeResult   { bool ok; std::vector<Token> tokens; std::string error; };
struct ConversionResult { bool ok; std::string output; std::string error; };
struct EvalResult       { bool ok; double value;       std::string error; };
typedef std::map<std::string, double> VarTable;

TokenizeResult tokenize(const std::string& expr);
//   Skips whitespace. Number = digits with at most one '.', Variable = letter/'_' then
//   letters/digits/'_', Operator = + - * / % ^, ( [ { = LeftParen, ) ] } = RightParen.
//   Invalid character → ok=false, error like "Invalid character '$' at position 4".
//   "12+3" → [12][+][3]   (multi-digit numbers stay ONE token)

std::string validateInfix(const std::vector<Token>& tokens);   // "" = valid, else error text
bool isOperator(const std::string& s);           // + - * / % ^
int  precedence(const std::string& op);          // ^ → 3,  * / % → 2,  + - → 1,  else 0
bool isRightAssociative(const std::string& op);  // true only for "^"
char matchingOpen(char close);                   // ')'→'(' ']'→'[' '}'→'{'  else '\0'
char matchingClose(char open);                   // '('→')' '['→']' '{'→'}'  else '\0'
bool applyOperator(const std::string& op, double left, double right,
                   double& result, std::string& error);
//   false on: "Division by zero", "Modulo by zero",
//             "Modulo requires integer operands", "Result is not a finite number"
std::string formatNumber(double value);          // 12 → "12", 3.5 → "3.5"
std::string joinTokens(const std::vector<std::string>& parts);  // {"a","b","+"} → "a b +"
```

### 2.3 Trace table — `#include "TraceTable.h"`

```cpp
class TraceTable {
public:
    void setHeaders(const std::string& c1, const std::string& c2, const std::string& c3);
    void addRow(const std::string& symbol, const std::string& stack, const std::string& output);
    // print() exists, but the MENU calls it — you never call print().
};
```

**Trace convention:** every function receives `TraceTable* trace` (may be `nullptr`). If `trace != nullptr`, call `trace->setHeaders(...)` **once** at the start and then `trace->addRow(...)` for each step. Always guard with `if (trace)`.

---

## 3. Module A — Balanced-brackets checker

### Header (fixed): `include/ParenChecker.h`

```cpp
#include <string>
#include "TraceTable.h"

enum class ParenStatus { Balanced, UnexpectedClosing, MismatchedPair, UnclosedOpening };

struct ParenResult {
    bool        balanced;
    ParenStatus status;
    int         position;      // 1-based position of the offending bracket; -1 if balanced
    char        found;         // offending bracket character; '\0' if balanced
    char        expected;      // closing bracket that was expected; '\0' if none
    int         openPosition;  // position of the related opening bracket; -1 if none
    std::string message;       // full human-readable message
};

ParenResult checkBalanced(const std::string& expr, TraceTable* trace = nullptr);
```

### Algorithm

```
stack ← empty LinkedStack of {symbol, position}
for i from 0 to len-1:            (position = i + 1)
    c ← expr[i]
    if c is ( [ {   → push {c, position}; trace "push"
    else if c is ) ] }:
        if stack empty          → return UnexpectedClosing
        top ← peek
        if top.symbol != matchingOpen(c) → return MismatchedPair
        pop; trace "pop - matched"
    else ignore (no trace row)
if stack not empty → top ← peek; return UnclosedOpening (report the TOP, i.e. innermost unclosed)
return Balanced
```

Suggested element type, defined **inside `ParenChecker.cpp`**:

```cpp
struct OpenBracket { char symbol; int position; };
// Needed because LinkedStack::toString() uses operator<<. Print just the symbol.
std::ostream& operator<<(std::ostream& out, const OpenBracket& b) { return out << b.symbol; }
```

### Exact result fields and messages (the tests check these)

| Status | `position` | `found` | `expected` | `openPosition` | `message` |
|---|---|---|---|---|---|
| Balanced | -1 | `'\0'` | `'\0'` | -1 | `Balanced: all brackets are properly matched.` |
| UnexpectedClosing | closer pos | closer | `'\0'` | -1 | `Unexpected closing ')' at position 4: no matching opening bracket.` |
| MismatchedPair | closer pos | closer | `matchingClose(top)` | top pos | `Mismatch at position 5: found ']' but expected ')' to close '(' from position 1.` |
| UnclosedOpening | top pos | top symbol | `matchingClose(top)` | top pos | `Unclosed '(' at position 1: no matching closing bracket.` |

### Trace (only bracket characters produce rows)

Headers: `Symbol`, `Stack`, `Action`. Symbol column = `"<char> @<pos>"` e.g. `( @1`. Stack column = `stack.toString()` **after** the step. Action = `push` / `pop - matched` / `ERROR: <short reason>`. When the scan ends with an unclosed bracket, add a final row `(end)` | stack | `ERROR: unclosed '('`.

### Examples

| Input | balanced | status | message |
|---|---|---|---|
| `{[a+b]*(c-d)}` | true | Balanced | `Balanced: all brackets are properly matched.` |
| `(a+b]` | false | MismatchedPair | `Mismatch at position 5: found ']' but expected ')' to close '(' from position 1.` |
| `a+b)` | false | UnexpectedClosing | `Unexpected closing ')' at position 4: no matching opening bracket.` |
| `((a+b)` | false | UnclosedOpening | `Unclosed '(' at position 1: no matching closing bracket.` |
| `[(a+b)*c` | false | UnclosedOpening | `Unclosed '[' at position 1: no matching closing bracket.` |
| `{a+(b*c})` | false | MismatchedPair | `Mismatch at position 8: found '}' but expected ')' to close '(' from position 4.` |
| `abc` (no brackets) | true | Balanced | `Balanced: all brackets are properly matched.` |
| `` (empty) | true | Balanced | same as above (the menu rejects empty input before calling you) |

Trace for `(a+b]`:
```
| Symbol | Stack | Action                         |
| ( @1   | (     | push                           |
| ] @5   | (     | ERROR: expected ')' but found ']' |
```

---

## 4. Module B — Infix → Prefix

### Header (fixed): `include/InfixToPrefix.h`

```cpp
#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

ConversionResult infixToPrefix(const std::string& infix, TraceTable* trace = nullptr);
```

### Algorithm (reverse → modified postfix → reverse)

```
1. tr ← tokenize(infix);            if !tr.ok → return {false, "", tr.error}
2. err ← validateInfix(tr.tokens);  if err != "" → return {false, "", err}
3. Reverse the TOKEN vector (not the string! "12" must stay "12").
   Swap brackets: LeftParen ↔ RightParen, and swap the text: "(" ↔ ")", "[" ↔ "]", "{" ↔ "}".
4. stack ← LinkedStack<std::string>, out ← vector<string>
   for each token t:
       Number/Variable → out.push_back(t.text)
       LeftParen       → push(t.text)
       RightParen      → pop into out until the top is a LeftParen; pop that bracket (discard it)
       Operator o      → while stack not empty AND top is an operator AND
                               ( precedence(top) >  precedence(o)  OR
                                (precedence(top) == precedence(o) AND isRightAssociative(o)) ):
                               out.push_back(pop())
                         push(o)
       trace row: (t.text, stack.toString(), joinTokens(out))
5. Pop everything left into out.     trace row: ("(end)", "", joinTokens(out))
6. Reverse out.                      trace row: ("(reverse)", "", joinTokens(out))
7. return {true, joinTokens(out), ""}
```

**Why the associativity rule is flipped compared to postfix:** in the reversed expression, a left-associative operator must stay on the stack when it meets an equal-precedence operator. For example `100-20-30` must give `- - 100 20 30`, which means `(100-20)-30`. A right-associative `^` must pop. For example `2^3^2` must give `^ 2 ^ 3 2`, which means `2^(3^2)`.

Trace headers: `Symbol`, `Stack`, `Output (reversed)`.

### Examples (output is space-separated, single spaces, no leading/trailing space)

| Infix | Prefix |
|---|---|
| `a+b*c` | `+ a * b c` |
| `(A+B)*C` | `* + A B C` |
| `2^3^2` | `^ 2 ^ 3 2` |
| `100-20-30` | `- - 100 20 30` |
| `(12+8)*3/4` | `/ * + 12 8 3 4` |
| `10%4+7` | `+ % 10 4 7` |
| `{a+b}*[c-d]` | `* + a b - c d` |
| `a+b*(c^d-e)^(f+g*h)-i` | `- + a * b ^ - ^ c d e + f * g h i` |
| `x1 + total` | `+ x1 total` |
| `a+*b` | ok=false, error = validateInfix's message |
| `a+b$` | ok=false, error = `Invalid character '$' at position 4` |

Trace for `(A+B)*C` (after reverse + swap the tokens are `C * ( B + A )`):
```
| Symbol    | Stack | Output (reversed) |
| C         |       | C                 |
| *         | *     | C                 |
| (         | * (   | C                 |
| B         | * (   | C B               |
| +         | * ( + | C B               |
| A         | * ( + | C B A             |
| )         | *     | C B A +           |
| (end)     |       | C B A + *         |
| (reverse) |       | * + A B C         |
```

---

## 5. Module C — Prefix evaluation

### Header (fixed): `include/PrefixEvaluator.h`

```cpp
#include <string>
#include "ExprCommon.h"
#include "TraceTable.h"

EvalResult evaluatePrefix(const std::string& prefix, const VarTable& vars,
                          TraceTable* trace = nullptr);
```

### Algorithm

```
1. tr ← tokenize(prefix); if !tr.ok → error tr.error
   if tr.tokens is empty → error "Empty expression"
   if any token is a bracket → error "Brackets are not allowed in prefix expressions"
2. stack ← LinkedStack<double>
   scan tokens from LAST to FIRST:
       Number   → push(std::stod(text));                 trace action "push <n>"
       Variable → if not in vars → error "Undefined variable '<name>'"
                  push(vars.at(name));                    trace action "push <name>=<value>"
       Operator → if stack.size() < 2 → error "Insufficient operands for '<op>' at position <pos>"
                  left  ← pop()     ← FIRST pop is the LEFT operand (opposite of postfix!)
                  right ← pop()
                  if !applyOperator(op, left, right, result, err)
                        → error err + " at position <pos>"   e.g. "Division by zero at position 1"
                  push(result);                           trace action "<l> <op> <r> = <result>"
3. if stack.size() != 1 → error "Too many operands: <k> values left on the stack"
4. return {true, pop(), ""}
```

On error, return `{false, 0.0, message}`. If tracing, add a final row with action `ERROR: <message>` before returning. Use `formatNumber` for all numbers in trace text.

Trace headers: `Symbol`, `Stack`, `Action`.

### Examples

| Prefix | vars | Result |
|---|---|---|
| `+ 2 3` | – | 5 |
| `- 10 4` | – | 6 (order check: NOT -6) |
| `/ 20 4` | – | 5 |
| `* + 2 3 4` | – | 20 |
| `/ * + 12 8 3 4` | – | 15 |
| `^ 2 ^ 3 2` | – | 512 |
| `- - 100 20 30` | – | 50 |
| `% 10 4` | – | 2 |
| `/ 7 2` | – | 3.5 |
| `+ a * b c` | a=2, b=3, c=4 | 14 |
| `/ 5 0` | – | error `Division by zero at position 1` |
| `% 5 0` | – | error `Modulo by zero at position 1` |
| `+ 5` | – | error `Insufficient operands for '+' at position 1` |
| `+ 2 3 4` | – | error `Too many operands: 2 values left on the stack` |
| `+ 2 x` | (x missing) | error `Undefined variable 'x'` |
| `( + 2 3 )` | – | error `Brackets are not allowed in prefix expressions` |
| `` (empty) | – | error `Empty expression` |

Trace for `* + 2 3 4`:
```
| Symbol | Stack | Action     |
| 4      | 4     | push 4     |
| 3      | 4 3   | push 3     |
| 2      | 4 3 2 | push 2     |
| +      | 4 5   | 2 + 3 = 5  |
| *      | 20    | 5 * 4 = 20 |
```

---

## 6. Test file — `tests/test_main.cpp`

A self-contained test runner with **no external libraries**. It is compiled **without** `main.cpp`/`Menu.cpp`, so it has its own `main()`.

Requirements:
- A tiny check helper, e.g.
  ```cpp
  static int passed = 0, failed = 0;
  void check(bool condition, const std::string& name) { ... prints "[PASS] name" or "[FAIL] name" ... }
  void checkEqual(const std::string& actual, const std::string& expected, const std::string& name);
  void checkNear(double actual, double expected, const std::string& name);  // fabs(diff) < 1e-9
  ```
- Group the tests into functions: `testArrayStack()`, `testLinkedStack()`, `testTokenizer()`, `testParenChecker()`, `testInfixToPostfix()`, `testInfixToPrefix()`, `testPostfixEval()`, `testPrefixEval()`, `testRoundTrip()`.
- At the end, print `Passed: X  Failed: Y`. **Return 1 if any test failed, else 0.**
- **At least 45 checks.** Use the example tables in §3–§5 plus the ones below.
- For validator/tokenizer errors, only check `ok == false` (and that `error` is non-empty). Don't compare exact validator text. Do compare exact messages for your own modules (§3, §5).

Cases for Aksh's modules to include:

| Area | Case | Expected |
|---|---|---|
| ArrayStack | capacity 3: push 1,2,3 → `isFull()` | true; 4th push throws `StackOverflowError` |
| ArrayStack | pop order after push 1,2,3 | 3, 2, 1; then pop throws `StackUnderflowError` |
| ArrayStack | `toString()` after push 1,2,3 | `"1 2 3"` |
| LinkedStack | maxSize 2: 3rd push | throws `StackOverflowError` |
| LinkedStack | empty `peek()` | throws `StackUnderflowError` |
| LinkedStack | unlimited: push 1..1000, `size()` | 1000, `isFull()` false |
| tokenize | `12+x1*3.5` | 5 tokens: `12`,`+`,`x1`,`*`,`3.5`; positions 1,3,4,6,7 |
| tokenize | `a+b$` | ok=false |
| infixToPostfix | `a+b*c` | `a b c * +` |
| infixToPostfix | `(A+B)*C` | `A B + C *` |
| infixToPostfix | `2^3^2` | `2 3 2 ^ ^` |
| infixToPostfix | `100-20-30` | `100 20 - 30 -` |
| infixToPostfix | `(12+8)*3/4` | `12 8 + 3 * 4 /` |
| infixToPostfix | `a+b*(c^d-e)^(f+g*h)-i` | `a b c d ^ e - f g h * + ^ * + i -` |
| infixToPostfix | `{a+b}*[c-d]` | `a b + c d - *` |
| infixToPostfix | `a+*b`, `(a+b`, `a b`, `()`, `-5` | ok=false each |
| evaluatePostfix | `2 3 4 * +` | 14 |
| evaluatePostfix | `5 1 2 + 4 * + 3 -` | 14 |
| evaluatePostfix | `12 8 + 3 * 4 /` | 15 |
| evaluatePostfix | `2 3 2 ^ ^` | 512 |
| evaluatePostfix | `10 4 -` | 6 |
| evaluatePostfix | `7 2 /` | 3.5 |
| evaluatePostfix | `a b c * +` with a=2,b=3,c=4 | 14 |
| evaluatePostfix | `5 0 /` | ok=false, error contains `Division by zero` |
| evaluatePostfix | `5 +` | ok=false |
| evaluatePostfix | `5 6 7 +` | ok=false |

**Round-trip test** (`testRoundTrip`): for each infix in `{"a+b*c", "(12+8)*3/4", "2^3^2", "100-20-30", "a+b*(c^d-e)^(f+g*h)-i", "10%4+7"}`, use vars a..i = 1..9. Check that `evaluatePostfix(infixToPostfix(x).output)` equals `evaluatePrefix(infixToPrefix(x).output)`.

Exception checks pattern:
```cpp
bool threw = false;
try { s.push(4); } catch (const StackOverflowError&) { threw = true; }
check(threw, "ArrayStack overflow throws");
```

---

## 7. `sample_inputs.txt`

A plain-text demo sheet used during the live demo and for screenshots. One section per menu option, each line `input  →  expected output`. At least:
- **Brackets (option 1):** 2 balanced, plus 1 each of mismatched / unexpected closing / unclosed.
- **Infix→postfix (2) and infix→prefix (3):** 6 cases including `2^3^2`, `100-20-30`, multi-digit, variables, mixed `{[()]}`.
- **Postfix eval (4) and prefix eval (5):** 4 valid, plus division by zero, insufficient operands, too many operands.
- **Full pipeline (6):** `a+b*(c-d)/e` with a=10,b=4,c=9,d=3,e=2 → postfix `a b c d - * e / +`, prefix `+ a / * b - c d e`, value 22.
- **Invalid input:** `a+*b`, `3 4`, `a+b$`, `-5+2`, empty line.

Top of the file: a 3-line header with project name, team and how to use the file.

---

## 8. Build and test

From the project root:

| Task | Windows (MinGW) | Linux |
|---|---|---|
| Build app | `mingw32-make` | `make` |
| Run app | `.\expr_eval.exe` | `./expr_eval` |
| Build + run tests | `mingw32-make test` | `make test` |
| Clean | `mingw32-make clean` | `make clean` |

Without make (single command):
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Menu.cpp src/ExprCommon.cpp src/TraceTable.cpp src/InfixToPostfix.cpp src/PostfixEvaluator.cpp src/ParenChecker.cpp src/InfixToPrefix.cpp src/PrefixEvaluator.cpp -o expr_eval

g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_main.cpp src/ExprCommon.cpp src/TraceTable.cpp src/InfixToPostfix.cpp src/PostfixEvaluator.cpp src/ParenChecker.cpp src/InfixToPrefix.cpp src/PrefixEvaluator.cpp -o test_runner
```

**Definition of done:**
1. Both commands compile with **zero warnings**.
2. `test_runner` prints `Failed: 0`.
3. Menu options 1, 3, 5 and 6 work, with trace mode on and off.
4. Every example in §3–§5 gives exactly the output shown.

## 9. Git steps

```bash
git clone <repo-url>
cd <repo>
git config user.name  "Bhavya Porwal"
git config user.email "<your github email>"
git checkout -b bhavya-modules
# ... implement, build, test ...
git add src/ParenChecker.cpp
git commit -m "Implement balanced-brackets checker with position/type error reporting"
git add src/InfixToPrefix.cpp
git commit -m "Implement infix to prefix conversion using LinkedStack"
git add src/PrefixEvaluator.cpp
git commit -m "Implement prefix evaluation with error handling"
git add tests/test_main.cpp
git commit -m "Add test runner covering stacks, conversions and evaluation"
git add sample_inputs.txt
git commit -m "Add sample inputs for demo"
git push -u origin bhavya-modules
# open a Pull Request into main on GitHub; Aksh merges it
```

## 10. Viva prep (your part)

Be ready to answer:
- Why is a stack the right structure for bracket matching? (The last bracket opened must be the first one closed: LIFO.)
- What are the three error cases, and how does your code detect each?
- Explain reverse → swap brackets → convert → reverse. Why does associativity flip?
- In prefix evaluation, why is the first popped value the LEFT operand?
- Time and space complexity of each function (all O(n) time, O(n) space).
- Why did you use `LinkedStack` and not `ArrayStack`? (No fixed capacity is needed. Size grows with the input.)
- How do your tests prove correctness? (Example cases, error cases, and the postfix-vs-prefix round-trip.)
