# Notas Del Proyecto KIP

## Estilo De Trabajo
- Este proyecto esta orientado al aprendizaje.
- Todo codigo nuevo debe priorizar legibilidad y comentarios utiles que expliquen la razon y el flujo.
- Se prefieren implementaciones claras y didacticas antes que soluciones demasiado compactas o rebuscadas.
- La documentacion interna del proyecto debe escribirse en espanol.
- Los comentarios, issues, tareas y textos de trabajo en GitHub deben mantenerse en espanol.

## Decisiones Actuales De Arquitectura
- El modulo `Behavior` sigue en desarrollo y no debe integrarse a la fuerza todavia.
- `ErrorManager` fue eliminado y sus responsabilidades se integraran en `DevConsole`.
- `SerialTest` fue eliminado y sera reemplazado por `DevConsole`.
- `Matriz` es provisional y en el futuro puede redisenarse, incluso como una implementacion basada en objetos.
- El barrido actual del sonar puede seguir siendo bloqueante por ahora para que el robot permanezca quieto durante la medicion.

## Reemplazo De Modulos
- En el futuro existira un modulo llamado `DevConsole` para reemplazar a `SerialTest`.
- `DevConsole` permitira que un dispositivo externo, como una Cardputer, controle y monitoree a KIP por Serial.
- Las responsabilidades de depuracion y reporte de errores convergen en `DevConsole` (reemplazo de `ErrorManager`).

## Documentacion
- Cada modulo dentro de `lib/` debe tener su propio `README.md` explicando objetivo, archivos, estado actual y forma de uso.
- Los `README.md` de los modulos deben ser claros, breves y escritos con enfoque didactico.
- Si un modulo esta incompleto o planificado para ser reemplazado, eso debe quedar explicitado en su `README.md`.

## Flujo De GitHub
- Al crear tareas en GitHub se debe respetar el template ubicado en `.github/ISSUE_TEMPLATE/nueva-tarea-del-proyecto-kip.md`.
- Las tareas deben mantenerse alineadas con el tablero del proyecto.
