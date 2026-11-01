# PPT Content (13 slides)

> One block per slide: **Title**, **Bullets**, **Visual**, **Speaker** (who presents it). Keep each slide to 5 or fewer bullets. Use a clean dark-text-on-light theme and one accent colour, with a monospace font for code and expressions.

---

### Slide 1 — Title
**Title:** Expression Evaluator & Balanced-Parentheses Checker using Stack Operations
**Bullets:**
- Data Structures Lab — Project Based Learning
- Aksh Rastogi (02518015625) · Bhavya Porwal (02318015625)
- B.Tech CSE-AIML, Sem 3, Section G, Group G1
- Faculty: Akash Bhardwaj · Delhi Technical Campus (GGSIPU)

**Visual:** College logo top-left. A stack-of-plates icon or stylised `( [ { } ] )` on the right.
**Speaker:** Aksh

---

### Slide 2 — Problem Statement
**Bullets:**
- Humans write infix: `a + b * c`. Machines prefer postfix or prefix.
- Infix depends on precedence, associativity and brackets.
- Unbalanced brackets and invalid expressions must be caught with precise errors.
- Goal: one tool that validates, converts and evaluates expressions using a stack written from scratch.

**Visual:** `(12+8)*3/4` with three arrows to "Brackets OK ✔", "`12 8 + 3 * 4 /`", "= 15".
**Speaker:** Aksh

---

### Slide 3 — Objectives
**Bullets:**
- Stack ADT in two versions (array and linked list), with overflow/underflow handling
- Bracket checker for `() {} []` that reports position and type of error
- Infix → postfix and infix → prefix with precedence and associativity
- Postfix and prefix evaluation with safe error handling
- Step-by-step trace mode, input validation, automated tests

**Visual:** Six icon tiles, one per objective.
**Speaker:** Bhavya

---

### Slide 4 — Stack ADT
**Bullets:**
- LIFO: push and pop happen only at the top, both O(1)
- Abstract class `StackADT<T>` with pure virtual functions
- `ArrayStack<T>`: fixed array, `top` index; full when `top == capacity-1`
- `LinkedStack<T>`: nodes linked from `head`; optional size limit
- `StackOverflowError` / `StackUnderflowError` exceptions

**Visual:** Side-by-side drawing: array cells with a `top` arrow, and a linked list `head → [c] → [b] → [a] → null`. A small UML (base class with two children) underneath.
**Speaker:** Aksh

---

### Slide 5 — System Architecture
**Bullets:**
- Layer 1: Stack ADT (array + linked)
- Layer 2: tokenizer, validator, operator rules, trace table
- Layer 3: five algorithm modules, each returning a result struct
- Layer 4: console menu (input and display only)
- Header-first design: fixed interfaces let two people build in parallel

**Visual:** The module diagram from REPORT §4.1 (render the mermaid at mermaid.live and export as PNG).
**Speaker:** Aksh

---

### Slide 6 — Balanced-Bracket Checker
**Bullets:**
- Push opening brackets together with their position
- On a closing bracket: empty stack → **unexpected closing**; wrong type → **mismatch**
- At the end, a non-empty stack → **unclosed opening**
- Reports position, found and expected character; a caret `^` marks the spot

**Visual:** Screenshot **[Fig. 2]** (trace of `{a+(b*c})` with the caret).
**Speaker:** Bhavya

---

### Slide 7 — Infix → Postfix
**Bullets:**
- Operands go straight to the output; operators wait on the stack
- Pop while the top has higher precedence, or equal precedence and left-associative
- `^` is right-associative: `2^3^2 → 2 3 2 ^ ^` = 512
- The tokenizer keeps `12`, `3.5` and `x1` as single tokens

**Visual:** Screenshot **[Fig. 3]** (trace table of `(A+B)*C`).
**Speaker:** Aksh

---

### Slide 8 — Infix → Prefix
**Bullets:**
- Reverse the tokens (not the characters), then swap `(`↔`)`
- Run the operator-stack algorithm with the associativity rule flipped
- Reverse the output to get the prefix form
- `100-20-30 → - - 100 20 30`, `2^3^2 → ^ 2 ^ 3 2`

**Visual:** Three-step strip: `(A+B)*C` → reversed `C*(B+A)` → `C B A + *` → `* + A B C`. Or screenshot **[Fig. 4]**.
**Speaker:** Bhavya

---

### Slide 9 — Evaluation (Postfix & Prefix)
**Bullets:**
- Postfix: scan left→right; first pop = **right** operand
- Prefix: scan right→left; first pop = **left** operand
- Variables are asked for at run time (`a`, `x1`, `total`)
- Errors: division/modulo by zero, too few or too many operands, undefined variable

**Visual:** Screenshot **[Fig. 5]** (prefix trace of `* + 2 3 4`). Small inset of the stack after each step.
**Speaker:** Bhavya (prefix) / Aksh (postfix)

---

### Slide 10 — Live Demo: Full Pipeline
**Bullets:**
- Input `a+b*(c-d)/e` with a=10, b=4, c=9, d=3, e=2
- Brackets ✔ → postfix `a b c d - * e / +` → prefix `+ a / * b - c d e`
- Both evaluate to **22**, so the results MATCH
- Trace mode can be switched on to show every step

**Visual:** Screenshot **[Fig. 6]**. Switch to the live terminal here.
**Speaker:** Aksh (drives the demo), Bhavya (narrates)

---

### Slide 11 — Testing & Validation
**Bullets:**
- 140 automated checks in 9 groups: **140 / 140 pass**
- Round-trip test: eval(postfix) = eval(prefix) for every expression
- Invalid input caught with position: `a+*b`, `3 4`, `a+b$`, `-5`, `()`
- Overflow and underflow shown live in the Stack Playground

**Visual:** Screenshot **[Fig. 8]** (`test_runner` summary). A small bar chart of checks per group.
**Speaker:** Bhavya

---

### Slide 12 — Complexity & Contribution
**Bullets:**
- Every stack operation is O(1); every algorithm is O(n) time and O(n) space
- Each token is pushed and popped at most once
- Aksh (~60%): Stack ADT, tokenizer/validator, postfix modules, trace, menu, build
- Bhavya (~40%): bracket checker, prefix modules, test suite, sample inputs

**Visual:** Left: the complexity table. Right: a 60/40 split bar with each member's modules listed.
**Speaker:** Both

---

### Slide 13 — Conclusion, Future Scope & GitHub
**Bullets:**
- One data structure (the stack) solves matching, conversion and evaluation
- Writing the stack ourselves taught array vs linked trade-offs, templates and polymorphism
- Future: unary minus, functions, expression trees, GUI animation
- Code: github.com/ChickenWings-0/DS-PBL

**Visual:** QR code for the GitHub repo (generate from the URL). "Thank you — Questions?" footer.
**Speaker:** Aksh
