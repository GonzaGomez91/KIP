#ifndef ESTADOS_H
#define ESTADOS_H

#include <Arduino.h>

// Enumeración de los estados posibles de Kippy
// Cada valor representa un "modo de comportamiento"
enum EstadoKippy {
  STATE_SLEEP,   // Dormido: mínimo consumo y mínima actividad
  STATE_IDLE,    // Despierto pero tranquilo, esperando interacción
  STATE_ACTIVE   // Activo: atento y respondiendo
};

// Inicializa el sistema de estados
// Se llama una sola vez desde setup()
void initEstados();

// Actualiza la máquina de estados
// Se llama continuamente desde loop()
void updateEstados();

// Notifica que ocurrió una interacción
// Cualquier sensor o evento debe llamar a esta función
void notifyInteraccion();

EstadoKippy getEstadoActual();

#endif
