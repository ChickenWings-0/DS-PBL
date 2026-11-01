# Viva Q&A (26 questions)

> Short answers to say out loud. **[A]** = Aksh answers, **[B]** = Bhavya answers, **[Both]** = either.

---

## 1. Stack ADT — [A]

**Q1. What is a stack? Why is it called LIFO?**
A linear data structure where insertion and deletion happen only at one end, the top. The **L**ast element **I**n is the **F**irst **O**ut, like a pile of plates.

**Q2. What is an ADT?**
An Abstract Data Type defines *what* operations exist (push, pop, peek…), not *how* they are implemented. Our `StackADT<T>` is the ADT. `ArrayStack` and `LinkedStack` are two implementations of it.

**Q3. Difference between your array and linked-list stacks?**
The array stack uses one contiguous block of fixed capacity and a `top` index. It's fast and cache-friendly but can overflow. The linked stack allocates a node per element with `head` as the top. It grows dynamically, but uses extra memory for the `next` pointers. Both run push and pop in O(1).

**Q4. What are overflow and underflow? How do you handle them?**
Overflow is pushing onto a full stack. Underflow is popping or peeking from an empty one. We throw `StackOverflowError` / `StackUnderflowError` (derived from `std::runtime_error`), and the menu catches them and prints a message instead of crashing.

**Q5. When can a linked stack overflow?**
When the user sets a maximum size (to demonstrate overflow), or when `new` fails (`std::bad_alloc`, out of memory). We convert that failure into `StackOverflowError`.

**Q6. Why templates? Why a pure virtual base class?**
Templates let one stack class hold `char`, `std::string`, `double` or `int`. We use all four. The abstract base class gives runtime polymorphism: the playground uses a `StackADT<int>*` pointer that can point to either implementation.

**Q7. Why did you delete the copy constructor?**
Both stacks own heap memory. A default (shallow) copy would make two objects share one array or node chain, and both destructors would free it, causing a double delete. Deleting the copy constructor prevents that.

**Q8. Why not `std::stack`?**
The aim of a DS lab is to implement the structure ourselves. Writing it by hand shows that we understand the `top` index, node linking, memory management and the error cases.

## 2. Tokenizer & validation — [A]

**Q9. Why tokenize instead of working on characters?**
So multi-digit numbers (`123`), decimals (`3.5`) and variables (`x1`) are handled as single units. Working character by character, `12` would be treated as `1` and `2`.

**Q10. What input errors do you detect?**
Invalid characters, malformed numbers, missing operands (`a+*b`), missing operators (`3 4`), empty brackets `()`, mismatched or unclosed brackets, and unary minus. Each error message includes its position.

**Q11. Why is unary minus not supported?**
`-` would then have two meanings (binary and unary) with different precedence, which complicates both conversions. We reject it with a clear message suggesting `(0-x)`. Supporting it is listed as future scope.

## 3. Bracket checker — [B]

**Q12. Why is a stack right for bracket matching?**
The most recently opened bracket must be closed first. That is exactly LIFO order.

**Q13. What are the three error cases?**
(1) A closing bracket when the stack is empty: **unexpected closing**. (2) A closing bracket whose type doesn't match the top: **mismatch**. (3) Brackets left on the stack at the end: **unclosed opening**.

**Q14. How do you report the position of an unclosed bracket?**
We push a struct `{symbol, position}` instead of just the character, so we know where each opening bracket was. For an unclosed bracket we report the top of the stack, which is the innermost unclosed one.

**Q15. Is counting brackets enough instead of a stack?**
No. A counter can't tell `([)]` from `([])`, because the counts are equal in both. Only a stack checks the nesting order and the bracket type.

## 4. Infix → postfix / prefix — [A] postfix, [B] prefix

**Q16. Explain infix → postfix in one line.**
Operands go straight to the output. Operators wait on a stack, and an incoming operator first pops every operator that must be applied before it (higher precedence, or equal precedence and left-associative). A closing bracket pops everything back to its opening bracket.

**Q17. How does associativity affect the algorithm?**
For left-associative operators, an equal-precedence operator on the stack is popped first: `100-20-30 → 100 20 - 30 -`. For right-associative `^` it is not popped: `2^3^2 → 2 3 2 ^ ^`, which means `2^(3^2) = 512`.

**Q18. How is infix → prefix done?**
Reverse the token list, swap `(` and `)`, run the postfix algorithm with the associativity rule flipped, then reverse the result. Example: `(A+B)*C` → reversed `C*(B+A)` → `C B A + *` → `* + A B C`.

**Q19. Why does the associativity rule flip for prefix?**
Reversing the expression mirrors the grouping. To keep `100-20-30` meaning `(100-20)-30`, left-associative operators must stay on the stack on equal precedence in the reversed scan, and right-associative `^` must pop.

**Q20. Why reverse tokens and not the string?**
Reversing the string would turn `12` into `21` and `x1` into `1x`. Reversing the token vector keeps each number and name intact.

## 5. Evaluation — [A] postfix, [B] prefix

**Q21. How is postfix evaluated? Why is the first pop the right operand?**
Scan left to right: push operands. On an operator, pop twice and push the result. The right operand was pushed last, so it comes off first. For `10 4 -` we pop 4 (right), then 10 (left): 10 − 4 = 6.

**Q22. How is prefix evaluation different?**
Scan right to left. The first value popped is the left operand. For `- 10 4` we push 4, then 10, so popping gives 10 (left), then 4 (right): 6.

**Q23. Which errors can evaluation detect?**
Division or modulo by zero, `%` on non-integers, an operator without two operands (`5 +`), operands left over (`5 6 7 +`), undefined variables, brackets in postfix/prefix, and non-finite results such as `10^1000`.

## 6. Complexity, design & testing — [Both]

**Q24. What is the time and space complexity?**
Each stack operation is O(1). Every algorithm is O(n) time and O(n) space, because each token is pushed and popped at most once.

**Q25. How did you test it?**
Bhavya wrote a 140-check test runner in 9 groups:

- both stacks
- the tokenizer
- the bracket checker
- both conversions
- both evaluations
- a round-trip test that checks postfix and prefix of the same expression give the same value

All 140 pass. We also tested the menu manually with invalid input.

**Q26. Where are stacks used in the real world?**
Compilers (expression parsing, syntax checking), function call stacks and recursion, undo/redo in editors, browser back buttons, and calculator apps. IDEs use bracket-matching highlighting.
