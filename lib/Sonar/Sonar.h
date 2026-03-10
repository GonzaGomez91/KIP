//Modulo para el sensor HC-SR04, que mide distancias usando ultrasonidos.
//El sensor estara montado en un servomotor para poder medir distancias en diferentes direcciones.
//El modulo funciona de la siguiente manera:
//1. Se envía un pulso de 10 microsegundos al pin de trigger para iniciar la medición.
//2. El sensor emite un pulso ultrasónico y espera a que este rebote y regrese al pin de echo.
//3. El tiempo que tarda el pulso en regresar se mide y se convierte a distancia usando la fórmula: distancia = (tiempo * velocidad_del_sonido) / 2
//4. La distancia se medira en centímetros, considerando que la velocidad del sonido es aproximadamente 343 metros por segundo (o 0.0343 cm/µs).
// Luego tendremos un metodo barrido que controlara el servo para girar el sensor y tomar medidas en diferentes direcciones, creando un mapa de distancias alrededor del robot,
// lo guarda en un array para que luego se pueda realizar un mapeo del entorno. La cantidad de mediciones, angulo minimo y maximo del barrido seran fijos.
// El modulo se integra con el ServoManager para controlar el servo del sonar y realizar el barrido. El servo que se utilizara sera siempre el mismo, no es necesario pasarlo como parametro.
#pragma once 
#include "ServoManager.h"
class Sonar {
public:
    // Constructor
    Sonar(int triggerPin, int echoPin, ServoMotor& servo);

    // Inicializa los pines del sonar
    void init();
    
    // Obtiene la distancia medida por el sonar en centímetros
    int getDistance();
    
    // Devuelve la cantidad de mediciones realizadas en el barrido
    static const int getMeasurementCount() { return NUM_MEASUREMENTS; }

    // Metodo para devolver el array de distancias medidas en el barrido
    const int* getDistances() const { return _distances; }
    
    // Realiza un barrido girando el servo y midiendo distancias en diferentes ángulos
    void barrido();


private:
    // Pines del sonar
    int _triggerPin;
    int _echoPin;
    ServoMotor& _servo; // Referencia al servo que controla el sonar

    
    static const int NUM_MEASUREMENTS = 5; // Cantidad de mediciones en el barrido
    int _distances[NUM_MEASUREMENTS]; // Array para almacenar las distancias medidas
    
    static const int MIN_ANGLE = 30;         // Ángulo mínimo del barrido
    static const int MAX_ANGLE = 150;       // Ángulo máximo del barrido


};