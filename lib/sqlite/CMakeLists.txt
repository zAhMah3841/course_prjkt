# Создаем библиотеку SQLite
add_library(sqlite3 STATIC 
    sqlite3.c
    sqlite3.h
)

# Настройки компиляции для Windows
if(WIN32)
    target_compile_definitions(sqlite3 PRIVATE 
        _CRT_SECURE_NO_WARNINGS 
        _CRT_NONSTDC_NO_DEPRECATE
    )
endif()

# Делаем заголовочные файлы доступными
target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 