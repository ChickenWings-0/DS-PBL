# Screenshots To Take

Build first: `mingw32-make clean` then `mingw32-make`, then run `.\expr_eval.exe`.

Tips:
- Maximise the terminal window and use a font size of 16 or more.
- Crop each screenshot to the relevant part.
- Save them as `docs/screenshots/figN_name.png`.
- **Fig. numbers match REPORT_CONTENT.md and PPT_CONTENT.md.**

| Fig | File name | Menu steps (type exactly) | What must be visible |
|---|---|---|---|
| 1 | `fig1_menu.png` | Start the program | The full main menu with `[currently: OFF]` |
| 2 | `fig2_bracket_mismatch.png` | `8` (trace ON) → `1` → `{a+(b*c})` | Trace table (3 rows), the mismatch message, and the caret `^` under position 8 |
| 3 | `fig3_postfix_trace.png` | (trace ON) `2` → `(A+B)*C` | Postfix trace table ending in `(end)`, and `Postfix: A B + C *` |
| 4 | `fig4_prefix_trace.png` | (trace ON) `3` → `(A+B)*C` | Prefix trace with the `(end)` and `(reverse)` rows, and `Prefix: * + A B C` |
| 5 | `fig5_prefix_eval_trace.png` | (trace ON) `5` → `* + 2 3 4` | Evaluation trace (`2 + 3 = 5`, `5 * 4 = 20`) and `Result = 20` |
| 6 | `fig6_full_pipeline.png` | `8` (trace OFF) → `6` → `a+b*(c-d)/e` → values `10`, `4`, `9`, `3`, `2` | Steps 1–5 and `Check: postfix and prefix results MATCH` |
| 7a | `fig7a_div_zero.png` | `4` → `5 0 /` | `[ERROR] Division by zero at position 5` |
| 7b | `fig7b_invalid_infix.png` | `2` → `a+*b`, then `2` → `-5+2` | Both validation error messages |
| 7c | `fig7c_balanced.png` | `1` → `{[a+b]*(c-d)}` | `[OK] Balanced: all brackets are properly matched.` |
| 8 | `fig8_tests.png` | In the terminal: `mingw32-make test` | The end of the output, showing the last `[PASS]` lines and `Passed: 140  Failed: 0` |
| 9 | `fig9_overflow.png` | `7` → `1` (array) → capacity `2` → `1`,`5` → `1`,`6` → `1`,`7` → `7` | `Pushed 5`, `Pushed 6`, `[OVERFLOW] ...capacity 2`, and the display `Bottom -> [ 5 6 ] <- Top` |
| 10 | `fig10_underflow.png` | `7` → `2` (linked) → `0` (unlimited) → `2` (pop) → `3` (peek) | Both `[UNDERFLOW]` messages |
| 11 | `fig11_postfix_eval_vars.png` | `4` → `a b c * +` → values `2`, `3`, `4` | `Variables found: a b c` and `Result = 14` |
| 12 | `fig12_input_validation.png` | At the menu type `abc`, then `9` | Both `[!] Enter a whole number between 0 and 8.` messages |
| 13 | `fig13_build.png` | `mingw32-make clean` then `mingw32-make` | The g++ command line and no warnings |
| 14 | `fig14_github.png` | Browser: the GitHub repo page | File tree and the commit history showing both contributors |

**Must-have for the report / PPT:** Figs 2, 3, 4, 5, 6, 8, 9.
**Nice to have:** the rest.
