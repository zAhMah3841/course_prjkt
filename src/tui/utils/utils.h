#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <signal.h>
#include <conio.h>

extern volatile sig_atomic_t exit_flag;

void clearScreen();
void waitForKey();
void handleSignal();
void initConsole();
bool getInput(const wchar_t* prompt, wchar_t* buffer, size_t size);
bool getPassword(const wchar_t* prompt, wchar_t* buffer, size_t size);
char* wideToUtf8(const wchar_t* wideStr);
wchar_t* utf8ToWide(const char* utf8Str);
size_t wstrWidth(const wchar_t* str);
wchar_t* trimWhitespace(wchar_t* str);
bool confirmAction(const wchar_t* message, bool default_choice);
char* wcharToUtf8(const wchar_t* wstr);

// Validation functions
bool validatePassword(const wchar_t* password);
bool isUsernameValid(const wchar_t* username);

#endif //UTILS_H