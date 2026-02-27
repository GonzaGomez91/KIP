#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Matriz.h"

// ================= CONFIGURACIÓN DE HARDWARE =================

// Tipo de matriz (las genéricas suelen usar FC16)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Cantidad de módulos 8x8 conectados
#define MAX_DEVICES 4

// Pin CS / LOAD
#define CS_PIN 10

// VCC 5v, GND GND, DIN 11, CLK 13

// Tamaño total de la matriz
#define WIDTH  (MAX_DEVICES * 8)
#define HEIGHT 8

// Objeto controlador de la matriz
static MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Control de tiempo para animación idle
static unsigned long ultimoIdleUpdate = 0;
static const unsigned long INTERVALO_IDLE = 300; // ms entre frames

// ================= PROTOTIPOS DE FUNCIONES INTERNAS =================
static void sleepPattern();
static void idlePattern();
static void activePattern();
static void randomDots();
static void randomBlocks();
static void noiseEffect();

// ================= FUNCIONES PÚBLICAS =================

// Inicializa la matriz LED
void initMatriz() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 15); // Brillo de 0 a 15
  mx.clear();

  // Semilla para efectos aleatorios
  randomSeed(analogRead(A1));
}

// Actualiza el patrón según el estado
void updateMatriz(ModoMatriz modo) {

  switch(modo) {
    case MATRIZ_SLEEP:
      sleepPattern();
      break;
    case MATRIZ_IDLE:
      idlePattern();
      break;
    case MATRIZ_ACTIVE:
      activePattern();
      break;
  }
}


// ================= FUNCIONES INTERNAS =================

// Patrón para STATE_SLEEP
static void sleepPattern() {
    // Si todavía no pasó el intervalo, no hacemos nada
  if (millis() - ultimoIdleUpdate < 3000) {
    return;
  }

  // Actualizamos el tiempo del último frame
  ultimoIdleUpdate = millis();

  // Limpiamos la matriz y dibujamos un nuevo patrón
  mx.clear();

  randomDots();
}


// Patrón idle con efectos aleatorios
static void idlePattern() {
  // Si todavía no pasó el intervalo, no hacemos nada
  if (millis() - ultimoIdleUpdate < INTERVALO_IDLE) {
    return;
  }

  // Actualizamos el tiempo del último frame
  ultimoIdleUpdate = millis();

  // Limpiamos la matriz y dibujamos un nuevo patrón
  mx.clear();

  // Selecciona un efecto al azar
  int mode = random(0, 2);

  switch (mode) {
    case 0:
      randomDots();
      break;
    case 1:
      noiseEffect();
      break;
  }
}

// Patrón para STATE_ACTIVE
static void activePattern() {
  mx.clear(); // por ahora solo limpia la matriz, luego podés poner animaciones más activas
}

// ---------------- EFECTOS ----------------

// Dibuja puntos aleatorios en la matriz
static void randomDots() {
  for (int i = 0; i < 20; i++) {
    int x = random(0, WIDTH);
    int y = random(0, HEIGHT);
    mx.setPoint(y, x, true);
  }
}

// Dibuja bloques tipo glitch
static void randomBlocks() {
  int x0 = random(0, WIDTH - 3);
  int y0 = random(0, HEIGHT - 3);

  for (int x = x0; x < x0 + 3; x++) {
    for (int y = y0; y < y0 + 3; y++) {
      mx.setPoint(y, x, true);
    }
  }
}

// Ruido visual tipo interferencia
static void noiseEffect() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      mx.setPoint(y, x, random(0, 2));
    }
  }
}
