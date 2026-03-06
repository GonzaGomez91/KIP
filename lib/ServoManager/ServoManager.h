#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Arduino.h>
#include <Servo.h>

// =======================================
// ENUMS
// =======================================

enum class MoveMode {
    INSTANT,
    CONSTANT_SPEED,
    ORGANIC
};

// Identificación de los servos del sistema
enum class ServoID {
    HEAD_SHUTTER,
    SONAR_MOUNT,
    // Más servos se pueden agregar aquí
};

// =======================================
// CLASE ServoMotor
// Controla un servo individual con lógica no bloqueante
// =======================================

class ServoMotor {
public:
    // Constructor
    ServoMotor(int pin,
               int initialPos,
               int minAngle,
               int maxAngle);

    // Inicializa físicamente el servo (attach)
    void init();

    // Actualiza posición según modo y tiempo transcurrido
    void update();

    // Movimiento instantáneo
    void moveInstant(int grados);

    // Movimiento a velocidad constante
    void moveConstant(int grados, float speed);

    // Movimiento orgánico con aceleración y desaceleración
    void moveOrganic(int grados, float maxSpeed, float acceleration);

    // Retorna si el servo todavía se está moviendo
    bool isMoving() const;

    int getCurrentPos() const { return (int)_currentPos; }

private:
    void setMode(MoveMode newMode);

    Servo _servo;          // Objeto Servo de Arduino
    int _pin;
    int _minAngle;
    int _maxAngle;

    float _currentPos;     // Posición interna
    float _targetPos;      // Posición objetivo

    float _velocity;       // Velocidad actual (solo para orgánico)
    float _maxSpeed;       // Velocidad máxima (solo para orgánico)
    float _acceleration;   // Aceleración (solo para orgánico)
    float _speed;          // Velocidad constante (solo para constante)

    MoveMode _mode;

    unsigned long _lastUpdate; // Para calcular dt
};

// =======================================
// CLASE ServoManager
// Contiene todos los servos del robot
// =======================================

class ServoManager {
public:
    void init();    // Inicializa todos los servos
    void update();  // Actualiza todos los servos

    // Acceso a servos
    ServoMotor& headShutter();
    ServoMotor& sonarMount();
    // Más métodos para acceder a otros servos se pueden agregar aquí

private:
    // Servo del párpado
    ServoMotor _headShutter{9, 15, 0, 150};
    ServoMotor _sonarMount{10, 90, 0, 150};
    // Más servos se pueden agregar aquí
};

#endif
