#include "IMU.h"

#include <Wire.h>

IMU::IMU(uint16_t updatePeriodMs)
    : _mpu(),
      _ready(false),
      _updatePeriodMs(updatePeriodMs),
      _lastUpdateMs(0),
      _roll(0.0f),
      _pitch(0.0f),
      _yaw(0.0f),
      _gyroXOffset(0.0f),
      _gyroYOffset(0.0f),
      _gyroZOffset(0.0f) {
}

bool IMU::init() {
    Wire.begin();
    Wire.setWireTimeout(3000, true);

    if (!_mpu.begin()) {
        _ready = false;
        return false;
    }

    _mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    _mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    calibrateGyro();

    _ready = true;
    _lastUpdateMs = millis();
    return true;
}

bool IMU::update() {
    if (!_ready) {
        return false;
    }

    unsigned long now = millis();
    if ((now - _lastUpdateMs) < _updatePeriodMs) {
        return false;
    }

    float dt = (now - _lastUpdateMs) / 1000.0f;
    _lastUpdateMs = now;
    if (dt > 0.1f) {
        dt = 0.1f;
    }

    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);

    float Ax = a.acceleration.x;
    float Ay = a.acceleration.y;
    float Az = a.acceleration.z;

    float Gx = g.gyro.x - _gyroXOffset;
    float Gy = g.gyro.y - _gyroYOffset;
    float Gz = g.gyro.z - _gyroZOffset;

    // Angulos por acelerometro (grados)
    float rollAcc = atan2(Ay, Az) * (180.0f / PI);
    float pitchAcc = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * (180.0f / PI);

    // Velocidades angulares en grados por segundo
    float gyroRollRate = Gx * (180.0f / PI);
    float gyroPitchRate = Gy * (180.0f / PI);
    float gyroYawRate = Gz * (180.0f / PI);

    // Filtro complementario (mismos pesos del algoritmo original)
    _roll = 0.98f * (_roll + gyroRollRate * dt) + 0.02f * rollAcc;
    _pitch = 0.98f * (_pitch + gyroPitchRate * dt) + 0.02f * pitchAcc;

    // Yaw solo por giroscopio (deriva con el tiempo)
    _yaw += gyroYawRate * dt;
    // Capamos a entero para evitar drift en decimales
    _yaw = (float)((int)roundf(_yaw));

    // Normalizacion simple para mantener los angulos en un rango razonable
    if (_roll > 180.0f) _roll -= 360.0f;
    if (_roll < -180.0f) _roll += 360.0f;
    if (_pitch > 180.0f) _pitch -= 360.0f;
    if (_pitch < -180.0f) _pitch += 360.0f;
    if (_yaw > 180.0f) _yaw -= 360.0f;
    if (_yaw < -180.0f) _yaw += 360.0f;

    return true;
}

void IMU::setUpdatePeriodMs(uint16_t ms) {
    _updatePeriodMs = ms;
}

uint16_t IMU::getUpdatePeriodMs() const {
    return _updatePeriodMs;
}

float IMU::getRoll() const {
    return _roll;
}

float IMU::getPitch() const {
    return _pitch;
}

float IMU::getYaw() const {
    return _yaw;
}

void IMU::setYaw(float yawDeg) {
    _yaw = yawDeg;
    if (_yaw > 180.0f) _yaw -= 360.0f;
    if (_yaw < -180.0f) _yaw += 360.0f;
}

bool IMU::isReady() const {
    return _ready;
}

void IMU::calibrateGyro() {
    // Promedio simple para offsets (sensor quieto)
    const int samples = 500;
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;

    for (int i = 0; i < samples; i++) {
        sensors_event_t a, g, t;
        _mpu.getEvent(&a, &g, &t);
        sumX += g.gyro.x;
        sumY += g.gyro.y;
        sumZ += g.gyro.z;
        delay(5);
    }

    _gyroXOffset = sumX / samples;
    _gyroYOffset = sumY / samples;
    _gyroZOffset = sumZ / samples;
}
