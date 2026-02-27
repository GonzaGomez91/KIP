#include "Estados.h"

// Estado actual de Kippy
// static limita su alcance a este archivo
static EstadoKippy estadoActual = STATE_SLEEP;

// Marca de tiempo de la última interacción
// Se usa para calcular los timeouts
static unsigned long ultimoEvento = 0;

// Tiempo máximo en estado ACTIVE antes de pasar a IDLE (en ms)
static const unsigned long TIMEOUT_IDLE = 10000;

// Tiempo máximo en estado IDLE antes de pasar a SLEEP (en ms)
static const unsigned long TIMEOUT_SLEEP = 30000;

// Inicialización de la máquina de estados
void initEstados() {
  estadoActual = STATE_IDLE;
  ultimoEvento = millis();
}

// Función principal de la máquina de estados
// Decide qué hacer según el estado actual
void updateEstados() {

  switch (estadoActual) {

    case STATE_SLEEP:
      // En sleep no hace casi nada
      // Solo espera que ocurra una interacción
      break;

    case STATE_ACTIVE:
      // Kippy está atento y funcionando normalmente

      // Si pasa cierto tiempo sin interacción, pasa a IDLE
      if (millis() - ultimoEvento > TIMEOUT_IDLE) {
        estadoActual = STATE_IDLE;
      }
      break;

    case STATE_IDLE:
      // Kippy está despierto pero relajado

      // Si pasa mucho tiempo sin interacción, se duerme
      if (millis() - ultimoEvento > TIMEOUT_SLEEP) {
        estadoActual = STATE_SLEEP;
      }
      break;
  }
}

//Funcion que devuelve el estado actual
EstadoKippy getEstadoActual() {
  return estadoActual;
}

// Esta función debe llamarse cuando ocurre cualquier interacción
// Por ejemplo: botón, toque, comando de voz, etc.
void notifyInteraccion() {
  // Actualizamos el tiempo del último evento
  ultimoEvento = millis();

  // Si estaba dormido o en idle, pasa a activo
  if (estadoActual == STATE_SLEEP || estadoActual == STATE_IDLE) {
    estadoActual = STATE_ACTIVE;
  }
}
