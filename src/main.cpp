#include <Arduino.h>
#include "Estados.h"
#include "Matriz.h"
#include "ServoManager.h"
#include "Sonar.h"
#include "IMU.h"


ServoManager servos;
Sonar sonar(7, 6, servos.sonarMount());
IMU imu;

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

  if (imu.update()) {
    Serial.print("roll:");
    Serial.print(imu.getRoll());
    Serial.print(" pitch:");
    Serial.print(imu.getPitch());
    Serial.print(" yaw:");
    Serial.print(imu.getYaw());
    Serial.print(" upper:");
    Serial.print(90);
    Serial.print(" lower:");
    Serial.println(-90);
  }

  servos.update();
}
