# Guia de conexiones (pinout)

Este archivo resume las conexiones actuales para evitar confusiones al cablear.
Si cambias un pin en el codigo, actualiza este archivo.

## Arduino UNO

### Servos
- **HEAD_SHUTTER**: pin digital **9**
- **SONAR_MOUNT**: pin digital **10**

### Sonar HC-SR04
- **TRIG**: pin digital **7**
- **ECHO**: pin digital **6**

### Matriz LED (MD_MAX72XX)
- **CS / LOAD**: pin digital **2**
- **DIN**: pin digital **11**
- **CLK**: pin digital **13**
- **VCC**: 5V
- **GND**: GND

### Otros
- **Potenciometro** (si se usa): **A0**
- **Semilla aleatoria**: **A1** (analogRead para randomSeed)

## Notas
- Si un modulo cambia de pins, actualizar tambien el README del modulo.
- Mantener los nombres de servos consistentes con DevConsole:
  `HEAD_SHUTTER` y `SONAR_MOUNT`.
