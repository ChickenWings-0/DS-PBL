# SPLIT — Work Division (≈ 60 / 40)

Team of 2, no lead. Each member owns their files completely. They must be able to explain every line of them in the viva.

| Member | Enrollment No. | Share |
|---|---|---|
| Aksh Rastogi | 02518015625 | ~60% |
| Bhavya Porwal | 02318015625 | ~40% |

## 1. Feature ownership

| # | Feature (from brief) | Owner |
|---|---|---|
| 1 | Stack ADT: abstract interface, array-based template, linked-list template, overflow/underflow | **Aksh** |
| 2 | Balanced-brackets checker `()`, `{}`, `[]` with position and type of mismatch | **Bhavya** |
| 3 | Infix → postfix (precedence, associativity, multi-digit numbers, variables) | **Aksh** |
| 4 | Infix → prefix | **Bhavya** |
| 5a | Postfix evaluation (division by zero, invalid expressions) | **Aksh** |
| 5b | Prefix evaluation (same error handling) | **Bhavya** |
| 6 | Trace mode: `TraceTable` class + menu toggle | **Aksh** (infrastructure). Each member fills trace rows inside their own algorithms |
| 7a | Input validation: tokenizer + infix validator + menu input checks | **Aksh** |
| 7b | Test file (`tests/test_main.cpp`) + `sample_inputs.txt` | **Bhavya** |
| 7c | README.md, Makefile, .gitignore, main menu and integration | **Aksh** |

## 2. File ownership

| File | Owner | Est. lines |
|---|---|---|
| `include/StackADT.h` | Aksh | 40 |
| `include/ArrayStack.h` | Aksh | 110 |
| `include/LinkedStack.h` | Aksh | 120 |
| `include/ExprCommon.h` + `src/ExprCommon.cpp` | Aksh | 230 |
| `include/TraceTable.h` + `src/TraceTable.cpp` | Aksh | 80 |
| `include/InfixToPostfix.h` + `src/InfixToPostfix.cpp` | Aksh | 100 |
| `include/PostfixEvaluator.h` + `src/PostfixEvaluator.cpp` | Aksh | 90 |
| `include/Menu.h` + `src/Menu.cpp` + `src/main.cpp` | Aksh | 280 |
| `Makefile`, `README.md`, `.gitignore` | Aksh | — |
| **Aksh subtotal** | | **≈ 1050 (≈ 61%)** |
| `src/ParenChecker.cpp` | Bhavya | 110 |
| `src/InfixToPrefix.cpp` | Bhavya | 120 |
| `src/PrefixEvaluator.cpp` | Bhavya | 100 |
| `tests/test_main.cpp` | Bhavya | 300 |
| `sample_inputs.txt` | Bhavya | 60 |
| **Bhavya subtotal** | | **≈ 690 (≈ 39%)** |

Headers `ParenChecker.h`, `InfixToPrefix.h`, `PrefixEvaluator.h` are written by Aksh in Phase 2, copied **exactly** from the signatures agreed in PLAN.md §5. Bhavya implements against them and does not edit them. The design of her result structs (`ParenResult`, `ParenStatus`) is credited to her in the report, because they describe her module.

## 3. Shared code: who uses what

| Bhavya's code uses (read-only) | Provided by |
|---|---|
| `LinkedStack<T>` (`push`, `pop`, `peek`, `isEmpty`, `size`, `toString`) | Aksh |
| `tokenize`, `validateInfix`, `isOperator`, `precedence`, `isRightAssociative`, `matchingOpen`, `matchingClose`, `applyOperator`, `formatNumber`, `joinTokens` | Aksh (`ExprCommon`) |
| `TraceTable::setHeaders`, `TraceTable::addRow` | Aksh |

| Aksh's code uses | Provided by |
|---|---|
| `checkBalanced`, `infixToPrefix`, `evaluatePrefix` (called from Menu.cpp) | Bhavya |

## 4. Viva responsibility

- **Aksh:** what a stack is, array vs linked-list implementation, overflow/underflow, templates and the abstract base class (polymorphism), tokenizing, precedence/associativity, infix→postfix, postfix evaluation, menu flow, build system.
- **Bhavya:** bracket matching and its error cases, infix→prefix (reverse + swap + reverse), why associativity flips in the prefix algorithm, prefix evaluation (operand order), the test strategy and test cases.
- **Both:** time/space complexity (all O(n)), why a stack (LIFO) suits these problems, real-world uses (compilers, calculators, editors).

## 5. Git workflow

1. Aksh pushes Phase 2 (all headers, his modules, stubs) to `main` on GitHub and adds Bhavya as a collaborator.
2. Bhavya clones, then runs `git checkout -b bhavya-modules`, replaces her 3 stub `.cpp` files, writes the tests and `sample_inputs.txt`, and commits under **her own** git name/email (so GitHub shows her contribution).
3. Bhavya pushes the branch and opens a Pull Request. Aksh merges it.
4. Phase 3: integration check, tests, docs.
