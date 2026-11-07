#include "rutinas.h"
#include <Arduino.h>
bool AgregarPaso(Rutina &r, int cubeta, long inmersion_s, long escurrido_s)
{
    if (r.numPasos >= pasos_max)
        return false;
    if (cubeta < 1 || cubeta > 4)
        return false;
    if (inmersion_s < tiempo_min || inmersion_s > tiempo_max)
        return false;
    if (escurrido_s < tiempo_min || escurrido_s > tiempo_max)
        return false;

    Paso &p = r.pasos[r.numPasos];
    p.vaso = static_cast<Posicion>(cubeta - 1);
    p.inmersion_ms = inmersion_s * 1000UL;
    p.escurrido_ms = escurrido_s * 1000UL;
    r.numPasos++;
    return true;
}

bool EjecutarRutina(const Rutina &r)
{
    for (uint8_t i = 0; i < r.numPasos; i++)
    {
        const Paso &p = r.pasos[i];
        MoverMotor(YUGO, Norte); // sube
        /*Proceso de seguridad, pregunta si el motor está arriba antes de mover.
            Pregunta 5 veces, si no se soluciona entonces suena la alarma*/
        for (uint8_t i = 0; i < 5; i++)
        {
            if (CheckYugo(Norte))
            {
                break;
            }
            else
            {
                MoverMotor(YUGO, Norte);
                delay(100);
            }

            if (i == 4) // En el quinto intento, llamo la alarma
            {
                SonarAlarma(3);
                return false;
            }
        }

        MoverMotor(CARRUSEL, p.vaso); // gira
                                      // antes de bajar, confirma la posicion del yugo. Suena la alarma y aborta la rutina. Se puede mejorar en el futuro.
        /*for (uint8_t i = 0; i < 5; i++)
        {
            if (CheckCarrusel(p.vaso))
            {
                break;
            }
            else
            {
                delay(100);
            }

            if (i == 4) // En el quinto intento, llamo la alarma
            {
                SonarAlarma(3);
                return false;
            }
        }*/

        MoverMotor(YUGO, Sur);   // baja
        delay(p.inmersion_ms);   // inmerso
        MoverMotor(YUGO, Norte); // sube

        for (uint8_t i = 0; i < 5; i++)
        {
            if (CheckYugo(Norte))
                break;
            else
                MoverMotor(YUGO, Norte);
            delay(100);

            if (i == 4) // En el quinto intento, llamo la alarma
            {
                SonarAlarma(3);
                return false;
            }
        }

        delay(p.escurrido_ms); // escurre
    }

    // Al finalizar la rutina, se inicia el secado corto, 3s digamos

    EncenderVentilador(3000);
    return true;
}