#include "SerialTest.h"
#include <Arduino.h>

static ServoManager* _servos = nullptr;

// ---- Estado interno ----
static bool _potMode = false;
static int _potPin = A0;
static int _potMin = 0;
static int _potMax = 180;

// ---- Inicialización ----
void SerialTest_init(ServoManager* sm) {
    _servos = sm;
}

// ---- Funciones públicas ----
bool SerialTest_isPotMode() {
    return _potMode;
}

int SerialTest_getPotAngle() {
    int val = analogRead(_potPin);
    float norm = val / 1023.0f;
    int ang = _potMin + (_potMax - _potMin) * norm;
    return ang;
}

// ---- Loop principal ----
void SerialTest_update() {

    if (!_servos) return;

    if (!Serial.available()) return;

    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) return;

    char mode = line.charAt(0);
    String args = "";
    if (line.length() > 2)
        args = line.substring(2);

    switch(mode) {

        case 'I': { // Instantáneo
            int ang = args.toInt();
            _servos->headShutter().moveInstant(ang);
            Serial.print("Instant: "); Serial.println(ang);
            break;
        }

        case 'C': { // Constante
            int space = args.indexOf(' ');
            int ang = args.substring(0, space).toInt();
            float speed = args.substring(space + 1).toFloat();
            _servos->headShutter().moveConstant(ang, speed);
            Serial.print("Constante: "); Serial.print(ang);
            Serial.print(" Vel: "); Serial.println(speed);
            break;
        }

        case 'O': { // Orgánico
            int first = args.indexOf(' ');
            int second = args.indexOf(' ', first + 1);
            int ang = args.substring(0, first).toInt();
            float maxSpeed = args.substring(first + 1, second).toFloat();
            float acc = args.substring(second + 1).toFloat();
            _servos->headShutter().moveOrganic(ang, maxSpeed, acc);
            Serial.print("Organico: "); Serial.print(ang);
            Serial.print(" VelMax: "); Serial.print(maxSpeed);
            Serial.print(" Acc: "); Serial.println(acc);
            break;
        }

        case 'P': { // Posición actual
            int pos = _servos->headShutter().getCurrentPos();
            Serial.print("Angulo actual: ");
            Serial.println(pos);
            break;
        }

        case 'A': { // Configurar potenciómetro (una sola lectura)
            int first = args.indexOf(' ');
            int second = args.indexOf(' ', first + 1);

            _potPin = args.substring(0, first).toInt();
            _potMin = args.substring(first + 1, second).toInt();
            _potMax = args.substring(second + 1).toInt();

            int ang = SerialTest_getPotAngle();
            _servos->headShutter().moveInstant(ang);

            Serial.println("Pot configurado y aplicado instantaneamente");
            break;
        }

        case 'M': { // Cambiar modo
            if (args == "P") {
                _potMode = true;
                Serial.println("Modo Potenciometro ACTIVADO");
            }
            else if (args == "S") {
                _potMode = false;
                Serial.println("Modo Serial ACTIVADO");
            }
            break;
        }

        default:
            Serial.println("Comandos:");
            Serial.println("I <angulo>");
            Serial.println("C <angulo> <vel>");
            Serial.println("O <angulo> <velMax> <acc>");
            Serial.println("P");
            Serial.println("A <pin> <min> <max>");
            Serial.println("M P  -> modo pot");
            Serial.println("M S  -> modo serial");
    }
}
