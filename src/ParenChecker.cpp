// =============================================================================
// ParenChecker.cpp  --  TEMPORARY STUB
// Owner: Bhavya Porwal (02318015625)
// Replace this whole file with the real implementation (see TEAMMATE_TASKS.md §3).
// =============================================================================
#include "ParenChecker.h"

#include <iostream>

ParenResult checkBalanced(const std::string& expr, TraceTable* trace) {
    (void)expr;
    (void)trace;
    std::cout << "[ParenChecker] checkBalanced(): not implemented yet\n";

    ParenResult r;
    r.balanced = false;
    r.status = ParenStatus::UnclosedOpening;
    r.position = -1;
    r.found = '\0';
    r.expected = '\0';
    r.openPosition = -1;
    r.message = "not implemented yet";
    return r;
}
