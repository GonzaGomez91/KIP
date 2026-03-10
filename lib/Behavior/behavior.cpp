#include "behavior.h"
#include "Estados.h"

void behaviorInit() {
    // Inicialización de variables o estados si es necesario
}

void behaviorSleep();
void behaviorIdle();
void behaviorActive();

void behaviorUpdate() {
    // Aquí se implementa la lógica de comportamiento del robot
    // Se pueden usar estructuras de control para determinar qué acción tomar según el estado actual

    switch (getEstadoActual()) {
        case STATE_SLEEP:
            // Comportamiento en estado de reposo
            behaviorSleep();
            break;
        case STATE_IDLE:
            // Comportamiento en estado de espera
            behaviorIdle();
            break;
        case STATE_ACTIVE:
            // Comportamiento en estado activo
            behaviorActive();
            break;

        default:
            // Comportamiento por defecto o manejo de estados desconocidos
            break;
    }
}

void behaviorSleep() {
    // Implementa el comportamiento específico para el estado de reposo
    // Por ejemplo, reducir la actividad, apagar luces, etc.
    // Para evitar que el robot haga algo constantemente en este estado, podemos usar un temporizador
    // Se tendra en cuenta el tiempo transcurrido desde la última acción para decidir cuándo realizar una nueva acción en este estado
    // El intervalo de tiempo puede ser aleatorio entre 5000 y 8000 ms para hacer el comportamiento más natural y menos predecible, este se actualiza cada vez que se realiza una acción en este estado, para que el robot no haga algo constantemente, sino que tenga momentos de "reposo" más largos o más cortos de forma aleatoria
    static unsigned long ultimoSleepAction = 0;
    static unsigned long intervaloSleepAction = random(5000, 8000);

    if (millis() - ultimoSleepAction > intervaloSleepAction) {
        // Realiza una acción de reposo, por ejemplo, mostrar un patrón de luces suave
        // Aquí puedes llamar a una función que controle la matriz LED para mostrar un patrón específico para el estado de reposo
        // Por ejemplo: showSleepPattern();

        // Actualiza el tiempo de la última acción y el intervalo para la próxima acción
        ultimoSleepAction = millis();
        intervaloSleepAction = random(5000, 8000);
    }

}

void behaviorIdle() {
    // Implementa el comportamiento específico para el estado de espera
    // Por ejemplo, mostrar un patrón de luces, emitir sonidos suaves, etc.
}

void behaviorActive() {
    // Implementa el comportamiento específico para el estado activo
    // Por ejemplo, responder a comandos, mostrar patrones de luces activos, etc.
}

