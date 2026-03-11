#pragma once

// Modo potenciómetro para DevConsole
// Permite vincular un servo a un pot y mostrar el angulo en Serial

void DevConsolePot_init();
void DevConsolePot_update();
void DevConsolePot_command(const char* args);
