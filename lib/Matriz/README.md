# Matriz

## Objetivo
Controlar la matriz LED (MD_MAX72xx) y mostrar un patrón visual en función
del estado del robot.

## Archivos
- `Matriz.h`
- `Matriz.cpp`

## Estado actual
- Funcional, pero **provisional**. Puede ser rediseñado más adelante.

## Uso básico
1. Incluir el header:
   ```cpp
   #include "Matriz.h"
   ```
2. Inicializar en `setup()`:
   ```cpp
   initMatriz();
   ```
3. Actualizar en `loop()` con el modo deseado:
   ```cpp
   updateMatriz(MATRIZ_IDLE);
   ```

## Modos disponibles
- `MATRIZ_SLEEP`
- `MATRIZ_IDLE`
- `MATRIZ_ACTIVE`

## Notas didácticas
- El módulo encapsula un `MD_MAX72XX` y genera efectos simples.
- `idlePattern()` usa un temporizador con `millis()` para no saturar el `loop()`.
