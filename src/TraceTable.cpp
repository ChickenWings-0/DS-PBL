// =============================================================================
// TraceTable.cpp
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#include "TraceTable.h"

#include <iostream>

TraceTable::TraceTable(const std::string& col1, const std::string& col2,
                       const std::string& col3) {
    setHeaders(col1, col2, col3);
}

void TraceTable::setHeaders(const std::string& col1, const std::string& col2,
                            const std::string& col3) {
    headers_.clear();
    headers_.push_back(col1);
    headers_.push_back(col2);
    headers_.push_back(col3);
}

void TraceTable::addRow(const std::string& symbol, const std::string& stack,
                        const std::string& output) {
    std::vector<std::string> row;
    row.push_back(symbol);
    row.push_back(stack);
    row.push_back(output);
    rows_.push_back(row);
}

bool TraceTable::empty() const { return rows_.empty(); }

void TraceTable::clear() { rows_.clear(); }

void TraceTable::print() const {
    // 1. Each column is as wide as its longest cell (header included).
    std::size_t width[3];
    for (int c = 0; c < 3; ++c) {
        width[c] = headers_[c].size();
        for (std::size_t r = 0; r < rows_.size(); ++r) {
            if (rows_[r][c].size() > width[c]) width[c] = rows_[r][c].size();
        }
    }

    // 2. Helpers to print a separator line and one padded row.
    auto separator = [&]() {
        std::cout << '+';
        for (int c = 0; c < 3; ++c) std::cout << std::string(width[c] + 2, '-') << '+';
        std::cout << '\n';
    };
    auto printRow = [&](const std::vector<std::string>& cells) {
        std::cout << '|';
        for (int c = 0; c < 3; ++c) {
            std::cout << ' ' << cells[c] << std::string(width[c] - cells[c].size(), ' ') << " |";
        }
        std::cout << '\n';
    };

    // 3. Header, rows, closing line.
    separator();
    printRow(headers_);
    separator();
    for (std::size_t r = 0; r < rows_.size(); ++r) printRow(rows_[r]);
    separator();
}
