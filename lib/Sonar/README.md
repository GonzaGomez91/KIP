# Sonar

## Objetivo
Leer distancias con un HC-SR04 y permitir un barrido angular usando un servo
para mapear el entorno.

## Archivos
- `Sonar.h`
- `Sonar.cpp`

## Estado actual
- Funcional y en uso desde `main.cpp`.
- El barrido es **bloqueante** por diseño (aceptado en esta etapa).

## Uso básico
1. Incluir el header:
   ```cpp
   #include "Sonar.h"
   ```
2. Crear el sonar (requiere un servo de `ServoManager`):
   ```cpp
   Sonar sonar(7, 6, servos.sonarMount());
   ```
3. Inicializar en `setup()`:
   ```cpp
   sonar.init();
   ```
4. Medir distancia puntual:
   ```cpp
   int cm = sonar.medirDistancia();
   ```
5. Ejecutar un barrido:
   ```cpp
   sonar.barrido();
   const int* distancias = sonar.getDistanciasBarrido();
   ```

## Notas didácticas
- `medirDistancia()` usa `pulseIn()` con timeout para evitar bloqueos infinitos.
- `barrido()` mueve el servo, espera que termine y toma lecturas. Esto pausa
  el `loop()` mientras se ejecuta.
