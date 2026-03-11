#include "DevConsole.h"
#include "DevConsoleRegistry.h"
#include "DevConsoleParse.h"
#include "DevConsolePot.h"

// =======================================
// Estado interno (todo estatico, sin malloc)
// =======================================

static DevModule  _modules[DEV_MAX_MODULES];
static uint8_t    _moduleCount = 0;

static DevCommand _commands[DEV_MAX_COMMANDS];
static uint8_t    _commandCount = 0;

static char       _lineBuf[DEV_MAX_LINE];
static uint8_t    _lineLen = 0;

// =======================================
// Comandos base (internos)
// =======================================

static void cmdPing(const char* args);
static void cmdHelp(const char* args);
static void cmdInfo(const char* args);
static void DevConsole_registerBaseCommands();

// =======================================
// Helpers internos (no visibles fuera del modulo)
// =======================================

// Busca un comando por nombre y devuelve su handler o nullptr
static DevCommandHandler findCommand(const char* name) {
    for (uint8_t i = 0; i < _commandCount; i++) {
        if (devEqualsIgnoreCase(_commands[i].nombre, name)) {
            return _commands[i].fn;
        }
    }
    return nullptr;
}

// Procesa una linea completa: separa comando y argumentos
static void processLine(char* line) {
    // Saltar espacios iniciales
    while (*line == ' ') line++;
    if (*line == '\0') return;

    // Separar en comando y args (primer espacio)
    char* args = nullptr;
    for (char* p = line; *p; p++) {
        if (*p == ' ') {
            *p = '\0';
            args = p + 1;
            break;
        }
    }
    if (!args) args = (char*)"";

    DevCommandHandler fn = findCommand(line);
    if (fn) {
        fn(args);
    } else {
        devSend("ERR", "UNKNOWN_COMMAND", "");
    }
}

// =======================================
// API publica - primera funcion
// =======================================

void DevConsole_init() {
    // Reiniciamos contadores
    _moduleCount = 0;
    _commandCount = 0;

    // Reiniciamos el buffer de entrada
    _lineLen = 0;
    _lineBuf[0] = '\0';

    // Registrar comandos base (PING, HELP, INFO, etc.)
    DevConsole_registerBaseCommands();

    // Registrar modulos y comandos del proyecto
    DevConsoleRegistry_registerAll();

    // Inicializar subsistemas internos
    DevConsolePot_init();
}

// Registra un modulo para que aparezca en INFO
bool devRegisterModule(const char* tipo, const char* nombre) {
    // Si no hay espacio, no registramos
    if (_moduleCount >= DEV_MAX_MODULES) {
        return false;
    }

    // Copia segura (evita overflow y asegura \0)
    strncpy(_modules[_moduleCount].tipo, tipo, sizeof(_modules[_moduleCount].tipo) - 1);
    _modules[_moduleCount].tipo[sizeof(_modules[_moduleCount].tipo) - 1] = '\0';

    strncpy(_modules[_moduleCount].nombre, nombre, sizeof(_modules[_moduleCount].nombre) - 1);
    _modules[_moduleCount].nombre[sizeof(_modules[_moduleCount].nombre) - 1] = '\0';

    // Estado inicial por defecto
    strncpy(_modules[_moduleCount].estado, "OFF", sizeof(_modules[_moduleCount].estado) - 1);
    _modules[_moduleCount].estado[sizeof(_modules[_moduleCount].estado) - 1] = '\0';

    _moduleCount++;
    return true;
}

// Actualiza el estado de un modulo ya registrado
bool devSetModuleStatus(const char* tipo, const char* nombre, const char* estado) {
    if (!tipo || !nombre || !estado) return false;

    for (uint8_t i = 0; i < _moduleCount; i++) {
        if (devEqualsIgnoreCase(_modules[i].tipo, tipo) &&
            devEqualsIgnoreCase(_modules[i].nombre, nombre)) {

            strncpy(_modules[i].estado, estado, sizeof(_modules[i].estado) - 1);
            _modules[i].estado[sizeof(_modules[i].estado) - 1] = '\0';
            return true;
        }
    }

    return false;
}

// Registra un comando con su handler
bool devRegisterCommand(const char* nombre, DevCommandHandler fn) {
    // Validaciones basicas
    if (_commandCount >= DEV_MAX_COMMANDS) {
        return false;
    }
    if (fn == nullptr) {
        return false;
    }

    // Copia segura del nombre del comando
    strncpy(_commands[_commandCount].nombre, nombre, sizeof(_commands[_commandCount].nombre) - 1);
    _commands[_commandCount].nombre[sizeof(_commands[_commandCount].nombre) - 1] = '\0';

    _commands[_commandCount].fn = fn;
    _commandCount++;
    return true;
}

// Envia una respuesta con formato: TIPO|CLAVE|VALOR
void devSend(const char* tipo, const char* clave, const char* valor) {
    // Si alguno es null, se envia vacio en ese campo
    Serial.print(tipo ? tipo : "");
    Serial.print('|');
    Serial.print(clave ? clave : "");
    Serial.print('|');
    Serial.println(valor ? valor : "");
}

// Procesa entrada por Serial sin bloquear
void DevConsole_update() {
    // Actualiza tareas internas que no bloquean
    DevConsolePot_update();

    while (Serial.available()) {
        char c = (char)Serial.read();

        // Ignorar CR para compatibilidad con terminales Windows
        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            // Fin de linea: terminamos el string y lo procesamos
            _lineBuf[_lineLen] = '\0';

            // Si la linea no esta vacia, la manejamos
            if (_lineLen > 0) {
                processLine(_lineBuf);
            }

            // Reiniciar buffer para la proxima linea
            _lineLen = 0;
            _lineBuf[0] = '\0';
            return; // procesar solo una linea por loop
        }

        // Guardar caracter si hay espacio
        if (_lineLen < (DEV_MAX_LINE - 1)) {
            _lineBuf[_lineLen++] = c;
        } else {
            // Si se llena el buffer, descartamos lo extra
            // Esperamos el fin de linea para reiniciar
        }
    }
}

// =======================================
// Implementacion de comandos base
// =======================================

static void DevConsole_registerBaseCommands() {
    devRegisterCommand("PING", cmdPing);
    devRegisterCommand("HELP", cmdHelp);
    devRegisterCommand("INFO", cmdInfo);
}

static void cmdPing(const char* args) {
    (void)args; // no se usan argumentos
    devSend("OK", "PING", "");
}

static void cmdHelp(const char* args) {
    (void)args; // no se usan argumentos

    // Lista los comandos registrados
    for (uint8_t i = 0; i < _commandCount; i++) {
        devSend("CMD", _commands[i].nombre, "");
    }
}

static void cmdInfo(const char* args) {
    (void)args; // no se usan argumentos

    // Lista los modulos registrados
    for (uint8_t i = 0; i < _moduleCount; i++) {
        // Enviamos: MOD|TIPO|NOMBRE:ESTADO
        // Ej: MOD|SENSOR|SONAR:READY
        char value[26];
        value[0] = '\0';

        // Construimos "NOMBRE:ESTADO" sin usar String
        strncat(value, _modules[i].nombre, sizeof(value) - 1);
        strncat(value, ":", sizeof(value) - strlen(value) - 1);
        strncat(value, _modules[i].estado, sizeof(value) - strlen(value) - 1);

        devSend("MOD", _modules[i].tipo, value);
    }
}
