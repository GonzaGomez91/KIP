# IMU

## Objetivo
Proveer orientacion basica (roll, pitch, yaw) usando una MPU6050 y un
filtro complementario simple.

## Archivos
- `IMU.h`
- `IMU.cpp`

## Estado actual
- Implementacion POO basada en el algoritmo probado en Arduino IDE.
- Roll y pitch combinan acelerometro + giroscopio.
- Yaw solo usa giroscopio (deriva con el tiempo).
- Solo se calibra al iniciar (no hay recalibracion automatica).

## Uso basico
1. Incluir el header:
   ```cpp
   #include "IMU.h"
   ```
2. Crear el objeto:
   ```cpp
   IMU imu; // 100 Hz por defecto
   ```
3. Inicializar en `setup()`:
   ```cpp
   imu.init();
   ```
4. Actualizar en `loop()`:
   ```cpp
   if (imu.update()) {
       float r = imu.getRoll();
       float p = imu.getPitch();
       float y = imu.getYaw();
   }
   ```

## Notas didacticas
- El giroscopio se calibra al iniciar (sensor quieto).
- `update()` usa un periodo fijo para mantener una tasa estable.
