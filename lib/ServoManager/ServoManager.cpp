#include "ServoManager.h"
#include "DevConsole.h"
#include "DevConsoleParse.h"
#include <string.h>
#include <stdlib.h>
// =======================================
// IMPLEMENTACIÓN DE ServoMotor
// =======================================

ServoMotor::ServoMotor(int pin, int initialPos, int minAngle, int maxAngle)
    : _pin(pin),
      _minAngle(minAngle),
      _maxAngle(maxAngle),
      _currentPos(initialPos),
      _targetPos(initialPos),
      _velocity(0),
      _maxSpeed(100),
      _acceleration(300),
      _speed(60),
      _mode(MoveMode::INSTANT),
      _lastUpdate(0)
{
}

// Inicializa físicamente el servo
void ServoMotor::init() {
    _servo.attach(_pin);
    _servo.write((int)_currentPos);
    _lastUpdate = millis();
}

// Cambia el modo de movimiento
void ServoMotor::setMode(MoveMode newMode) {
    _mode = newMode;
    _velocity = 0;
    _lastUpdate = millis();
}

// Movimiento instantáneo
void ServoMotor::moveInstant(int grados) {
    grados = constrain(grados, _minAngle, _maxAngle);
    setMode(MoveMode::INSTANT);
    _currentPos = grados;
    _targetPos = grados;
    _servo.write(grados);
}

// Movimiento a velocidad constante
void ServoMotor::moveConstant(int grados, float speed) {
    grados = constrain(grados, _minAngle, _maxAngle);
    _targetPos = grados;
    _speed = abs(speed);
    setMode(MoveMode::CONSTANT_SPEED);
}

// Movimiento orgánico
void ServoMotor::moveOrganic(int grados, float maxSpeed, float acceleration) {
    grados = constrain(grados, _minAngle, _maxAngle);
    _targetPos = grados;
    _maxSpeed = abs(maxSpeed);
    _acceleration = abs(acceleration);
    setMode(MoveMode::ORGANIC);
}

// Update no bloqueante
void ServoMotor::update() {
    unsigned long now = millis();
    float dt = (now - _lastUpdate) / 1000.0f;
    _lastUpdate = now;

    float distance = _targetPos - _currentPos;

    switch (_mode) {
        case MoveMode::CONSTANT_SPEED: {
            float direction = (distance > 0) ? 1.0f : -1.0f;
            float step = direction * _speed * dt;
            if (abs(step) >= abs(distance))
                _currentPos = _targetPos;
            else
                _currentPos += step;
            break;
        }
        case MoveMode::ORGANIC: {
            if (abs(distance) < 0.5f) {
                _currentPos = _targetPos;
                _velocity = 0;
                break;
            }
            float direction = (distance > 0) ? 1.0f : -1.0f;
            float brakingDistance = (_velocity * _velocity) / (2 * _acceleration);

            if (abs(distance) <= brakingDistance)
                _velocity -= direction * _acceleration * dt;
            else
                _velocity += direction * _acceleration * dt;

            if (abs(_velocity) > _maxSpeed)
                _velocity = direction * _maxSpeed;

            _currentPos += _velocity * dt;
            break;
        }
        case MoveMode::INSTANT:
        default:
            break;
    }

    _servo.write((int)_currentPos);
}

// Indica si todavía se mueve
bool ServoMotor::isMoving() const {
    float distance = _targetPos - _currentPos;
    return !(abs(distance) < 0.5f && abs(_velocity) < 1.0f);
}

// =======================================
// IMPLEMENTACIÓN DE ServoManager
// =======================================

// Variable estática para DevConsole
static ServoManager* _devActiveServos = nullptr;

void ServoManager::init() {
    _devActiveServos = this;
    _headShutter.init();
    devSetModuleStatus("SERVO", "HEAD_SHUTTER", "READY");
    _sonarMount.init();
    devSetModuleStatus("SERVO", "SONAR_MOUNT", "READY");
}

void ServoManager::update() {
    _headShutter.update();
    _sonarMount.update();
}

ServoMotor& ServoManager::headShutter() {
    return _headShutter;
}

ServoMotor& ServoManager::sonarMount() {
    return _sonarMount;
}

ServoManager* ServoManager::devGetActive() {
    return _devActiveServos;
}

// =======================================
// DevConsole - comando SERVO
// =======================================

// Busca servo por ID de texto
static ServoMotor* findServoById(const char* id) {
    if (devEqualsIgnoreCase(id, "HEAD_SHUTTER")) {
        return &_devActiveServos->headShutter();
    }
    if (devEqualsIgnoreCase(id, "SONAR_MOUNT")) {
        return &_devActiveServos->sonarMount();
    }
    return nullptr;
}

void ServoManager::devCommand(const char* args) {
    if (!_devActiveServos) {
        devSend("ERR", "SERVO", "NOT_READY");
        return;
    }

    // Comando: SERVO LIST
    char id[16];
    char mode[16];
    char p1[16];
    char p2[16];
    char p3[16];

    const char* p = devNextToken(args, id, sizeof(id));
    if (!p) {
        devSend("ERR", "SERVO", "MISSING_ARGS");
        return;
    }

    if (devEqualsIgnoreCase(id, "LIST")) {
        devSend("DATA", "SERVO", "HEAD_SHUTTER");
        devSend("DATA", "SERVO", "SONAR_MOUNT");
        return;
    }

    p = devNextToken(p, mode, sizeof(mode));
    if (!p || mode[0] == '\0') {
        devSend("ERR", "SERVO", "MISSING_ARGS");
        return;
    }

    ServoMotor* servo = findServoById(id);
    if (!servo) {
        devSend("ERR", "SERVO", "UNKNOWN_ID");
        return;
    }

    if (devEqualsIgnoreCase(mode, "SET")) {
        p = devNextToken(p, p1, sizeof(p1));
        if (!p || p1[0] == '\0') {
            devSend("ERR", "SERVO", "MISSING_ARGS");
            return;
        }
        int angle = atoi(p1);
        servo->moveInstant(angle);
        devSend("OK", "SERVO", id);
        return;
    }

    if (devEqualsIgnoreCase(mode, "CONST")) {
        p = devNextToken(p, p1, sizeof(p1));
        p = devNextToken(p, p2, sizeof(p2));
        if (!p1[0] || !p2[0]) {
            devSend("ERR", "SERVO", "MISSING_ARGS");
            return;
        }
        int angle = atoi(p1);
        float speed = (float)atof(p2);
        servo->moveConstant(angle, speed);
        devSend("OK", "SERVO", id);
        return;
    }

    if (devEqualsIgnoreCase(mode, "ORG")) {
        p = devNextToken(p, p1, sizeof(p1));
        p = devNextToken(p, p2, sizeof(p2));
        p = devNextToken(p, p3, sizeof(p3));
        if (!p1[0] || !p2[0] || !p3[0]) {
            devSend("ERR", "SERVO", "MISSING_ARGS");
            return;
        }
        int angle = atoi(p1);
        float maxSpeed = (float)atof(p2);
        float acc = (float)atof(p3);
        servo->moveOrganic(angle, maxSpeed, acc);
        devSend("OK", "SERVO", id);
        return;
    }

    devSend("ERR", "SERVO", "UNKNOWN_MODE");
}
