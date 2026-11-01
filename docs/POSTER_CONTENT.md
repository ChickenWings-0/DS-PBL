# Poster Content

> Suggested layout: A2 or A1 portrait, 3 columns. Title band across the top, Demo/Results band in the middle, Conclusion + QR code at the bottom. Keep body text at 24 pt or larger and use one accent colour. Put expressions in a monospace font.

---

## HEADER BAND

**EXPRESSION EVALUATOR & BALANCED-PARENTHESES CHECKER USING STACK OPERATIONS**

Aksh Rastogi (02518015625) · Bhavya Porwal (02318015625)
B.Tech CSE-AIML · Sem 3 · Section G · Group G1 · Data Structures Lab
Faculty: Akash Bhardwaj · Delhi Technical Campus (GGSIPU)

*[College logo left · stack icon right]*

---

## COLUMN 1

### Introduction
Compilers and calculators can't evaluate `a + b * c` directly. They check the brackets, convert to **postfix** or **prefix**, then evaluate. Every one of these steps uses a **stack** (Last-In-First-Out).

### Objectives
- Stack ADT written from scratch: **array** and **linked list**
- Bracket check for `() {} []` with position and type of error
- Infix → postfix and prefix (precedence + associativity)
- Postfix and prefix evaluation with error handling
- Step-by-step **trace mode**

### Stack ADT
```
   ArrayStack            LinkedStack
  ┌───┐ ← top          head → [c] → [b] → [a] → ∅
  │ c │
  │ b │          both implement StackADT<T>
  │ a │          push · pop · peek · isEmpty
  └───┘          isFull · size · display
```
Overflow/underflow → exceptions

---

## COLUMN 2

### Methodology
1. **Tokenize**: `12`, `x1` and `3.5` each stay one token
2. **Validate**: missing operands, unary ops, empty brackets
3. **Bracket check**: push openers, match closers
4. **Convert**: operator stack + precedence table
5. **Evaluate**: operand stack, safe arithmetic

| Op | Prec | Assoc |
|---|---|---|
| `^` | 3 | right |
| `* / %` | 2 | left |
| `+ -` | 1 | left |

### Key Algorithms
- **Postfix:** pop while prec(top) > prec(op), or equal and op is left-assoc
- **Prefix:** reverse tokens → swap brackets → flipped rule → reverse
- **Evaluation:** postfix → first pop is the *right* operand; prefix → first pop is the *left* operand

*[Small flowchart: Input → Brackets? → Postfix / Prefix → Evaluate → Match ✔]*

---

## COLUMN 3

### Demo / Results
**Input:** `a+b*(c-d)/e` (a=10, b=4, c=9, d=3, e=2)

| Stage | Output |
|---|---|
| Brackets | Balanced ✔ |
| Postfix | `a b c d - * e / +` |
| Prefix | `+ a / * b - c d e` |
| Value | **22** (both) ✔ |

*[Screenshot: trace table of `(A+B)*C`]*

**Error detection:**
`{a+(b*c})` → mismatch at position 8, expected `)`
`5 0 /` (postfix) → Division by zero
`- 5` (prefix) → Insufficient operands

### Testing
**140 / 140** automated checks pass
Includes the round-trip test: postfix value = prefix value

### Complexity
Stack operations: **O(1)** · All algorithms: **O(n)** time and space

---

## FOOTER BAND

**Conclusion:** One simple data structure handles bracket matching, notation conversion and evaluation, all in linear time.
**Future scope:** unary minus · functions · expression trees · GUI animation
**Contribution:** Aksh ~60% (Stack ADT, postfix, menu, build) · Bhavya ~40% (bracket checker, prefix, tests)

*[QR code → github.com/ChickenWings-0/DS-PBL]*
