#include <Arduino.h>
#include "ErrorManager.h"

#define ERROR_HISTORY_SIZE 10

static SystemError errorHistory[ERROR_HISTORY_SIZE];
static int errorIndex = 0;
static int errorCount = 0;

void ErrorManager_init() {

    errorIndex = 0;
    errorCount = 0;

    for (int i = 0; i < ERROR_HISTORY_SIZE; i++) {
        errorHistory[i] = ERROR_NONE;
    }
}

void ErrorManager_report(SystemError error) {

    if (error == ERROR_NONE) return;

    errorHistory[errorIndex] = error;

    errorIndex = (errorIndex + 1) % ERROR_HISTORY_SIZE;

    if (errorCount < ERROR_HISTORY_SIZE)
        errorCount++;
}

SystemError ErrorManager_getLast() {

    if (errorCount == 0)
        return ERROR_NONE;

    int lastIndex = (errorIndex - 1 + ERROR_HISTORY_SIZE) % ERROR_HISTORY_SIZE;

    return errorHistory[lastIndex];
}

bool ErrorManager_hasErrors() {
    return errorCount > 0;
}

SystemError ErrorManager_getFromHistory(int index) {

    if (index >= errorCount)
        return ERROR_NONE;

    int realIndex = (errorIndex - 1 - index + ERROR_HISTORY_SIZE) % ERROR_HISTORY_SIZE;

    return errorHistory[realIndex];
}

void ErrorManager_clear() {

    errorIndex = 0;
    errorCount = 0;
}
