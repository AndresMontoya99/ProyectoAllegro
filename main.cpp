#include <allegro.h>
#include <vector>
#include <stdio.h>

#include <string>
#include <strstream>
#include <iostream>
#include <sstream>

#include "Adicional/VariablesGlobales.cpp"
#include "Clases/Serpiente.cpp"
#include "Adicional/Funciones.cpp"

int main()
{
    int opcion;

    iniciar();
    imprimirMenu();
    imprimirJuego();


    destroy_bitmap(buffer);
    destroy_bitmap(fondo);
    destroy_bitmap(fondo2);
    destroy_bitmap(header);
    destroy_bitmap(cuerpo);
    destroy_bitmap(logo);


    destroy_sample(sonidoMenu);
    destroy_sample(sonidoJuego);
    destroy_sample(sonidoGanar);
    destroy_sample(sonidoPerder);
    destroy_sample(sonidoObjetivo);

    return 0;
}
END_OF_MAIN();
