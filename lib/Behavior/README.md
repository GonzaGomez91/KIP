# Behavior

## Objetivo
Definir la lógica de comportamiento de KIP en función del estado actual del sistema.

Este módulo está pensado para centralizar “qué hace” el robot cuando está en
`SLEEP`, `IDLE` o `ACTIVE`, sin depender todavía de objetos complejos.

## Archivos
- `behavior.h`
- `behavior.cpp`

## Estado actual
- En desarrollo.
- No está integrado en `main.cpp` todavía (se llama a `Estados` y `Matriz`, pero
  `behaviorUpdate()` no se ejecuta).

## Uso básico (cuando se integre)
1. Incluir el header:
   ```cpp
   #include "behavior.h"
   ```
2. Inicializar en `setup()`:
   ```cpp
   behaviorInit();
   ```
3. Actualizar en `loop()`:
   ```cpp
   behaviorUpdate();
   ```

## Notas didácticas
- El comportamiento se decide con un `switch` sobre `getEstadoActual()`.
- Las funciones `behaviorSleep()`, `behaviorIdle()` y `behaviorActive()` permiten
  aislar acciones por estado y mantener el código claro.
