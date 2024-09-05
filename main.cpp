#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()

using namespace std;

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ResetColor() {
    SetColor(15); // Cor padrão (branco)
}

int GetRandomColor() {
    return 1 + rand() % 15; // Gera um número entre 1 e 15
}

int main()
{
    // Alerta: Não modificar o trecho de código, a seguir.
    // Início: Comandos para que o cursor não fique piscando na tela
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    // Fim: Comandos para que o cursor não fique piscando na tela
    // Início: Comandos para reposicionar o cursor no início da tela
    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    // Fim: Comandos para reposicionar o cursor no início da tela
    // Alerta: Não modificar o trecho de código, acima.

    int TAM = 15;

    int m[TAM][TAM] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1},
        {1, 0, 2, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 1},
        {1, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 1},
        {1, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
    };

    // Posição inicial do personagem no console
    int x = 12, y = 6;
    // Variável para tecla pressionada
    char tecla;

    srand(static_cast<unsigned>(time(0))); // Inicializa a semente para números aleatórios
    while (true) {

        // Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        // Imprime o jogo: mapa e personagem.
        for (int i = 0; i < TAM; i++) {
            for (int j = 0; j < TAM; j++) {
                
                if (i == x && j == y){
                     ResetColor();
                    cout << char(110);
                } else { 
                    switch (m[i][j]){
                         case 0:
                            ResetColor();
                            cout << " "; // caminho
                            break;
                    }
                    
                }cout << "\n";
                  if (_kbhit()) {
            tecla = getch();
            switch (tecla) {
                case 75: case 'a': // esquerda
                    y--;
                    break;
                case 77: case 'd': // direita
                    y++;
                    break;
            }
        }
                
                
                
                
                if (i == x && j == y) {
                    ResetColor();
                    cout << char(95); // personagem
                } else {
                    switch (m[i][j]) {
                        case 0:
                            ResetColor();
                            cout << " "; // caminho
                            break;
                        case 1:
                            SetColor(1); // Azul
                            cout << char(219); // bloco azul
                            break;
                        case 2:
                            SetColor(GetRandomColor()); // Cor aleatória
                            cout << char(219); // bloco em cor aleatória
                            break;
                        case 3:
                            SetColor(12); // Vermelho
                            cout << char(219); // bloco vermelho
                            break;
                        default:
                            ResetColor();
                            cout << "-"; // erro
                            break;
                    }
                }
            }
            cout << "\n";
        }

        // Executa os movimentos
        if (_kbhit()) {
            tecla = getch();
            switch (tecla) {
                case 75: case 'a': // esquerda
                    y--;
                    break;
                case 77: case 'd': // direita
                    y++;
                    break;
            }
        }
    }

    return 0;
}
