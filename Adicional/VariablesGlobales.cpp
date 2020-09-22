

using namespace std;
using namespace nlohmann;

int pantallaAncho;
int pantallaAlto;

int headerAlto;

int fondoAlto;

int fondo2Ancho;
int fondo2Alto;

int imgSerpiente;
int imgLogo;

int imgVida;

int margen;

string titulo;

int modoJuego;
int dir;

int x;
int y;

int objetivoCont;

int nivel;
int puntos;
int tamano;
int vidas;

int vidasMax;
int vidasAumen;

int timer;
int tasa;
int corte;
int corteNivel;

bool terminar;

BITMAP  *buffer;//formato BMP
BITMAP  *fondo;  //100, 144, 67  0x649043
BITMAP  *fondo2; //182, 222, 107 0xB6DE6B
BITMAP  *header; //86, 122, 58   0x567A3A
BITMAP  *cuerpo;
BITMAP  *objetivo;
BITMAP  *cabeza;
BITMAP  *vida;
BITMAP  *logo;
BITMAP  *copa;
BITMAP  *copa2;
BITMAP  *perder;
BITMAP  *perder2;
BITMAP  *explo;


// definimos el audio del programa
SAMPLE *sonidoMenu;
SAMPLE *sonidoJuego;

SAMPLE *sonidoGanar;
SAMPLE *sonidoPerder;

SAMPLE *sonidoObjetivo;
