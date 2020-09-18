

class Coordenada{
    private:
        int x;
        int y;


    public:
        Coordenada(){}

        Coordenada(int x, int y){
            this-> x = x;
            this-> y = y;
        }

        void setX(int valor){
            x = valor;
        }

        int getX(){
            return x;
        }

        void setY(int valor){
            y = valor;
        }

        int getY(){
            return y;
        }

};
