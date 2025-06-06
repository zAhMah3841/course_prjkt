cmake_minimum_required(VERSION 3.20)
project(untitled C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Включение каталогов с заголовочными файлами
include_directories(
        src/
        src/tui/utils/
        src/tui/dialogs/
        src/tui/menus/
        src/database/common/
        src/loger/
        lib/sqlite/  # Исправляем путь к SQLite
)

# Библиотека SQLite из папки lib
add_library(sqlite3 STATIC
        lib/sqlite/sqlite3.c
        lib/sqlite/sqlite3.h
        src/database/tables/tables.h
        src/database/tables/tables.c
        src/database/tables/schema.h
        src/database/users_management/users_management.c
        src/database/users_management/users_management.h
        src/database/orders_management/orders_management.c
        src/database/orders_management/orders_management.h
)

# Указываем, что заголовочные файлы SQLite являются публичными
target_include_directories(sqlite3 PUBLIC lib/sqlite)

# Основной исполняемый файл
add_executable(untitled
        src/main.c
        src/tui/utils/console_utils.c
        src/tui/utils/validation_utils.c
        src/tui/utils/order_validation.c
        src/tui/dialogs/login.c
        src/database/common/database.c
        src/loger/loger.c
        src/tui/dialogs/add_user.c
        src/tui/dialogs/delete_user.c
        src/tui/dialogs/edit_user.c
        src/tui/dialogs/add_order.c
        src/tui/dialogs/edit_order.c
        src/tui/dialogs/delete_order.c
        src/tui/dialogs/check_order_number.c
        src/tui/dialogs/search_orders_by_letter.c
        src/tui/dialogs/search_orders.c
        src/tui/dialogs/sort_orders.c
        src/tui/tables/users_list.c
        src/tui/tables/orders_list.c
        src/tui/tables/tables.h
        src/tui/menus/menus.h
        src/tui/menus/common/menus.c
        src/tui/menus/admin.c
        src/tui/menus/common/menu_types.h
        src/tui/menus/acounts_manager.c
        src/tui/menus/orders_manager.c
        src/tui/menus/user.c
        src/tui/widgets/table/table.c
        src/tui/widgets/table/table.h
)

# Связывание библиотек
target_link_libraries(untitled
        sqlite3
)