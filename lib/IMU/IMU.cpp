#include "IMU.h"

#include <Wire.h>

IMU* IMU::_activeInstance = nullptr;

IMU::IMU()
    : _mpu(),
      _ready(false),
      _lastUpdateMs(0),
      _roll(0.0f),
      _pitch(0.0f),
      _yaw(0.0f),
      _gyroXOffset(0.0f),
      _gyroYOffset(0.0f),
      _gyroZOffset(0.0f),
      _intPin(2),
      _intEnabled(false),
      _intFlag(false),
      _motionDetected(false),
      _motionThreshold(2),
      _motionDuration(1) {
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

bool IMU::requestOrientation() {
    if (!_ready) {
        return false;
    }

    unsigned long now = millis();
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

bool IMU::initInterrupt(uint8_t intPin) {
    if (!_ready) {
        return false;
    }

    _intPin = intPin;
    pinMode(_intPin, INPUT);

    _activeInstance = this;
    attachInterrupt(digitalPinToInterrupt(_intPin), IMU::isrThunk, RISING);

    // Configuracion basica de interrupcion por movimiento
    // MOT_THR (0x1F) y MOT_DUR (0x20)
    writeRegister(0x1F, _motionThreshold);
    writeRegister(0x20, _motionDuration);

    // INT_PIN_CFG (0x37): latch + clear on any read
    writeRegister(0x37, 0x30);

    // INT_ENABLE (0x38): habilitar MOT_INT (bit 6)
    writeRegister(0x38, 0x40);

    // Limpiar interrupcion pendiente
    readRegister(0x3A);

    _intEnabled = true;
    return true;
}

void IMU::updateEvents() {
    if (!_intEnabled) {
        return;
    }
    if (_intFlag) {
        _intFlag = false;
        // Leer INT_STATUS para limpiar latch
        uint8_t status = readRegister(0x3A);
        if (status & 0x40) {
            _motionDetected = true;
        }
    }
}

bool IMU::detectMotion() {
    if (_motionDetected) {
        _motionDetected = false;
        return true;
    }
    return false;
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

void IMU::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(0x68);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t IMU::readRegister(uint8_t reg) {
    Wire.beginTransmission(0x68);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)0x68, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}

void IMU::isrThunk() {
    if (IMU::_activeInstance) {
        IMU::_activeInstance->_intFlag = true;
    }
}
