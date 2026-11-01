// =============================================================================
// TraceTable.h
// Collects the rows of a step-by-step trace (Symbol | Stack | Output/Action)
// and prints them as a boxed ASCII table. Algorithms only ADD rows;
// the menu decides whether to print.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef TRACE_TABLE_H
#define TRACE_TABLE_H

#include <string>
#include <vector>

class TraceTable {
public:
    TraceTable(const std::string& col1 = "Symbol",
               const std::string& col2 = "Stack",
               const std::string& col3 = "Output");
    void setHeaders(const std::string& col1, const std::string& col2, const std::string& col3);
    void addRow(const std::string& symbol, const std::string& stack, const std::string& output);
    void print() const;   // prints a boxed ASCII table to std::cout with auto column widths
    bool empty() const;
    void clear();         // removes rows, keeps headers
private:
    std::vector<std::string> headers_;                // always 3 entries
    std::vector<std::vector<std::string> > rows_;
};

#endif // TRACE_TABLE_H
