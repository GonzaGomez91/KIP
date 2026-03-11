#include <unity.h>

// Ejemplo simple para mostrar el flujo sin hardware.
// Esto sirve como plantilla base para futuras pruebas.
static int sumar(int a, int b) {
    return a + b;
}

void test_sumar_basico() {
    TEST_ASSERT_EQUAL_INT(5, sumar(2, 3));
}

void test_sumar_negativos() {
    TEST_ASSERT_EQUAL_INT(-8, sumar(-3, -5));
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    UNITY_BEGIN();
    RUN_TEST(test_sumar_basico);
    RUN_TEST(test_sumar_negativos);
    return UNITY_END();
}
