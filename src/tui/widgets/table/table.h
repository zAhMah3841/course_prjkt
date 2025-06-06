//
// Created by Gleb on 03.06.2025.
//

#ifndef TABLE_H
#define TABLE_H

#include <wchar.h>
#include <stdbool.h>

typedef struct {
    wchar_t*** data;
    size_t rows;
    size_t cols;
    size_t* col_widths;
    bool* col_wrap;
} Table;

Table* createTable(size_t rows, size_t cols);
void freeTable(Table* table);

void setCell(const Table* table, size_t row, size_t col, const wchar_t* value);
void calculateLayout(const Table* table, int max_total_width);

void printTable(const Table* table);

#endif //TABLE_H
