#ifndef ORDERS_MANAGEMENT_H
#define ORDERS_MANAGEMENT_H

#include <wchar.h>
#include <stddef.h>

typedef struct {
    int number;
    wchar_t* last_name;
    wchar_t* first_name;
    wchar_t* surname_name;
    wchar_t* work_type;
    double price;
    wchar_t* team;
} Order;

// Получение списка всех заказов
Order* getOrdersList(size_t* count);

// Добавление нового заказа
bool addOrder(const Order* order);

// Удаление заказа по номеру
bool deleteOrder(int number);

// Обновление данных заказа
bool updateOrder(const Order* order);

// Проверка существования заказа по номеру
bool isOrderExists(int number);

// Поиск заказов по первой букве имени заказчика
Order* findOrdersByFirstLetter(wchar_t letter, size_t* count);

// Поиск заказов по фамилии заказчика
Order* findOrdersByLastName(const wchar_t* last_name, size_t* count);

// Поиск заказов по типу работы
Order* findOrdersByWorkType(const wchar_t* work_type, size_t* count);

// Поиск заказов по диапазону цен
Order* findOrdersByPriceRange(double min_price, double max_price, size_t* count);

// Поиск заказов по команде
Order* findOrdersByTeam(const wchar_t* team, size_t* count);

// Сортировка заказов по номеру
Order* getOrdersSortedByNumber(size_t* count);

// Сортировка заказов по фамилии заказчика
Order* getOrdersSortedByLastName(size_t* count);

// Сортировка заказов по типу работы
Order* getOrdersSortedByWorkType(size_t* count);

// Сортировка заказов по стоимости
Order* getOrdersSortedByPrice(size_t* count);

// Сортировка заказов по команде
Order* getOrdersSortedByTeam(size_t* count);

#endif // ORDERS_MANAGEMENT_H 