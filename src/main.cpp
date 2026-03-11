#include <Arduino.h>
#include "Estados.h"
#include "Matriz.h"
#include "ServoManager.h"
#include "Sonar.h"
#include "DevConsole.h"


ServoManager servos;
Sonar sonar(7, 6, servos.sonarMount());

void setup() {
  Serial.begin(9600);

  initEstados();
  initMatriz();
  servos.init();
  sonar.init(); // Configura pines del HC-SR04 antes de leerlo
  DevConsole_init();

  Serial.println("=== Sistema listo ===");
}

void loop() {

  updateEstados();

  EstadoKippy estado = getEstadoActual();

  ModoMatriz modo;

  switch (estado) {
    case STATE_SLEEP:  modo = MATRIZ_SLEEP;  break;
    case STATE_IDLE:   modo = MATRIZ_IDLE;   break;
    case STATE_ACTIVE: modo = MATRIZ_ACTIVE; break;
  }

  updateMatriz(modo);

  servos.update();
  DevConsole_update();
}
