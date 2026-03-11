#include "DevConsoleRegistry.h"
#include "DevConsole.h"
#include "Sonar.h"
#include "ServoManager.h"
#include "DevConsolePot.h"

// Aqui iremos agregando registros de modulos y comandos
void DevConsoleRegistry_registerAll() {
    // Registro de modulos para INFO (estado inicial OFF)
    devRegisterModule("SENSOR", "SONAR");
    devRegisterModule("SERVO", "HEAD_SHUTTER");
    devRegisterModule("SERVO", "SONAR_MOUNT");

    // Registro de comandos por modulo
    devRegisterCommand("SONAR", Sonar::devCommand);
    devRegisterCommand("SERVO", ServoManager::devCommand);
    devRegisterCommand("POT", DevConsolePot_command);
}
