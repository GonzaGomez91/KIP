#pragma once

// Modulo IMU para MPU6050 usando la libreria de Adafruit.
// Implementacion POO basada en el algoritmo probado por el usuario.

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class IMU {
public:
    // Constructor con periodo de actualizacion en ms (por defecto 10 ms ~ 100 Hz)
    IMU(uint16_t updatePeriodMs = 10);

    // Inicializa la IMU y calibra el giroscopio.
    // Devuelve true si el sensor esta presente.
    bool init();

    // Actualiza lecturas si ya paso el tiempo configurado.
    // Devuelve true si actualizo.
    bool update();

    // Configura el periodo de actualizacion (ms)
    void setUpdatePeriodMs(uint16_t ms);
    uint16_t getUpdatePeriodMs() const;

    // Angulos actuales (grados)
    float getRoll() const;
    float getPitch() const;
    float getYaw() const;
    void setYaw(float yawDeg);

    // Indica si el modulo esta listo
    bool isReady() const;

private:
    // Calibracion basica del giroscopio (sensor quieto)
    void calibrateGyro();

    // Inicia una recalibracion no bloqueante
    void startRecalibration(bool autoMode);

    Adafruit_MPU6050 _mpu;
    bool _ready;
    uint16_t _updatePeriodMs;
    unsigned long _lastUpdateMs;

    // Estado de orientacion (grados)
    float _roll;
    float _pitch;
    float _yaw;

    // Offsets del giroscopio (rad/s)
    float _gyroXOffset;
    float _gyroYOffset;
    float _gyroZOffset;

};
