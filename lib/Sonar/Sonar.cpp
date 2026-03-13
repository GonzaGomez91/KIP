#include  "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(int triggerPin, int echoPin, ServoMotor& servo)
    : _triggerPin(triggerPin),
      _echoPin(echoPin),
      _servo(servo) {
}

// Inicializa los pines del sonar
void Sonar::init() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

// Funcionamiento del sensor
int Sonar::getDistance() {
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
        int distance = getDistance();

        // Guardar la distancia medida
        _distances[i] = distance;
        

    }

    // Volver el servo a la posición central después del barrido
    _servo.moveInstant((MIN_ANGLE + MAX_ANGLE) / 2);

    // Devolver el array de distancias medidas para depuración
    Serial.println("Distancias medidas en el barrido:");
    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        Serial.print("Ángulo ");
        Serial.print(MIN_ANGLE + i * angleStep);
        Serial.print("°: ");
        if (_distances[i] >= 0) {
            Serial.print(_distances[i]);
            Serial.println(" cm");
        } else {
            Serial.println("Error o fuera de rango");
        }
    }
}


