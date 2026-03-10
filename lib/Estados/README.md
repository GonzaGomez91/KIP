# Estados

## Objetivo
Implementar la máquina de estados de KIP para manejar transiciones entre:
`SLEEP`, `IDLE` y `ACTIVE`.

## Archivos
- `Estados.h`
- `Estados.cpp`

## Estado actual
- Funcional y en uso desde `main.cpp`.

## Uso básico
1. Incluir el header:
   ```cpp
   #include "Estados.h"
   ```
2. Inicializar una vez en `setup()`:
   ```cpp
   initEstados();
   ```
3. Actualizar en cada iteración del `loop()`:
   ```cpp
   updateEstados();
   ```
4. Consultar el estado actual:
   ```cpp
   EstadoKippy estado = getEstadoActual();
   ```
5. Notificar actividad (botón, sensor, comando):
   ```cpp
   notifyInteraccion();
   ```

## Notas didácticas
- La lógica usa `millis()` para medir el tiempo sin bloquear el `loop()`.
- Si no hay interacción durante cierto tiempo:
  - `ACTIVE` pasa a `IDLE`
  - `IDLE` pasa a `SLEEP`
