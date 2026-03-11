# ServoManager

## Objetivo
Gestionar los servos del robot con movimientos no bloqueantes y con distintos
modos de movimiento (instantáneo, constante y orgánico).

## Archivos
- `ServoManager.h`
- `ServoManager.cpp`

## Estado actual
- Funcional y en uso desde `main.cpp`.

## Uso básico
1. Incluir el header:
   ```cpp
   #include "ServoManager.h"
   ```
2. Crear una instancia global:
   ```cpp
   ServoManager servos;
   ```
3. Inicializar en `setup()`:
   ```cpp
   servos.init();
   ```
4. Actualizar en `loop()`:
   ```cpp
   servos.update();
   ```
5. Mover un servo:
   ```cpp
   servos.headShutter().moveInstant(90);
   ```

## Modos de movimiento
- `moveInstant(angulo)`: mueve inmediatamente.
- `moveConstant(angulo, velocidad)`: velocidad fija.
- `moveOrganic(angulo, velMax, aceleracion)`: acelera y frena suavemente.


## Como agregar un nuevo servo
1. Declarar el nuevo `ServoMotor` en `ServoManager` (en el `private`):
   ```cpp
   ServoMotor _nuevoServo{PIN, ANGULO_INICIAL, MIN, MAX};
   ```
2. Agregar un metodo publico de acceso:
   ```cpp
   ServoMotor& nuevoServo();
   ```
3. Inicializarlo en `ServoManager::init()`:
   ```cpp
   _nuevoServo.init();
   ```
4. Actualizarlo en `ServoManager::update()`:
   ```cpp
   _nuevoServo.update();
   ```
5. Si usa DevConsole:
   - Registrar el modulo en `DevConsoleRegistry_registerAll()`.
   - Agregar el ID en el handler `SERVO`.
   - Marcar `READY` al inicializar.

## Notas didácticas
- El cálculo del movimiento usa `millis()` para evitar bloqueos.
- `ServoMotor::update()` debe llamarse con frecuencia para que el movimiento
  sea fluido.




