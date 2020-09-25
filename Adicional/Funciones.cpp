
Serpiente serpiente;

using namespace std;

void iniciarAlegro(){

    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }

	set_volume(250, 200);

    srand(time(0));

    opcionesJuego.push_back("Libre");
    opcionesJuego.push_back("Numeros Pares");
    opcionesJuego.push_back("Multiplos de 5");
    opcionesJuego.push_back("Salir");

    sonidoMenu  = load_wav("music/comeAndGetYourLove.wav");
    sonidoJuego = load_wav("music/animals.wav");

    sonidoGanar = load_wav("music/weAreTheChampions.wav");
    sonidoPerder = load_wav("music/evilMorty.wav");

    sonidoObjetivo = load_wav("music/objetivo.wav");
}


void iniciarValores(){

    imgSerpiente = 30;
    imgLogo = 50;
    imgVida = 24;

    x = 0;
    y = 0;

    nivel = 1;
    puntos = 0;

    modoJuego = 1;
    dir = 2;
    tamano = 3;

    timer = 0;
    corte = 700;
    corteNivel = 5;

    vidas = 3;
    vidasMax = 10;
    vidasAumen = 25;

    objetivoCont = 3;

    serpiente = Serpiente();
    serpiente.guardarPosicion();

    objetivoXY = vector<Coordenada>{};
    serpiente.guardarPosicion();
}


void cargarImagenes(){

    fondo = load_bitmap("img/Background.bmp",NULL);
    fondo2 = load_bitmap("img/Background2.bmp",NULL);
    header = load_bitmap("img/Header.bmp",NULL);
    cuerpo = load_bitmap("img/Cuerpo.bmp",NULL);
    objetivo = load_bitmap("img/Objectivo.bmp",NULL);
    logo = load_bitmap("img/Logo.bmp",NULL);
    vida = load_bitmap("img/Vida.bmp",NULL);

    copa = load_bitmap("img/Copa.bmp",NULL);
    copa2 = load_bitmap("img/Copa2.bmp",NULL);
    explo = load_bitmap("img/Explosion.bmp",NULL);
    perder = load_bitmap("img/Perder.bmp",NULL);
    perder2 = load_bitmap("img/Perder2.bmp",NULL);


    pausa = load_bitmap("img/Pausa.bmp",NULL);

}

void montarImagenes(){

    masked_blit(header,buffer, 0,0,0,0,pantallaAncho, headerAlto);
    masked_blit(logo,buffer, 0,0, pantallaAncho - imgLogo - margen ,(headerAlto/2)-(imgLogo/2),imgLogo, imgLogo);
    masked_blit(fondo,buffer,  0,0,0,headerAlto ,pantallaAncho, fondoAlto+1);
    masked_blit(fondo2,buffer, 0,0, margen,margen + headerAlto,fondo2Ancho+1, fondo2Alto+1);

}

void cambiarPantalla(){

    fondoAlto = pantallaAlto-headerAlto;

    fondo2Ancho = pantallaAncho-40;
    fondo2Alto = fondoAlto-39/*480*/;

    margen = (pantallaAncho-fondo2Ancho)/2;

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, pantallaAncho, pantallaAlto, 0, 0);
    set_window_title(titulo.c_str());

    buffer = create_bitmap(pantallaAncho, pantallaAlto);
}

void dibujarObjetivo(bool actualizar = false, bool primera = false, int index = -1){

    if(actualizar){
        serpiente.posicionDisponible();

        if(!primera){

            switch(modoJuego){

                case 2:
                    terminar = objetivoXY[index].getValor() % 2 != 0;
                    break;
                case 3:
                    terminar = objetivoXY[index].getValor() % 5 != 0;
                    break;
            }

            play_sample(sonidoObjetivo,50,150,1000,0);

            if(!terminar){
                puntos++;
                tamano++;
                if(puntos % corteNivel == 0){
                    nivel++;
                    if(corte > 50)
                        corte -= 50;
                }

                if(vidas < vidasMax && puntos % vidasAumen == 0){
                    vidas++;
                }
            }

            objetivoXY.clear();
        }

    }

    bool dis = false;

    for(int i = 0 ; i < objetivoXY.size() ; i++){

        ostringstream os;

        if(actualizar && modoJuego != 1){

            switch(modoJuego){
                case 2:
                    do{
                        objetivoXY[i].setValor(rand() % 100 + 1);
                        if(!dis)
                            dis = objetivoXY[i].getValor() % 2 == 0;
                    }while(!dis);

                    break;

                case 3:
                    do{
                        objetivoXY[i].setValor(rand() % 100 + 1);
                        if(!dis)
                            dis = objetivoXY[i].getValor() % 5 == 0;
                    }while(!dis);

                    break;
            }
        }

        if(modoJuego != 1){
            os << objetivoXY[i].getValor();
        }


        masked_blit(objetivo,buffer, 0,0,(objetivoXY[i].getX()*imgSerpiente)+margen+1,(objetivoXY[i].getY()*imgSerpiente)+ headerAlto+margen+1 ,imgSerpiente,imgSerpiente);


            textout_centre_ex(buffer, font, os.str().c_str(),(objetivoXY[i].getX()*imgSerpiente)+margen+1 +14,(objetivoXY[i].getY()*imgSerpiente)+ headerAlto+margen+1 +10,
                                        0xFFFFFF, 0xFF0000 );
    }

}


