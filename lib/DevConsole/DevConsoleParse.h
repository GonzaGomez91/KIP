#pragma once

#include <stddef.h>

// Helpers de parseo para comandos DevConsole (sin malloc)

// Compara textos sin distinguir mayusculas/minusculas (ASCII basico)
static inline bool devEqualsIgnoreCase(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b) {
        char ca = *a;
        char cb = *b;
        if (ca >= 'a' && ca <= 'z') ca = (char)(ca - 'a' + 'A');
        if (cb >= 'a' && cb <= 'z') cb = (char)(cb - 'a' + 'A');
        if (ca != cb) return false;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

// Extrae el siguiente token separado por espacios
// Devuelve puntero al resto de la cadena (o nullptr si no hay mas)
static inline const char* devNextToken(const char* s, char* out, size_t outSize) {
    if (!out || outSize == 0) return nullptr;
    out[0] = '\0';
    if (!s) return nullptr;

    while (*s == ' ') s++;
    if (*s == '\0') return nullptr;

    size_t i = 0;
    while (*s && *s != ' ' && i < (outSize - 1)) {
        out[i++] = *s++;
    }
    out[i] = '\0';

    while (*s == ' ') s++;
    return s;
}
