#include "menus.h"
#include "../tables/tables.h"
#include "../dialogs/dialogs.h"

void addOrderDialogWrapper(MenuParam param) {
    addOrderDialog();
}

void editOrderDialogWrapper(MenuParam param) {
    editOrderDialog();
}

void deleteOrderDialogWrapper(MenuParam param) {
    deleteOrderDialog();
}

void checkOrderNumberDialogWrapper(MenuParam param) {
    checkOrderNumberDialog();
}

void searchOrdersByLetterDialogWrapper(MenuParam param) {
    searchOrdersByLetterDialog();
}

void searchOrdersDialogWrapper(MenuParam param) {
    searchOrdersDialog();
}

void sortOrdersDialogWrapper(MenuParam param) {
    sortOrdersDialog();
}

void runOrdersManagerMenu() {
    MenuItem items[] = {
        {L'1', L"Отобразить все данные", printOrdersTableAndWait, {0}},
        {L'2', L"Добавить заявку", addOrderDialogWrapper, {0}},
        {L'3', L"Редактировать заявку", editOrderDialogWrapper, {0}},
        {L'4', L"Удалить заявку", deleteOrderDialogWrapper, {0}},
        {L'5', L"Проверить уникальность номера заявки", checkOrderNumberDialogWrapper, {0}},
        {L'6', L"Поиск заказчиков по 1-й букве имени", searchOrdersByLetterDialogWrapper, {0}},
        {L'7', L"Поиск данных", searchOrdersDialogWrapper, {0}},
        {L'8', L"Сортировка данных", sortOrdersDialogWrapper, {0}},
        {L'0', L"Выход", NULL, {0}}
    };

    const Menu ordersManagerMenu = {
        L"УПРАВЛЕНИЕ ДАННЫМИ",
        items,
        sizeof(items) / sizeof(items[0])
    };

    runMenu(&ordersManagerMenu);
}
