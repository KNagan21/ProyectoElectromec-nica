#pragma once
#include <stdint.h>
#include "controlStepper.h"
#include "actuadores.h"
const uint8_t pasos_max = 10;
const long tiempo_min = 1;
const long tiempo_max = 3600;

struct Paso{
    Posicion vaso;
    uint32_t inmersion_ms;
    uint32_t escurrido_ms;
};

struct Rutina{
    Paso pasos[pasos_max];
    uint8_t numPasos;
};

bool AgregarPaso(Rutina &r, int cesta, long inmersion_s, long escurrido_s);
bool EjecutarRutina(const Rutina &r);