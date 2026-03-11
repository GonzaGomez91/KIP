#include "DevConsolePot.h"
#include "DevConsole.h"
#include "DevConsoleParse.h"
#include "ServoManager.h"
#include <Arduino.h>
#include <stdlib.h>

// ==============================
// Estado interno
// ==============================

static ServoMotor* _potServo = nullptr;
static int _potPin = A0;
static int _potMin = 0;
static int _potMax = 180;
static bool _potActive = false;
static unsigned long _potLastReport = 0;
static const unsigned long POT_REPORT_MS = 1000;

// ==============================
// Helpers
// ==============================

static ServoMotor* findServoById(const char* id) {
    ServoManager* sm = ServoManager::devGetActive();
    if (!sm) return nullptr;

    if (devEqualsIgnoreCase(id, "HEAD_SHUTTER")) {
        return &sm->headShutter();
    }
    if (devEqualsIgnoreCase(id, "SONAR_MOUNT")) {
        return &sm->sonarMount();
    }
    return nullptr;
}

static int mapPotToAngle(int value, int minA, int maxA) {
    long mapped = map(value, 0, 1023, minA, maxA);
    return (int)mapped;
}

// ==============================
// API
// ==============================

void DevConsolePot_init() {
    _potServo = nullptr;
    _potPin = A0;
    _potMin = 0;
    _potMax = 180;
    _potActive = false;
    _potLastReport = 0;
}

void DevConsolePot_update() {
    if (!_potActive || !_potServo) return;

    int raw = analogRead(_potPin);
    int angle = mapPotToAngle(raw, _potMin, _potMax);

    _potServo->moveInstant(angle);

    unsigned long now = millis();
    if (now - _potLastReport >= POT_REPORT_MS) {
        _potLastReport = now;
        char buf[12];
        itoa(angle, buf, 10);
        devSend("DATA", "POT", buf);
    }
}

void DevConsolePot_command(const char* args) {
    char cmd[16];
    char p1[16];
    char p2[16];
    char p3[16];
    char p4[16];

    const char* p = devNextToken(args, cmd, sizeof(cmd));
    if (!p) {
        devSend("ERR", "POT", "MISSING_ARGS");
        return;
    }

    if (devEqualsIgnoreCase(cmd, "START")) {
        if (!_potServo) {
            devSend("ERR", "POT", "NOT_BOUND");
            return;
        }
        _potActive = true;
        devSend("OK", "POT", "STARTED");
        return;
    }

    if (devEqualsIgnoreCase(cmd, "STOP")) {
        _potActive = false;
        devSend("OK", "POT", "STOPPED");
        return;
    }

    if (devEqualsIgnoreCase(cmd, "STATUS")) {
        if (_potServo) {
            devSend("DATA", "POT", _potActive ? "ACTIVE" : "IDLE");
        } else {
            devSend("DATA", "POT", "UNBOUND");
        }
        return;
    }

    if (devEqualsIgnoreCase(cmd, "BIND")) {
        p = devNextToken(p, p1, sizeof(p1)); // SERVO_ID
        p = devNextToken(p, p2, sizeof(p2)); // PIN
        p = devNextToken(p, p3, sizeof(p3)); // MIN
        p = devNextToken(p, p4, sizeof(p4)); // MAX

        if (!p1[0] || !p2[0] || !p3[0] || !p4[0]) {
            devSend("ERR", "POT", "MISSING_ARGS");
            return;
        }

        ServoMotor* s = findServoById(p1);
        if (!s) {
            devSend("ERR", "POT", "UNKNOWN_SERVO");
            return;
        }

        _potServo = s;
        _potPin = (toupper(p2[0]) == 'A') ? atoi(p2 + 1) + A0 : atoi(p2);
        _potMin = atoi(p3);
        _potMax = atoi(p4);

        devSend("OK", "POT", "BOUND");
        return;
    }

    devSend("ERR", "POT", "UNKNOWN_CMD");
}
