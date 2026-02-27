#include "ServoManager.h"

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

void ServoManager::init() {
    _headShutter.init();
}

void ServoManager::update() {
    _headShutter.update();
}

ServoMotor& ServoManager::headShutter() {
    return _headShutter;
}
