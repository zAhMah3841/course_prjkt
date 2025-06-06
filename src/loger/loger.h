#ifndef LOGGER_H
#define LOGGER_H

#include <wchar.h>

typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

void loggerLog(LogLevel level, const char* message);
void loggerLogW(LogLevel level, const wchar_t* message);

#define LOG_ERROR(msg) loggerLog(LOG_LEVEL_ERROR, msg)
#define LOG_WARNING(msg) loggerLog(LOG_LEVEL_WARNING, msg)
#define LOG_INFO(msg) loggerLog(LOG_LEVEL_INFO, msg)
#define LOG_DEBUG(msg) loggerLog(LOG_LEVEL_DEBUG, msg)

#define LOG_ERROR_W(msg) loggerLogW(LOG_LEVEL_ERROR, msg)
#define LOG_WARNING_W(msg) loggerLogW(LOG_LEVEL_WARNING, msg)
#define LOG_INFO_W(msg) loggerLogW(LOG_LEVEL_INFO, msg)
#define LOG_DEBUG_W(msg) loggerLogW(LOG_LEVEL_DEBUG, msg)

#endif // LOGGER_H