int cargarPuntaje(){
    json imagenes;
    ifstream people_file("json/Puntaje.json", ifstream::binary);
    people_file >> imagenes;

    bool high = puntos > imagenes["PuntajeMaximo"];

    if(high){
        json jsonfile;

        jsonfile["PuntajeMaximo"] = puntos;

        ofstream file("json/Puntaje.json");
        file << jsonfile;
    }

    return imagenes["PuntajeMaximo"];
}

void imprimirResultado(){

    pantallaAlto = 440;
    pantallaAncho = 440;

    headerAlto = 75;

    int record =  cargarPuntaje();
    bool puntaje = puntos > record;

    titulo = "Serpiente Matematica - Puntaje";
    if(puntaje)
        titulo += " Alto";

    cambiarPantalla();
    stop_sample(sonidoJuego);
    int rl = 1;

    play_sample(((puntaje) ?  sonidoGanar  : sonidoPerder ),25,150,1000,1);


    do{
        if(timer % 500 == 0){

            clear(buffer);
            montarImagenes();
            ostringstream os;

            if(puntaje){

                os << "Nuevo Record: " << puntos;
                textout_centre_ex(buffer, font, os.str().c_str(), pantallaAncho/2, headerAlto/2, 0xFFFFFF, 0x567A3A);

                masked_blit((rl == 0) ? copa : copa2,buffer, 0,0, pantallaAncho/2 - (170/2) ,margen + headerAlto+1 + (fondo2Alto/2) - (170/2) ,170, 177);
            }else{
                os << "Puntaje: " << puntos;
                textout_centre_ex(buffer, font, os.str().c_str(), pantallaAncho/2, headerAlto/2 -10, 0xFFFFFF, 0x567A3A);

                os.str("");
                os << "Record: " << record;
                textout_centre_ex(buffer, font, os.str().c_str(), pantallaAncho/2, headerAlto/2 + 10, 0xFFFFFF, 0x567A3A);

                masked_blit((rl == 0) ? perder : perder2 ,buffer, 0,0, pantallaAncho/2 - (206/2) ,margen + headerAlto+1 + (fondo2Alto/2) - (206/2) ,206, 150);
            }

            textout_centre_ex(buffer, font, "ESC para continuar", pantallaAncho/2, headerAlto/2 + 45, 0xFFFFFF, 0x649043);
            rl = ((rl == 0) ? 1 : 0 );
            timer = 0;
        }

        blit(buffer,screen, 0,0,0,0,pantallaAncho,pantallaAlto);
        timer++;

    }while(!key[KEY_ESC]);

    timer = 0;

    stop_sample(sonidoGanar);
    stop_sample(sonidoPerder);
}


