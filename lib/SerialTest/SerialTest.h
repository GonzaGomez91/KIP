#ifndef SERIAL_TEST_H
#define SERIAL_TEST_H

#include "ServoManager.h"
#include "Sonar.h"

// Inicialización
void SerialTest_init(ServoManager* sm, Sonar* sonar);

// Se llama en el loop
void SerialTest_update();

// Estado del modo potenciómetro
bool SerialTest_isPotMode();

// Obtener ángulo actual del potenciómetro
int SerialTest_getPotAngle();



#endif
