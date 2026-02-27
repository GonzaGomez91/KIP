#ifndef MATRIZ_H
#define MATRIZ_H

#include <Arduino.h>

// Inicializa la matriz LED
// Debe llamarse una sola vez desde setup()
void initMatriz();

enum ModoMatriz {
  MATRIZ_SLEEP,
  MATRIZ_IDLE,
  MATRIZ_ACTIVE
};

void updateMatriz(ModoMatriz modo);
#endif
