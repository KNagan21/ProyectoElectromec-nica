#include <WiFi.h>
#include <WebServer.h>
#include "conectividad.h"
#include "rutinas.h"
#include "pagina.h"

static WebServer server(80);
static Rutina pendiente; // el "buzón": la última rutina recibida
static bool hayPendiente = false;
static Comando comandoPendiente;
static void ManejarCalibracion()
{
    String accion = server.arg("accion");
    Comando c = {};
    if (accion == "entrar")
        c.tipo = EntrarCalib;
    else if (accion == "salir")
        c.tipo = SalirCalib;
    else if (accion == "mover" || accion == "fijar")
    {
        if(!server.hasArg("motor")){
            server.send(400, "text/plain", "falta motor");
            return;
        }
        int motor = server.arg("motor").toInt();
        if(motor != YUGO && motor != CARRUSEL){
            server.send(400, "text/plain", "motor invalido");
            return; 
        }
        c.motor = static_cast<Motores>(motor);
        if(accion == "fijar"){
            c.tipo = FijarCalib;
        }
        else{
            int pasos = server.arg("pasos").toInt();
            if(pasos == 0 || abs(pasos) > PASOS_CALIB_MAX){
                server.send(400, "text/plain", "pasos invalidos");
                return;
            }
            c.tipo = MoverCalib;
            c.pasos = pasos;
        }
    }
    else{
        server.send(400, "text/plain", "accion desconocida");
        return;
    }
    comandoPendiente = c;
    server.send(200, "text/plain", "ok");
}
static void ManejarPagina(){
    server.send(200, "text/html", PAGINA);
}
static void ManejarRutina()
{
    int n = server.arg("n").toInt(); // devuelve 0 si falta o no es número
    if (n < 1 || n > pasos_max)
    {
        server.send(400, "text/plain", "numero de pasos invalido");
        return;
    }

    Rutina r = {}; // temporal, todo en 0: numPasos empieza en 0
    for (int i = 0; i < n; i++)
    {
        int vaso = server.arg("v" + String(i)).toInt();
        long inm = server.arg("i" + String(i)).toInt();
        long esc = server.arg("e" + String(i)).toInt();
        if (!AgregarPaso(r, vaso, inm, esc))
        {
            server.send(400, "text/plain", "paso invalido");
            return; // la temporal se descarta: nada a medias
        }
    }

    pendiente = r; // solo aquí entra la rutina completa
    hayPendiente = true;
    server.send(200, "text/plain", "ok");
}

void IniciarConexionWiFi()
{
    WiFi.softAP("Tenidor", "12345678"); // red propia; la clave pide 8 caracteres o más
}

void CrearServerPropio()
{
    server.on("/rutina", HTTP_POST, ManejarRutina);
    server.on("/calibracion", HTTP_POST, ManejarCalibracion);
    server.on("/", HTTP_GET, ManejarPagina);
    server.begin();
}

void AtenderConexion() { server.handleClient(); } // se llama en cada vuelta del loop
bool HayRutinaPendiente() { return hayPendiente; }

void ObtenerRutina(Rutina &destino)
{
    destino = pendiente; // copia unos 130 bytes
    hayPendiente = false;
}


Comando GetCommand(){
    Comando c = comandoPendiente;
    comandoPendiente.tipo = SinComando;
    return c;
}
