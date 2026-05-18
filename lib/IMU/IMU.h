#pragma once

// Modulo IMU para MPU6050 usando la libreria de Adafruit.
// Implementacion POO basada en el algoritmo probado por el usuario.

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class IMU {
public:
    // Constructor
    IMU();

    // Inicializa la IMU y calibra el giroscopio.
    // Devuelve true si el sensor esta presente.
    bool init();

    // Habilita interrupciones para detectar movimiento
    // Requiere conectar INT del MPU6050 a un pin de interrupcion
    bool initInterrupt(uint8_t intPin);

    // Procesa interrupciones pendientes (no calcula orientacion)
    void updateEvents();

    // Detecta movimiento simple (consumible)
    bool detectMotion();

    // Calcula orientacion bajo demanda
    bool requestOrientation();

    // Angulos actuales (grados)
    float getRoll() const;
    float getPitch() const;
    float getYaw() const;
    void setYaw(float yawDeg);

    // Indica si el modulo esta listo
    bool isReady() const;

private:
    // ISR estatico para interrupciones
    static void isrThunk();
    static IMU* _activeInstance;

    // Helpers I2C para configuracion de interrupciones
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);

    // Calibracion basica del giroscopio (sensor quieto)
    void calibrateGyro();

    Adafruit_MPU6050 _mpu;
    bool _ready;
    unsigned long _lastUpdateMs;

    // Estado de orientacion (grados)
    float _roll;
    float _pitch;
    float _yaw;

    // Offsets del giroscopio (rad/s)
    float _gyroXOffset;
    float _gyroYOffset;
    float _gyroZOffset;

    // Interrupciones de movimiento
    uint8_t _intPin;
    bool _intEnabled;
    volatile bool _intFlag;
    bool _motionDetected;

    // Configuracion basica de movimiento
    uint8_t _motionThreshold;
    uint8_t _motionDuration;

};
