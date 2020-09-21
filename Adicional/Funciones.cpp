
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

    sonidoMenu  = load_wav("music/comeAndGetYourLove.wav");
    sonidoJuego = load_wav("music/animals.wav");

    sonidoGanar = load_wav("music/moveYourFeet.wav");
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
    tasa = 20;
    corte = 10;
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


void cargarImagenes(bool menu = false){

    fondo = load_bitmap("img/Background.bmp",NULL);
    fondo2 = load_bitmap("img/Background2.bmp",NULL);
    if(menu)
        header = load_bitmap("img/HeaderMenu.bmp",NULL);
    else
        header = load_bitmap("img/Header.bmp",NULL);
    cuerpo = load_bitmap("img/Cuerpo.bmp",NULL);
    objetivo = load_bitmap("img/Objectivo.bmp",NULL);
    logo = load_bitmap("img/Logo.bmp",NULL);
    vida = load_bitmap("img/Vida.bmp",NULL);

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
                    if(tasa > 1)
                        tasa -= 1;
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


void movimientos(){

    bool pintar = true;
    int actual;
    terminar = false;

    dibujarObjetivo(true, true);

    while(!key[KEY_ESC] && !terminar)
    {
        clear(buffer);
        montarImagenes();

        if(pintar){
            actual = dir;
            pintar = false;
        }

        if (key[KEY_UP])
        {
            if(actual != 3)
                dir = 1;
            else
                dir = actual;

        }else if(key[KEY_DOWN]){

            if(actual != 1)
                dir = 3;
            else
                dir = actual;

        }else if(key[KEY_RIGHT]){

            if(actual != 4)
                dir = 2;
            else
                dir = actual;

        }else if(key[KEY_LEFT]){
            if(actual != 2)
                dir = 4;
            else
                dir = actual;
        }

        if(timer % corte == 0){


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
            textout_centre_ex(buffer, font, "Libre", pantallaAncho/2, headerAlto/2, 0xFFFFFF, 0x567A3A);

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

            blit(buffer,screen, 0,0,0,0,pantallaAncho,pantallaAlto);

            pintar = true;
            timer=0;
        }

        timer++;
        rest(tasa);
    }

}


void imprimirJuego(){

    stop_sample(sonidoMenu);
    pantallaAlto = 594;
    pantallaAncho = 940;

    headerAlto = 75;

    titulo = "Serpiente Matematica - Juego";

    cambiarPantalla();
    cargarImagenes();
    montarImagenes();

    play_sample(sonidoJuego,10,150,1000,1);

    movimientos();
}


void imprimirMenu(){

    pantallaAlto = 440;
    pantallaAncho = 440;

    headerAlto = 75;

    titulo = "Serpiente Matematica - Menu";

    cambiarPantalla();
    cargarImagenes(true);
    montarImagenes();

    vector<string> opciones;

    bool continuar = true;

    opciones.push_back("Libre");
    opciones.push_back("Numeros Pares");
    opciones.push_back("Multiplos de 5");
    opciones.push_back("Salir");


    play_sample(sonidoMenu,20,150,1000,1);

    do
    {
        clear(buffer);

        montarImagenes();

        textout_centre_ex(buffer, font, "Seleccione Modo de Juego", pantallaAncho/2, headerAlto/2, 0xFFFFFF, 0x567A3A);

        int tam = opciones.size();



        if(keypressed()){
        if(continuar){
            char letra = readkey() >> 8;

            switch(letra){

                case KEY_UP:
                    modoJuego--;
                    if(modoJuego == 0)
                        modoJuego = tam;

                    break;

                case KEY_DOWN:
                    modoJuego++;
                    if(modoJuego > tam)
                        modoJuego = 1;
                    break;

                case KEY_ENTER:
                    continuar = false;
                    break;

                case KEY_ESC:
                    continuar = false;
                    break;
            }

        }
        }

        for(int i = 0 ; i < opciones.size() ; i++){

            string str = opciones[i];

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

        blit(buffer,screen, 0,0,0,0,pantallaAncho,pantallaAlto);
        rest(50);

    }while(continuar);


}
