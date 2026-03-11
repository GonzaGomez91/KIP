#pragma once

// DevConsole: definiciones publicas y configuracion basica

#include <Arduino.h>

// ==============================
// Configuracion basica (sin malloc)
// ==============================

static const uint8_t DEV_MAX_MODULES  = 8;
static const uint8_t DEV_MAX_COMMANDS = 12;
static const uint8_t DEV_MAX_LINE     = 64;

// ==============================
// Tipos expuestos por la API
// ==============================

// Handler de comandos: recibe el texto que sigue al comando
// Ejemplo: "PING" -> args = ""
// Ejemplo: "SERVO SET 90" -> args = "SET 90"
typedef void (*DevCommandHandler)(const char* args);

// Modulo registrado (para INFO)
struct DevModule {
    char tipo[12];
    char nombre[12];
    char estado[12]; // OFF, READY, ERROR, etc.
};

// Comando registrado (nombre + handler)
struct DevCommand {
    char nombre[12];
    DevCommandHandler fn;
};

// ==============================
// API publica
// ==============================

// Inicializa el estado interno del DevConsole
void DevConsole_init();

// Debe llamarse en cada iteracion del loop
void DevConsole_update();

// Registra un modulo (para el comando INFO)
bool devRegisterModule(const char* tipo, const char* nombre);

// Actualiza el estado de un modulo ya registrado
bool devSetModuleStatus(const char* tipo, const char* nombre, const char* estado);

// Registra un comando (nombre + handler)
bool devRegisterCommand(const char* nombre, DevCommandHandler fn);

// Envia una respuesta con el formato: TIPO|CLAVE|VALOR
void devSend(const char* tipo, const char* clave, const char* valor);
