#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

// Errores globales del sistema
enum SystemError {

    ERROR_NONE = 0,

    // --- Servo ---
    ERROR_SERVO_INVALID_ID,
    ERROR_SERVO_OUT_OF_RANGE,

    // --- Matrix ---
    ERROR_MATRIX_NOT_INITIALIZED,

    // --- Sensor ---
    ERROR_SENSOR_TIMEOUT,

    ERROR_COUNT
};

void ErrorManager_init();

void ErrorManager_report(SystemError error);

SystemError ErrorManager_getLast();

bool ErrorManager_hasErrors();

SystemError ErrorManager_getFromHistory(int index);

void ErrorManager_clear();

#endif
