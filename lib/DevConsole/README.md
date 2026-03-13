# DevConsole

## Objetivo
Proveer una consola de desarrollo por Serial para depurar y controlar KIP
sin usar memoria dinamica.

## Conceptos clave
- **Formato de respuesta:** `TIPO|CLAVE|VALOR`
- **Registro de modulos:** los modulos se registran con estado inicial `OFF`
- **Estado del modulo:** cuando un modulo se inicializa correctamente, cambia
  a `READY` llamando a `devSetModuleStatus()`

## Registro de modulos (flujo recomendado)
1. Registrar el modulo en el registry (estado inicial `OFF`):
   ```cpp
   // DevConsoleRegistry_registerAll()
   devRegisterModule("SENSOR", "SONAR");
   ```
2. Al finalizar `init()` del modulo, marcarlo como listo:
   ```cpp
   // Sonar::init()
   devSetModuleStatus("SENSOR", "SONAR", "READY");
   ```

## Registro de futuros modulos (pasos)
1. Agregar el modulo en `DevConsoleRegistry_registerAll()` con `devRegisterModule()`.
2. Registrar el comando del modulo con `devRegisterCommand()` apuntando al handler.
3. Dentro del `init()` del modulo, llamar a `devSetModuleStatus(..., "READY")`.
4. Si el modulo no esta listo, el handler debe responder `ERR|...|NOT_READY`.

## Ejemplo completo (referencia)
```cpp
// Registro centralizado
void DevConsoleRegistry_registerAll() {
    devRegisterModule("SENSOR", "SONAR");
    devRegisterCommand("SONAR", Sonar::devCommand);
}

// Modulo concreto
void Sonar::init() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    devSetModuleStatus("SENSOR", "SONAR", "READY");
}
```

## Comandos base
- `PING`: verifica que la consola responde.
- `HELP`: lista comandos registrados.
- `INFO`: lista modulos con su estado actual.

## Comandos Sonar (actual)
- `SONAR READ`: mide distancia y responde `DATA|SONAR|<cm>` o `ERR|SONAR|TIMEOUT`.
- `SONAR SCAN`: realiza barrido y responde varias lineas:
  - `DATA|SONAR_SCAN|ANGULO:CM`
  - `DATA|SONAR_SCAN|ANGULO:ERR`
  - `OK|SONAR_SCAN|`
