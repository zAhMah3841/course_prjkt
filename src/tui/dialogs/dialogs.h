#ifndef DIALOGS_H
#define DIALOGS_H

#include <wchar.h>
#include <stdbool.h>

int userLoginDialog(wchar_t* output_username, size_t username_size, unsigned max_attempts);
bool addUserDialog();
bool deleteUserDialog(const wchar_t* current_user);
bool editUserDialog();
bool addOrderDialog();
bool editOrderDialog();
bool deleteOrderDialog();
bool checkOrderNumberDialog();
bool searchOrdersByLetterDialog();
bool searchOrdersDialog();
bool sortOrdersDialog();

#endif //DIALOGS_H
