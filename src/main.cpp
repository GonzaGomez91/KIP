#include <Arduino.h>
#include "Estados.h"
#include "Matriz.h"
#include "ServoManager.h"
#include "SerialTest.h"

ServoManager servos;

void setup() {
  Serial.begin(9600);

  initEstados();
  initMatriz();
  servos.init();

  SerialTest_init(&servos);

  Serial.println("=== Sistema listo ===");
}

void loop() {

  if (SerialTest_isPotMode()) {
      int ang = SerialTest_getPotAngle();
      servos.headShutter().moveInstant(ang);
  }

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
  SerialTest_update();
}
