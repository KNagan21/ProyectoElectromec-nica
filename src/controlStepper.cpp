#include "controlStepper.h"
//Definicion de los pines de los motores

const u_int8_t CarrPin1 = 18;
const u_int8_t CarrPin2 = 19;
const u_int8_t CarrPin3 = 21;
const u_int8_t CarrPin4 = 22;

const u_int8_t YugoPin1 = 23;
const u_int8_t YugoPin2 = 25;
const u_int8_t YugoPin3 = 26;
const u_int8_t YugoPin4 = 27;

// Se crean los motores en la clase.
AccelStepper stepperCarrusel(4, CarrPin1, CarrPin3, CarrPin2, CarrPin4);
AccelStepper stepperYugo(4, YugoPin1, YugoPin3,YugoPin2,YugoPin4);

int velocidadCarrusel;
int velocidadYugo;
int aceleracionYugo;
int aceleracionCarrusel;

static const u_int8_t SensorCarruselA = 32;
static const u_int8_t SensorCarruselB = 33;
static const u_int8_t SensorYugo = 13;

void ConfigurarMotores()
{
    pinMode(SensorCarruselA, INPUT_PULLUP);
    pinMode(SensorCarruselB, INPUT_PULLUP);
    pinMode(SensorYugo, INPUT_PULLUP);
    stepperCarrusel.setMaxSpeed(velocidadCarrusel);
    stepperCarrusel.setAcceleration(aceleracionCarrusel);
    stepperYugo.setMaxSpeed(velocidadYugo);
    stepperYugo.setAcceleration(aceleracionYugo);
}

void MoverMotor(Motores motor, Posicion pos)
{
    const float pasos_por_vuelta = 2048;
    const float pasos90 = pasos_por_vuelta / 4; // 512 pasos = 90 grados
    switch (motor)
    {
    case YUGO:
        long objetivo;
        if (pos == Norte)
            objetivo = 0; // arriba
        else if (pos == Sur)
            objetivo = pasos90; // abajo
        else
            return;
        stepperYugo.moveTo(objetivo);
        stepperYugo.runToPosition();
        break;

    case CARRUSEL:
        long actual = int(stepperCarrusel.currentPosition() / pasos90) % 4;
        long delta = (pos - actual + 4) % 4;

        stepperCarrusel.move(delta * pasos90);
        stepperCarrusel.runToPosition();
        break;
    }
}
void InicializarMotores()
{
    MoverMotor(YUGO, Norte);
    MoverMotor(CARRUSEL, Norte);
}

static AccelStepper &MotorDe(Motores motor)
{
    return (motor == YUGO) ? stepperYugo : stepperCarrusel;
}

void MoverPaso(Motores motor, int pasos)
{
    AccelStepper &m = MotorDe(motor);
    m.move(pasos);
    m.runToPosition();
}

void FijarNorte(Motores motor)
{
    MotorDe(motor).setCurrentPosition(0);
}

bool CheckCarrusel(Posicion pos)
{
    const bool a = digitalRead(SensorCarruselA);  // 12
    const bool b = digitalRead(SensorCarruselB);  // 9
    switch (pos)
    {
    case Norte: return !a && !b;   // 0,0
    case Este:  return !a &&  b;   // 0,1
    case Sur:   return  a &&  b;   // 1,1
    case Oeste: return  a && !b;   // 1,0
    }
    return false;
}
bool CheckYugo(Posicion pos)
{
    bool sensor = digitalRead(SensorYugo);
    switch (pos)
    {
    case Norte:
        if (!(sensor))
            return true;
        break;
    case Sur:
        if (sensor)
            return true;
    default:
        break;
    }
    return false;
}
