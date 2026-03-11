#include "DevConsoleRegistry.h"
#include "DevConsole.h"
#include "Sonar.h"

// Aqui iremos agregando registros de modulos y comandos
void DevConsoleRegistry_registerAll() {
    // Registro de modulos para INFO (estado inicial OFF)
    devRegisterModule("SENSOR", "SONAR");

    // Registro de comandos por modulo
    devRegisterCommand("SONAR", Sonar::devCommand);
}
