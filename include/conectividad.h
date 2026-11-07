#ifndef conectividad
#define conectividad

#include "controlStepper.h"
#include "rutinas.h"
enum TipoComando{
    SinComando,
    EntrarCalib,
    SalirCalib,
    MoverCalib,
    FijarCalib
};

struct Comando{
    TipoComando tipo;
    Motores motor;
    int pasos;
};

Comando GetCommand();
void IniciarConexionWiFi();
void CrearServerPropio();
void AtenderConexion() ;
bool HayRutinaPendiente();
void ObtenerRutina(Rutina &destino);

#endif