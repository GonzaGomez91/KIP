#include  "Sonar.h"
#include "DevConsole.h"
#include "DevConsoleParse.h"
#include <Arduino.h>

// Instancia activa para DevConsole (se asigna en init)
static Sonar* _devActiveSonar = nullptr;

Sonar::Sonar(int triggerPin, int echoPin, ServoMotor& servo)
    : _triggerPin(triggerPin),
      _echoPin(echoPin),
      _servo(servo) {
}

// Inicializa los pines del sonar
void Sonar::init() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);

    // Marcamos el modulo como listo una vez inicializado
    devSetModuleStatus("SENSOR", "SONAR", "READY");

    // Registramos esta instancia como activa para DevConsole
    _devActiveSonar = this;
}

// Funcionamiento del sensor
int Sonar::medirDistancia() {
    // Enviar pulso de trigger
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

    // Medir el tiempo del pulso de echo
    unsigned long duration = pulseIn(_echoPin, HIGH, 30000);

    if (duration == 0) {
        return -1; // Indica timeout o error
    }   

    // Convertir a distancia (cm)
    int distance = duration / 58;

    
    return distance;
}




// Realiza un barrido girando el servo y midiendo distancias en diferentes ángulos
void Sonar::barrido() {
    // Evitar división por cero
    int steps = (NUM_MEASUREMENTS > 1) ? (NUM_MEASUREMENTS - 1) : 1;
    int angleStep = (MAX_ANGLE - MIN_ANGLE) / steps;

    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        int targetAngle = MIN_ANGLE + i * angleStep;

        // Mover el servo a la posición deseada
        _servo.moveInstant(targetAngle); 

        // Esperar a que termine de moverse (barrido bloqueante por diseño)
        while (_servo.isMoving()) {
            _servo.update();
        }

        // Pequeña pausa para estabilizar la lectura (bloqueante)
        delay(1000);

        // Tomar la medición
        int distance = medirDistancia();

        // Guardar la distancia medida
        _distances[i] = distance;
        

    }

    // Volver el servo a la posición central después del barrido
    _servo.moveInstant((MIN_ANGLE + MAX_ANGLE) / 2);

    // Nota: el reporte de resultados se hace desde DevConsole (SONAR SCAN)
}

// =======================================
// DevConsole - comando SONAR
// =======================================

void Sonar::devCommand(const char* args) {
    if (!_devActiveSonar) {
        devSend("ERR", "SONAR", "NOT_READY");
        return;
    }

    char cmd[16];
    const char* p = devNextToken(args, cmd, sizeof(cmd));
    if (!p || cmd[0] == '\0') {
        devSend("ERR", "SONAR", "MISSING_ARGS");
        return;
    }

    if (devEqualsIgnoreCase(cmd, "READ")) {
        int cm = _devActiveSonar->medirDistancia();
        if (cm < 0) {
            devSend("ERR", "SONAR", "TIMEOUT");
        } else {
            char buf[12];
            itoa(cm, buf, 10);
            devSend("DATA", "SONAR", buf);
        }
        return;
    }

    if (devEqualsIgnoreCase(cmd, "SCAN")) {
        _devActiveSonar->barrido();
        
        // Devolver mediciones del ultimo barrido
        const int* dist = _devActiveSonar->getDistanciasBarrido();
        int steps = (NUM_MEASUREMENTS > 1) ? (NUM_MEASUREMENTS - 1) : 1;
        int angleStep = (MAX_ANGLE - MIN_ANGLE) / steps;

        for (int i = 0; i < NUM_MEASUREMENTS; i++) {
            int angle = MIN_ANGLE + i * angleStep;
            char buf[24];

            // Formato: ANGULO:CM  (o ANGULO:ERR)
            if (dist[i] >= 0) {
                snprintf(buf, sizeof(buf), "%d:%d", angle, dist[i]);
            } else {
                snprintf(buf, sizeof(buf), "%d:ERR", angle);
            }

            devSend("DATA", "SONAR_SCAN", buf);
        }

        devSend("OK", "SONAR_SCAN", "");
        return;
    }

    devSend("ERR", "SONAR", "UNKNOWN_ARG");
}


