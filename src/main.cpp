#include <Arduino.h>
#include "controlStepper.h"
#include "conectividad.h"
#include "actuadores.h"
#include "rutinas.h"
enum Estados
{
  Inicializando,
  EsperandoRutina,
  EnRutina,
  Calibracion
};
Estados estadoActual;
Rutina rutinaActual;
void setup()
{
  Serial.begin(115200);

  velocidadCarrusel = 250;
  velocidadYugo = 400;
  aceleracionCarrusel = 150;
  aceleracionYugo = 100;

  estadoActual = Inicializando;

  /*
    Los pines de sensores son:
    Carrusel Sensor A = 32
    Carrusel Sensor B = 33
    Yugo Sensor = 13

    Los pines de los motores son:
    Stepper del Carrusel: 18,19,21,22
    Stepper del Yugo: 23,25,26,27

    Pin del ventilador: 15
    Pin del buzzer: 5

    Dirección IP: http://192.168.4.1
    Red: Tinnidor
    Contraseña: 12345678
  */
}

void loop()
{
  AtenderConexion(); // se conecta en cada vuelta
  switch (estadoActual)
  {
  case Inicializando:
    ConfigurarActuadores();
    ConfigurarMotores();
    InicializarMotores();
    IniciarConexionWiFi();
    CrearServerPropio();
    estadoActual = EsperandoRutina;

    break;
  case EsperandoRutina:
    if (HayRutinaPendiente())
    {
      ObtenerRutina(rutinaActual);
      estadoActual = EnRutina;
    }
    else if (GetCommand().tipo == EntrarCalib)
    {
      estadoActual = Calibracion;
    }
    break;
  case EnRutina:
    if (EjecutarRutina(rutinaActual))
    {
      MoverMotor(YUGO, Norte);
      estadoActual = EsperandoRutina; 
    }
    else estadoActual = EsperandoRutina;
    break;
  case Calibracion:
  {
    Comando c = GetCommand();
    switch (c.tipo)
    {
    case MoverCalib:
      MoverPaso(c.motor, c.pasos);
      break;
    case FijarCalib:
      FijarNorte(c.motor);
      break;
    case SalirCalib:
      estadoActual = EsperandoRutina;
      break;
    default:
      break;
    }
  }
  break;
  }
}