void movimientos(){

    bool pintar = true;
    bool pause = false;
    int actual;
    terminar = false;

    dibujarObjetivo(true, true);

    while(!key[KEY_ESC] && !terminar)
    {

        //if(mouse_x >= 470 &&  mouse_x <= 500 && mouse_y >= 23 &&  mouse_y <= 50){

            if(mouse_b == 1)
                pause = true;
            if(mouse_b == 2)
                pause = false;
        //}

        if(!pause){

            if(pintar){
                actual = dir;
                pintar = false;
            }

            if (key[KEY_W])
            {
                if(actual != 3)
                    dir = 1;
                else
                    dir = actual;

            }else if(key[KEY_S]){

                if(actual != 1)
                    dir = 3;
                else
                    dir = actual;

            }else if(key[KEY_D]){

                if(actual != 4)
                    dir = 2;
                else
                    dir = actual;

            }else if(key[KEY_A]){
                if(actual != 2)
                    dir = 4;
                else
                    dir = actual;
            }

            if(timer % corte == 0){

                clear(buffer);
                montarImagenes();

                switch(dir){
                    case 1:
                        y--;
                        break;
                    case 2:
                        x++;
                        break;
                    case 3:
                        y++;
                        break;
                    case 4:
                        x--;
                        break;
                }

                textout_centre_ex(buffer, font, "ESC para terminar", pantallaAncho/2, headerAlto/2 + 45, 0xFFFFFF, 0x649043);
                textout_centre_ex(buffer, font, "Click Izq - Pausa / Click Der - Reanudar", pantallaAncho/2, headerAlto/2 + 20, 0xFFFFFF, 0x567A3A);

                ostringstream os;

                os << "NIVEL  :";
                textout_centre_ex(buffer, font, os.str().c_str(), 50, 10, 0xFFFFFF, 0x567A3A);
                os.str("");
                os << nivel;
                textout_centre_ex(buffer, font, os.str().c_str(), 115, 10, 0xFFFFFF, 0x567A3A);
                os.str("");


                os << "PUNTOS :";
                textout_centre_ex(buffer, font, os.str().c_str(), 50, 30, 0xFFFFFF, 0x567A3A);
                os.str("");
                os << puntos;
                textout_centre_ex(buffer, font, os.str().c_str(), 115, 30, 0xFFFFFF, 0x567A3A);

                for(int i = 0 ; i < vidas ; i++ ){
                    masked_blit(vida,buffer, 0,0, (i+1)*(margen+1), 46,imgVida,imgVida);
                }

                serpiente.guardarPosicion();

                if(objetivoXY.size() > 0){
                    bool comer = false;
                    for(int i = 0 ; i < objetivoXY.size() && !comer ; i++){
                        comer = x == objetivoXY[i].getX() && y == objetivoXY[i].getY();
                        dibujarObjetivo(comer,false,i);
                    }
                }else{
                    dibujarObjetivo(true, true);
                }

                serpiente.dibujarPosicion();
                serpiente.borrarPosicion();

                //masked_blit(pausa,buffer, 0,0, pantallaAncho/2 , headerAlto/2 - 14 ,27, 27);

                pintar = true;
                timer=0;
            }

            timer++;
        }

        show_mouse(buffer);
        blit(buffer,screen, 0,0,0,0,pantallaAncho,pantallaAlto);

    }

    imprimirResultado();
}


void imprimirJuego(){

    stop_sample(sonidoMenu);
    pantallaAlto = 594;
    pantallaAncho = 940;

    headerAlto = 75;

    titulo = "Serpiente Matematica - Juego / " + opcionesJuego[modoJuego-1];

    cambiarPantalla();
    cargarImagenes();
    montarImagenes();

    play_sample(sonidoJuego,25,150,1000,1);

    movimientos();
}


void imprimirMenu(){

    pantallaAlto = 440;
    pantallaAncho = 440;

    headerAlto = 75;

    titulo = "Serpiente Matematica - Menu";

    cambiarPantalla();
    cargarImagenes();
    montarImagenes();


    bool continuar = true;

    int tam = opcionesJuego.size();

    play_sample(sonidoMenu,25,150,1000,1);


    do
    {

        clear(buffer);
        montarImagenes();

        textout_centre_ex(buffer, font, "Seleccione Modo de Juego", pantallaAncho/2, headerAlto/2, 0xFFFFFF, 0x567A3A);

        for(int i = 0 ; i < opcionesJuego.size() ; i++){

            string str = opcionesJuego[i];

            if((i+1 == modoJuego)) {
                if(str.find("-> ") == -1)
                    str = str.replace(0, 0 ,"-> ");
            }else{
                if(str.find("-> ") != -1)
                    str = str.replace(0, 3 ,"");

            }
            textout_centre_ex(buffer, font, str.c_str(), pantallaAncho/2, headerAlto + (fondoAlto/2) - (  (tam*10 * (tam/2)) - (i)*(tam * 10)  ),
                               ((i+1 == modoJuego) ? 0x000000 : 0xFFFFFF ),((i+1 == modoJuego) ? 0xFFFFFF : 0xB6DE6B ) );
        }

        if(mouse_x >= 188 &&  mouse_x <= 253 && mouse_y >= 178 &&  mouse_y <= 186){
            modoJuego = 1;

            if(mouse_b == 1)
                continuar = false;

        }

        if(mouse_x >= 156 &&  mouse_x <= 285 && mouse_y >= 217 &&  mouse_y <= 226){
            modoJuego = 2;

            if(mouse_b == 1)
                continuar = false;
        }

        if(mouse_x >= 152 &&  mouse_x <= 287 && mouse_y >= 258 &&  mouse_y <= 265){
            modoJuego = 3;

            if(mouse_b == 1)
                continuar = false;
        }

        if(mouse_x >= 189 &&  mouse_x <= 252 && mouse_y >= 298 &&  mouse_y <= 305){
            modoJuego = 4;

            if(mouse_b == 1)
                continuar = false;
        }

        show_mouse(buffer);

        blit(buffer,screen, 0,0,0,0,pantallaAncho,pantallaAlto);

    }while(continuar);
}
