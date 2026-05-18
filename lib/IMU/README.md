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
- Soporta interrupcion de movimiento (pin INT).

## Uso basico
1. Incluir el header:
   ```cpp
   #include "IMU.h"
   ```
2. Crear el objeto:
   ```cpp
   IMU imu;
   ```
3. Inicializar en `setup()`:
   ```cpp
   imu.init();
   ```
4. Pedir orientacion cuando se necesite:
   ```cpp
   if (imu.requestOrientation()) {
       float r = imu.getRoll();
       float p = imu.getPitch();
       float y = imu.getYaw();
   }
   ```

## Interrupcion de movimiento
1. Conectar `INT` del MPU6050 a un pin de interrupcion (UNO: 2 o 3).
2. Inicializar la interrupcion:
   ```cpp
   imu.initInterrupt(2);
   ```
3. En el loop, procesar eventos:
   ```cpp
   imu.updateEvents();
   if (imu.detectMotion()) {
       // reaccionar a movimiento
   }
   ```

## Notas didacticas
- El giroscopio se calibra al iniciar (sensor quieto).
- `update()` usa un periodo fijo para mantener una tasa estable.
