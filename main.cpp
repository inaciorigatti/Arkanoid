/**
    Estrutura inicial para um jogo
    vers�o: 0.1 (Prof. Alex,  Adaptado Prof. Felski)
*/
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;


int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    int TAM = 10;
    int m[TAM][TAM] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    };


    //Posicao inicial do personagem no console
    int x=8, y=5;
    //Variavel para tecla precionada
    char tecla;

    while(true){
        ///Posiciona a escrita no iicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<TAM;i++){
            for(int j=0;j<TAM;j++){
                if(i==x && j==y){
                    cout<<char(95); //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        } //fim for mapa

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {

                case 75:case 'a': ///esquerda
                    y--;
                break;
                case 77: case 'd': ///direita
                    y++;
                break;
            }
         }


    } //fim do laco do jogo

    return 0;
} //fim main
