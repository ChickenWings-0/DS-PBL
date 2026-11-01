# Expression Evaluator / Balanced-Parentheses Checker using Stack Operations

A C++17 console application that shows the **Stack ADT**, written from scratch in two versions (array-based and linked-list-based). The stack is used to check balanced brackets, convert infix expressions to postfix and prefix, and evaluate them. A step-by-step trace table shows how the stack changes at every step.

> **Data Structures Lab — Project Based Learning (PBL)**
> B.Tech CSE-AIML, Semester 3 · Section G · Group G1
> Delhi Technical Campus (GGSIPU) · Faculty: **Akash Bhardwaj**

## Team

| Member | Enrollment No. | Contribution |
|---|---|---|
| Aksh Rastogi | 02518015625 | Stack ADT (array + linked list), tokenizer & validator, infix → postfix, postfix evaluation, trace table, menu & integration, Makefile, README (~60%) |
| Bhavya Porwal | 02318015625 | Balanced-brackets checker, infix → prefix, prefix evaluation, test suite, sample inputs (~40%) |

## Features

1. **Stack ADT from scratch** (no `std::stack`)
   - `StackADT<T>`: abstract interface with pure virtual functions
   - `ArrayStack<T>`: fixed-capacity array, `top` index
   - `LinkedStack<T>`: singly linked list, optional size limit
   - `push`, `pop`, `peek`, `isEmpty`, `isFull`, `size`, `display`
   - Overflow and underflow raise `StackOverflowError` and `StackUnderflowError`
2. **Balanced-brackets checker** for `()`, `{}`, `[]`. It reports the **position** and **type** of an error (unexpected closing bracket, mismatched pair, unclosed opening bracket).
3. **Infix → postfix** with precedence (`^` > `* / %` > `+ -`) and associativity (`^` right, others left). Supports multi-digit/decimal numbers and multi-letter variables.
4. **Infix → prefix** (reverse → convert → reverse).
5. **Postfix and prefix evaluation.** Variables are asked for at run time. Errors handled: division/modulo by zero, insufficient or too many operands, undefined variables, invalid characters.
6. **Trace mode**: prints a `Symbol | Stack | Output` table for every algorithm.
7. **Input validation**: every menu input and expression is checked, and errors are reported with the position where they occur.

## Project structure

```
include/   StackADT.h  ArrayStack.h  LinkedStack.h  ExprCommon.h  TraceTable.h
           InfixToPostfix.h  PostfixEvaluator.h  ParenChecker.h  InfixToPrefix.h
           PrefixEvaluator.h  Menu.h
src/       main.cpp  Menu.cpp  ExprCommon.cpp  TraceTable.cpp  InfixToPostfix.cpp
           PostfixEvaluator.cpp  ParenChecker.cpp  InfixToPrefix.cpp  PrefixEvaluator.cpp
tests/     test_main.cpp
sample_inputs.txt   Makefile   README.md
```

## Build and run

Requirements: `g++` with C++17 support (tested with MinGW GCC 6.3 on Windows), using only the standard library.

| Task | Windows (MinGW) | Linux |
|---|---|---|
| Build | `mingw32-make` | `make` |
| Build + run | `mingw32-make run` | `make run` |
| Run tests | `mingw32-make test` | `make test` |
| Clean | `mingw32-make clean` | `make clean` |

**Without make**, use a single command:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Menu.cpp src/ExprCommon.cpp src/TraceTable.cpp src/InfixToPostfix.cpp src/PostfixEvaluator.cpp src/ParenChecker.cpp src/InfixToPrefix.cpp src/PrefixEvaluator.cpp -o expr_eval
```

Then run `expr_eval.exe` (Windows) or `./expr_eval` (Linux).

Tests:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_main.cpp src/ExprCommon.cpp src/TraceTable.cpp src/InfixToPostfix.cpp src/PostfixEvaluator.cpp src/ParenChecker.cpp src/InfixToPrefix.cpp src/PrefixEvaluator.cpp -o test_runner
```

## Menu

```
 1. Check balanced brackets
 2. Convert infix  -> postfix
 3. Convert infix  -> prefix
 4. Evaluate postfix expression
 5. Evaluate prefix expression
 6. Full pipeline (infix -> postfix + prefix -> evaluate both)
 7. Stack ADT playground (array / linked list)
 8. Toggle trace mode            [currently: OFF]
 0. Exit
```

## Examples

| Infix | Postfix | Prefix | Value |
|---|---|---|---|
| `a+b*c` | `a b c * +` | `+ a * b c` | – |
| `(12+8)*3/4` | `12 8 + 3 * 4 /` | `/ * + 12 8 3 4` | 15 |
| `2^3^2` | `2 3 2 ^ ^` | `^ 2 ^ 3 2` | 512 |
| `100-20-30` | `100 20 - 30 -` | `- - 100 20 30` | 50 |
| `a+b*(c-d)/e` (a=10,b=4,c=9,d=3,e=2) | `a b c d - * e / +` | `+ a / * b - c d e` | 22 |

Trace of `(A+B)*C` → postfix (trace mode ON):

```
+--------+-------+-----------+
| Symbol | Stack | Postfix   |
+--------+-------+-----------+
| (      | (     |           |
| A      | (     | A         |
| +      | ( +   | A         |
| B      | ( +   | A B       |
| )      |       | A B +     |
| *      | *     | A B +     |
| C      | *     | A B + C   |
| (end)  |       | A B + C * |
+--------+-------+-----------+
```

Error examples:

| Input | Message |
|---|---|
| `a+*b` | `Operator '*' at position 3 is missing its left operand` |
| `(a+b]` | `Bracket mismatch: '(' at position 1 is closed by ']' at position 5` |
| `-5+2` | `Unary '-' at position 1 is not supported; write (0-x) instead` |
| postfix `5 0 /` | `Division by zero at position 5` |
| postfix `5 6 7 +` | `Too many operands: 2 values left on the stack` |

More demo inputs are in [sample_inputs.txt](sample_inputs.txt).

## Input rules

- Operands are numbers (`7`, `120`, `3.5`) or variables (`a`, `x1`, `total`).
- Operators are `+ - * / % ^`. `%` needs whole-number operands.
- Infix expressions can use `()`, `[]` and `{}`. Spaces are optional.
- In postfix/prefix input, **separate operands with spaces** (`12 8 +`, not `128+`).

## Limitations

- Unary operators (`-5`, `+x`) are not supported. Write `(0-5)` instead.
- No functions (`sin`, `max`) and no assignment.
- Values are stored as `double` (about 15 significant digits).

## Complexity

All algorithms run in **O(n)** time and **O(n)** extra space, where *n* is the number of tokens. Every stack operation is **O(1)**.
