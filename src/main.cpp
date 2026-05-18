#include <Arduino.h>
#include "Estados.h"
#include "Matriz.h"
#include "ServoManager.h"
#include "Sonar.h"
#include "IMU.h"


ServoManager servos;
Sonar sonar(7, 6, servos.sonarMount());
IMU imu;

void abrirOjo();
void cerrarOjo(); 

void setup() {
  Serial.begin(9600);

  // ========= Modulo Estados =========
  initEstados();

  // ========= Modulo Matriz =========
  initMatriz();

  // ========= Modulo Servos =========
  servos.init();

  // ========= Modulo Sonar =========
  sonar.init(); // Configura pines del HC-SR04 antes de leerlo

  // ========= Modulo IMU =========
  if (imu.init()) {
    Serial.println("IMU_OK");
  } else {
    Serial.println("IMU_FAIL");
  }
  imu.setYaw(0.0f);
  imu.initInterrupt(2);

  Serial.println("=== Sistema listo ===");
}

void loop() {
  imu.updateEvents();
  
  if (imu.detectMotion()) {
    abrirOjo();
    Serial.println("Movimiento detectado: abriendo ojo");
    delay(2000); // Mantener el ojo abierto por 2 segundos
  } else {
    cerrarOjo();
    
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
  
  delay(10); // Pequeña pausa para evitar saturar el loop
}

void abrirOjo() {
  servos.headShutter().moveInstant(150); // Abrir completamente
}

void cerrarOjo() {
  servos.headShutter().moveInstant(15); // Cerrar completamente
}