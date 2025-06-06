#include "../../utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include "table.h"

Table* createTable(const size_t rows, const size_t cols) {
    Table* table = malloc(sizeof(Table));
    table->rows = rows;
    table->cols = cols;

    table->data = malloc(rows * sizeof(wchar_t**));
    for (size_t i = 0; i < rows; i++) {
        table->data[i] = malloc(cols * sizeof(wchar_t*));
        for (size_t j = 0; j < cols; j++) {
            table->data[i][j] = NULL;
        }
    }

    table->col_widths = malloc(cols * sizeof(size_t));
    table->col_wrap = malloc(cols * sizeof(bool));

    for (size_t i = 0; i < cols; i++) {
        table->col_widths[i] = 10;
        table->col_wrap[i] = true;
    }

    return table;
}

void freeTable(Table* table) {
    if (!table) return;

    for (size_t i = 0; i < table->rows; i++) {
        for (size_t j = 0; j < table->cols; j++) {
            free(table->data[i][j]);
        }
        free(table->data[i]);
    }
    free(table->data);

    free(table->col_widths);
    free(table->col_wrap);
    free(table);
}

void setCell(const Table* table, const size_t row, const size_t col, const wchar_t* value) {
    if (row >= table->rows || col >= table->cols) return;

    free(table->data[row][col]);
    table->data[row][col] = wcsdup(value);
}

void calculateLayout(const Table* table, const int max_total_width) {
    size_t* min_widths = malloc(table->cols * sizeof(size_t));

    for (size_t col = 0; col < table->cols; col++) {
        size_t max_width = 0;
        for (size_t row = 0; row < table->rows; row++) {
            if (table->data[row][col]) {
                size_t width = wstrWidth(table->data[row][col]);
                if (width > max_width) max_width = width;
            }
        }
        min_widths[col] = max_width + 2;
    }

    int remaining_width = max_total_width;
    int fixed_cols = 0;

    for (size_t col = 0; col < table->cols; col++) {
        table->col_widths[col] = min_widths[col];
        remaining_width -= min_widths[col];
        if (!table->col_wrap[col]) fixed_cols++;
    }

    if (remaining_width > 0 && fixed_cols < table->cols) {
        size_t flexible_cols = table->cols - fixed_cols;
        size_t extra_width = remaining_width / flexible_cols;

        for (size_t col = 0; col < table->cols; col++) {
            if (table->col_wrap[col]) {
                table->col_widths[col] += extra_width;
            }
        }
    }

    free(min_widths);
}

wchar_t** splitString(const wchar_t* str, const size_t width, size_t* lines_count) {
    if (!str || width == 0) return NULL;

    const size_t len = wcslen(str);
    const size_t max_lines = (len / width) + 2;
    wchar_t** lines = malloc(max_lines * sizeof(wchar_t*));

    size_t current_line = 0;
    size_t start = 0;
    const size_t end = 0;

    while (end < len) {
        size_t line_end = start + width;
        if (line_end >= len) {
            line_end = len;
        } else {
            while (line_end > start && str[line_end] != L' ' && str[line_end] != L'\t') {
                line_end--;
            }
            if (line_end == start) { // Если нет пробелов - переносим посимвольно
                line_end = start + width;
                if (line_end > len) line_end = len;
            }
        }

        size_t line_len = line_end - start;
        lines[current_line] = malloc((line_len + 1) * sizeof(wchar_t));
        wcsncpy(lines[current_line], str + start, line_len);
        lines[current_line][line_len] = L'\0';

        current_line++;
        start = line_end;
        while (start < len && (str[start] == L' ' || str[start] == L'\t')) {
            start++;
        }
    }

    *lines_count = current_line;
    return lines;
}

void printTable(const Table* table) {
    wprintf(L"┌");
    for (size_t col = 0; col < table->cols; col++) {
        for (size_t i = 0; i < table->col_widths[col]; i++) {
            wprintf(L"─");
        }
        if (col < table->cols - 1) wprintf(L"┬");
    }
    wprintf(L"┐\n");

    for (size_t row = 0; row < table->rows; row++) {
        size_t max_lines = 1;
        for (size_t col = 0; col < table->cols; col++) {
            if (table->data[row][col] && table->col_wrap[col]) {
                size_t cell_width = wstrWidth(table->data[row][col]);
                size_t col_width = table->col_widths[col] - 2; // -2 для отступов
                if (cell_width > col_width) {
                    size_t lines = (cell_width / col_width) + 1;
                    if (lines > max_lines) max_lines = lines;
                }
            }
        }

        for (size_t line = 0; line < max_lines; line++) {
            wprintf(L"│");
            for (size_t col = 0; col < table->cols; col++) {
                wchar_t* cell = table->data[row][col] ? table->data[row][col] : L"";
                size_t width = table->col_widths[col];
                size_t cell_width = wstrWidth(cell);

                if (cell_width <= width - 2 || !table->col_wrap[col]) {
                    // Вмещается целиком или перенос запрещен
                    wprintf(L"%-*ls", (int)(width-2), line == 0 ? cell : L"");
                } else {
                    size_t lines_count = 0;
                    wchar_t** lines = splitString(cell, width - 2, &lines_count);

                    if (line < lines_count) {
                        wprintf(L"%-*ls", (int)(width-2), lines[line]);
                    } else {
                        wprintf(L"%-*ls", (int)(width-2), L"");
                    }

                    if (lines) {
                        for (size_t i = 0; i < lines_count; i++) {
                            free(lines[i]);
                        }
                        free(lines);
                    }
                }
                wprintf(L"  │");
            }
            wprintf(L"\n");
        }

        if (row < table->rows - 1) {
            wprintf(L"├");
            for (size_t col = 0; col < table->cols; col++) {
                for (size_t i = 0; i < table->col_widths[col]; i++) {
                    wprintf(L"─");
                }
                if (col < table->cols - 1) wprintf(L"┼");
            }
            wprintf(L"┤\n");
        }
    }

    wprintf(L"└");
    for (size_t col = 0; col < table->cols; col++) {
        for (size_t i = 0; i < table->col_widths[col]; i++) {
            wprintf(L"─");
        }
        if (col < table->cols - 1) wprintf(L"┴");
    }
    wprintf(L"┘\n");
}