#ifndef controlStepper
#define controlStepper

#include <AccelStepper.h>

enum Motores
{
    YUGO,
    CARRUSEL
};

enum Posicion
{
    Norte, 
    Este,
    Sur,
    Oeste
};

extern int velocidadCarrusel;
extern int velocidadYugo;
extern int aceleracionYugo;
extern int aceleracionCarrusel;


void ConfigurarMotores();  // Declara los motores con la librería accelstepper
void InicializarMotores(); // Inicializa los motores moviendo el yugo arriba y despues el carrusel al norte
void MoverMotor(Motores motor, Posicion pos);

bool CheckCarrusel(Posicion pos);
bool CheckYugo(Posicion pos);

const int PASOS_CALIB_MAX = 100;           // límite de seguridad por comando

void MoverPaso(Motores motor, int pasos);  // relativo, con signo
void FijarNorte(Motores motor);            // la posición actual pasa a ser 0
#endif