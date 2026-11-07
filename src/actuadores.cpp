#include "actuadores.h"
#include <Arduino.h>
static const int pinVentilador = 15;
static const int pinBuzzer = 5;
void EncenderVentilador(int duracion_ms)
{
    digitalWrite(pinVentilador, HIGH);
    delay(duracion_ms);
    digitalWrite(pinVentilador, LOW);
}

void SonarAlarma(int repeticiones)
{
    for (int i = 0; i < repeticiones; i++)
    {
        digitalWrite(pinBuzzer, HIGH);
        delay(500);
        digitalWrite(pinBuzzer, LOW);
        delay(500);
    }
}

void ConfigurarActuadores()
{
    pinMode(pinVentilador, OUTPUT);
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinVentilador, LOW);
    digitalWrite(pinBuzzer, LOW);
}