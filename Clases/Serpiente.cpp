
#include "Coordenada.cpp"

vector<Coordenada> objetivoXY;

class Serpiente{
    private:
        vector<Coordenada> posiciones;


    public:
        Serpiente(){}

        void setPosiciones(vector<Coordenada> p){
            posiciones = p;
        }

        vector<Coordenada> getPosiciones(){
            return posiciones;
        }


        void guardarPosicion(){
            posiciones.push_back(Coordenada(x,y));
        }

        void dibujarPosicion(){

            BITMAP *img;

            if(!(x < 0 || x > 29 || y < 0 || y > 15)){

                for(int i = 0 ; i < posiciones.size()-1 && !terminar ; i++){
                    if(posiciones[i].getX() == x && posiciones[i].getY() == y){
                        if(dir != 2 && dir != 3){
                            dir = 2;
                        }
                        terminar = true;
                    }
                }

                if(!terminar){
                    for(int i = 0 ; i < posiciones.size() ; i++){

                        if(i == posiciones.size()-1 ){

                            ostringstream os;
                            os << "img/Cabeza"<< dir <<".bmp";
                            cabeza = load_bitmap(os.str().c_str(),NULL);
                            img = cabeza;

                        }else{
                            img = cuerpo;
                        }

                        masked_blit(img,buffer, 0,0,(posiciones[i].getX()*imgSerpiente)+margen+1,(posiciones[i].getY()*imgSerpiente)+ headerAlto+margen+1 , imgSerpiente,imgSerpiente);
                    }
                }

            }else{
                if(dir != 2 && dir != 3){
                    dir = 2;
                }
                terminar = true;
            }

            if(terminar && vidas > 1){
                x = 0;
                y = 0;
                vidas--;
                posiciones.clear();
                guardarPosicion();
                terminar = false;
                rest(100);
            }
        }

        void borrarPosicion(){

            if(posiciones.size() == tamano)
                posiciones.erase(posiciones.begin());
        }

        void posicionDisponible(){

            bool dis;
            int contador = 0;

            do{
                dis = true;
                int objetivoX = 0+rand()%29;
                int objetivoY = 0+rand()%15;

                for(int i = 0 ; i < objetivoXY.size() && dis ; i++){
                    dis = !(objetivoXY[i].getX() == objetivoX && objetivoXY[i].getY() == objetivoY);
                }

                for(int i = 0 ; i < posiciones.size() && dis ; i++)
                {
                    dis = !(posiciones[i].getX() == objetivoX && posiciones[i].getY() == objetivoY);
                }

                if(dis){
                    objetivoXY.push_back(Coordenada(objetivoX, objetivoY));
                    contador++;
                }

            }while(!dis || contador < objetivoCont);
        }
};
