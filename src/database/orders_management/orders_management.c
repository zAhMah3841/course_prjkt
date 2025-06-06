#include "../common/database.h"
#include "orders_management.h"
#include "../../tui/utils/utils.h"
#include <stdlib.h>
#include <string.h>

Order* getOrdersList(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

bool addOrder(const Order* order) {
    const char* sql = "INSERT INTO orders (last_name, first_name, surname_name, work_type, price, team) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return false;

    bindTextParam(stmt, 1, wideToUtf8(order->last_name));
    bindTextParam(stmt, 2, wideToUtf8(order->first_name));
    bindTextParam(stmt, 3, wideToUtf8(order->surname_name));
    bindTextParam(stmt, 4, wideToUtf8(order->work_type));
    bindRealParam(stmt, 5, order->price);
    bindTextParam(stmt, 6, wideToUtf8(order->team));

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool deleteOrder(int number) {
    const char* sql = "DELETE FROM orders WHERE id = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return false;

    bindIntParam(stmt, 1, number);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool updateOrder(const Order* order) {
    const char* sql = "UPDATE orders SET last_name = ?, first_name = ?, surname_name = ?, work_type = ?, price = ?, team = ? WHERE id = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return false;

    bindTextParam(stmt, 1, wideToUtf8(order->last_name));
    bindTextParam(stmt, 2, wideToUtf8(order->first_name));
    bindTextParam(stmt, 3, wideToUtf8(order->surname_name));
    bindTextParam(stmt, 4, wideToUtf8(order->work_type));
    bindRealParam(stmt, 5, order->price);
    bindTextParam(stmt, 6, wideToUtf8(order->team));
    bindIntParam(stmt, 7, order->number);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

bool isOrderExists(int number) {
    const char* sql = "SELECT 1 FROM orders WHERE id = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return false;

    bindIntParam(stmt, 1, number);

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

Order* findOrdersByFirstLetter(wchar_t letter, size_t* count) {
    // Convert wide character to UTF-8
    char utf8Letter[5] = {0};
    wchar_t letterStr[2] = {letter, L'\0'};
    const char* utf8LetterPtr = wideToUtf8(letterStr);
    if (utf8LetterPtr) {
        strncpy(utf8Letter, utf8LetterPtr, sizeof(utf8Letter) - 1);
        free((void*)utf8LetterPtr);
    }

    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders WHERE first_name LIKE ? || '%' ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    bindTextParam(stmt, 1, utf8Letter);

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* findOrdersByLastName(const wchar_t* last_name, size_t* count) {
    const char* utf8LastName = wideToUtf8(last_name);
    if (!utf8LastName) return NULL;

    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders WHERE last_name LIKE ? || '%' ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) {
        free((void*)utf8LastName);
        return NULL;
    }

    bindTextParam(stmt, 1, utf8LastName);
    free((void*)utf8LastName);

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* findOrdersByWorkType(const wchar_t* work_type, size_t* count) {
    const char* utf8WorkType = wideToUtf8(work_type);
    if (!utf8WorkType) return NULL;

    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders WHERE work_type LIKE ? || '%' ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) {
        free((void*)utf8WorkType);
        return NULL;
    }

    bindTextParam(stmt, 1, utf8WorkType);
    free((void*)utf8WorkType);

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* findOrdersByPriceRange(double min_price, double max_price, size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders WHERE price BETWEEN ? AND ? ORDER BY price";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    bindRealParam(stmt, 1, min_price);
    bindRealParam(stmt, 2, max_price);

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* findOrdersByTeam(const wchar_t* team, size_t* count) {
    const char* utf8Team = wideToUtf8(team);
    if (!utf8Team) return NULL;

    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders WHERE team LIKE ? || '%' ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) {
        free((void*)utf8Team);
        return NULL;
    }

    bindTextParam(stmt, 1, utf8Team);
    free((void*)utf8Team);

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* getOrdersSortedByNumber(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY id";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* getOrdersSortedByLastName(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY last_name COLLATE NOCASE";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* getOrdersSortedByWorkType(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY work_type COLLATE NOCASE";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* getOrdersSortedByPrice(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY price";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* getOrdersSortedByTeam(size_t* count) {
    const char* sql = "SELECT id, last_name, first_name, surname_name, work_type, price, team FROM orders ORDER BY team COLLATE NOCASE";
    sqlite3_stmt* stmt = prepareStatement(sql);
    if (!stmt) return NULL;

    *count = getRecordsCount(stmt);
    Order* orders = (Order*)calloc(*count, sizeof(Order));
    if (!orders) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < *count) {
        orders[i].number = sqlite3_column_int(stmt, 0);
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 1);
        const unsigned char* first_name = sqlite3_column_text(stmt, 2);
        const unsigned char* surname = sqlite3_column_text(stmt, 3);
        const unsigned char* work_type = sqlite3_column_text(stmt, 4);
        const unsigned char* team = sqlite3_column_text(stmt, 6);

        orders[i].last_name = last_name ? utf8ToWide((const char*)last_name) : NULL;
        orders[i].first_name = first_name ? utf8ToWide((const char*)first_name) : NULL;
        orders[i].surname_name = surname ? utf8ToWide((const char*)surname) : NULL;
        orders[i].work_type = work_type ? utf8ToWide((const char*)work_type) : NULL;
        orders[i].price = sqlite3_column_double(stmt, 5);
        orders[i].team = team ? utf8ToWide((const char*)team) : NULL;

        i++;
    }

    sqlite3_finalize(stmt);
    return orders;
} 