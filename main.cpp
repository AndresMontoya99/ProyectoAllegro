#include <allegro.h>
#include <vector>
#include <stdio.h>

#include <string>
#include <strstream>
#include <iostream>
#include <sstream>

#include <fstream>
#include <nlohmann/json.hpp>

#include <conio.h>

#include "Adicional/VariablesGlobales.cpp"
#include "Clases/Serpiente.cpp"
#include "Adicional/Funciones.cpp"

int main()
{
    iniciarAlegro();

    do{
        iniciarValores();
        imprimirMenu();
        if(modoJuego != 4)
            imprimirJuego();

    }while(modoJuego != 4);

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

    //return 0;
}
END_OF_MAIN();
