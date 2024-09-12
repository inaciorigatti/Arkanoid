
// Para a musica funcionar. No codeblocks: Settings --> Compiler --> Linker --> Other linker options --> Colocar isto: -lwinmm
// A musica tem que estar na mesma pasta do project e em .wav
// A musica esta no github, juntamente com a musica

#include <iostream>
#include <windows.h>
#include <locale.h> // Portugues PT-BR Caracteres
#include <conio.h> // Para controle das teclas
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()
#include <mmsystem.h> // Para musica

using namespace std;

//Declara as funcoes que existem no programa
bool verificarColisaoBloco(int bolaX, int bolaY, int blocoX, int blocoY, int& direcaoX, int& direcaoY, int& pontuacao, int m[][15]);
void _SetColor(int cor);
void _ResetColor();
void _ReposicionarCursor();
void jogoJogar();
void jogar();                 // Fun��o para iniciar o jogo
void comoJogar();             // Fun��o que explica como jogar o jogo
void sobreJogo();             // Fun��o que explica o que � o jogo
int escolhaMenu(int escolha); // Fun��o que trata as op��es do menu
int verificador(int escolha); // Fun��o que valida as op��es
void menu();                  // Fun��o que exibe o menu

int vida = 3;

// Fun��o que valida a escolha do usu�rio
int verificador(int escolha) {
    if (escolha <= 0 || escolha > 4) {
        cout << "Op��o inv�lida, tente novamente: " << endl;
        cin >> escolha;
    }
    system("cls");
    return escolhaMenu(escolha); // Chama a fun��o escolhaMenu com a escolha v�lida
}

// Fun��o que processa a escolha do usu�rio
int escolhaMenu(int escolha) {
    switch (escolha) {
        case 1:
            jogar();
            break;
        case 2:
            comoJogar();
            break;
        case 3:
            sobreJogo();
            break;
        case 4:
            cout << "Saindo do jogo..." << endl;
            exit(0); //Encerra o programa
        default:
            return 0;
    }
    return escolha;
}

void jogar() {
    cout << "Iniciando o jogo Arkanoid... Bom Jogo!" << endl;
    Sleep(1200);
    system("cls");
    jogoJogar();
}


void comoJogar() {
    system("cls");
    cout << "\nComo Jogar Arkanoid: \n";
    cout << "1. Voc� controla uma plataforma horizontal (barra) na parte inferior da tela.\n";
    cout << "2. O objetivo � rebater a bola para destruir todos os blocos na parte superior.\n";
    cout << "3. Use as setas do teclado para mover a barra para a esquerda e direita.\n";
    cout << "4. Se a bola cair sem ser rebatida, voc� perde uma vida.\n";
    cout << "5. O jogo termina quando voc� destr�i todos os blocos ou perde todas as vidas.\n";
    cout << "6. Aten��o aos power-ups que caem dos blocos destru�dos, eles podem ajudar ou atrapalhar!\n" << endl;
    menu();
}

void sobreJogo() {
    system("cls");
    cout << "\nSobre Arkanoid:\n";
    cout << "Arkanoid � um cl�ssico jogo de arcade lan�ado em 1986 pela Taito.\n";
    cout << "Ele � uma evolu��o do conceito do jogo Breakout, criado pela Atari nos anos 70.\n";
    cout << "No jogo, voc� controla uma barra que se move lateralmente para rebater uma bola.\n";
    cout << "Seu objetivo � destruir todos os blocos em cada n�vel, sem deixar a bola cair.\n";
    cout << "Desenvolvido por: In�cio Radin Rigatti, Lucas de Amorim Coelho e Nilson Hoffmann Neto. Setembro/2024. \n" << endl;
    menu();
}

void menu() {
    PlaySound(NULL, 0, 0); // para de tocar a musica
    cout << "-= Arkanoid =-" << endl;
    cout << "1. Jogar" << endl;
    cout << "2. Como jogar" << endl;
    cout << "3. Sobre" << endl;
    cout << "4. Sair" << endl;
    cout << "Digite sua escolha: ";

    int escolha;
    cin >> escolha;
    verificador(escolha);
}

int vidaJogador(int &vida) {
    vida = vida - 1;
    if (vida <= 0) {
        system("cls");
        cout << "\nVoc� perdeu todas as suas vidas!" << endl;
        Sleep(3);
        system("cls");
        menu();
        return 0;
    }
    return vida;
}


void mostrarVida() {
    cout << "Vidas: ";
    for (int i = 1; i <= vida; i++) {
        cout << "@ "; // @ representa as vidas
    }
    cout << endl;
}

void apagarTela(){
    system("cls");
}

void jogoJogar() {

    PlaySound(TEXT("JOGO_MUSICA.wav"), NULL, SND_ASYNC);

    // Configura o cursor para n�o piscar
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // Esconde o cursor
    SetConsoleCursorInfo(out, &cursorInfo);

    // Definindo tamanho do mapa
    const int TAM = 15;

    // Cria��o do mapa
    int m[TAM][TAM] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1},
        {1, 0, 2, 0, 4, 4, 4, 0, 0, 4, 4, 4, 2, 0, 1},
        {1, 0, 2, 0, 4, 2, 4, 0, 0, 4, 2, 4, 2, 0, 1},
        {1, 0, 2, 0, 4, 2, 4, 0, 0, 4, 2, 4, 2, 0, 1},
        {1, 0, 2, 0, 4, 4, 4, 0, 0, 4, 4, 4, 2, 0, 1},
        {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1},
        {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
    };

    // Posi��o inicial do paddle e da bolinha
    int x = 12, y = 6;
    int bolaX = 11, bolaY = 11;
    int dirX = 1, dirY = 1;
    char tecla;
    int pontuacao = 0;

    // Inicializa a semente para n�meros aleat�rios
    srand(static_cast<unsigned>(time(0)));

    while (pontuacao<6000) {  //alterado



        _ReposicionarCursor();


        // Verifica colis�o da bolinha com o paddle antes de atualizar a posi��o
        if (bolaY >= y && bolaY < y + 4) { // Verifica se a bolinha est� na posi��o vertical onde o paddle est�
            // Colis�o com a lateral esquerda do paddle
            if (bolaX == x) { // Verifica se a bolinha atingiu o paddle
                dirX = -dirX; // Inverte a dire��o horizontal
                bolaX = x - 1; // Garante que a bolinha encoste no paddle
            }
            // Colis�o com a lateral direita do paddle
            else if (bolaX == x + 4) { // Verifica se a bolinha atingiu o outro lado do paddle
                dirX = -dirX; // Inverte a dire��o horizontal
                bolaX = x + 4; // Garante que a bolinha encoste no paddle
            }
        }

        // Atualiza a posi��o da bolinha
        bolaX += dirX;
        bolaY += dirY;

        // Verifica colis�o com as paredes e inverte a dire��o se necess�rio
        if (bolaX <= 1 || bolaX >= TAM - 2) { // Se chegar nas extremidades do mapa
            dirX = -dirX; // Inverte o sinal da vari�vel, para ir para a outra dire��o poss�vel
        }
        if (bolaY <= 1 || bolaY >= TAM - 2) { // Se chegar nas extremidades do mapa
            dirY = -dirY; // Inverte o sinal da vari�vel, para ir para a outra dire��o poss�vel
        }

        // Verifica colis�o com os blocos
        for (int i = 0; i < TAM; i++) {
            for (int j = 0; j < TAM; j++) {
                if (m[i][j] == 2 || m[i][j] == 4) { // Se o bloco � do tipo 2 ou 4
                    if (verificarColisaoBloco(bolaX, bolaY, i, j, dirX, dirY, pontuacao, m)) {
                        // Bloco quebrado
                    }
                }
            }
        }

        // Imprime o jogo: mapa e personagem
        for (int i = 0; i < TAM; i++) {
            for (int j = 0; j < TAM; j++) {
                if (i == x && j >= y && j < y + 4) {
                    _ResetColor();
                    cout << char(220); // Paddle
                } else if (i == bolaX && j == bolaY) {
                    _ResetColor();
                    cout << '.'; // Bolinha
                } else {
                    switch (m[i][j]) {
                        case 0:
                            _ResetColor();
                            cout << " "; // Caminho
                            break;
                        case 1:
                            _SetColor(1); // Azul
                            cout << char(219); // Bloco azul
                            break;
                        case 2:
                            _SetColor(14); // Amarelo
                            cout << char(219); // Bloco amarelo
                            break;
                        case 3:
                            _SetColor(12); // Vermelho
                            cout << char(219); // Bloco vermelho
                            break;
                        case 4:
                            _SetColor(9); // Azul (outra cor de bloco azul)
                            cout << char(219); // Bloco azul
                            break;
                        default:
                            _ResetColor();
                            cout << "-"; // Erro
                            break;
                    }
                }
            }
            cout << endl;
        }

        // Exibe a pontua��o
        _ResetColor();
        cout << "Pontua��o: " << pontuacao << endl;
          // Executa os movimentos do paddle

        if (_kbhit()) {
            tecla = getch();
            switch (tecla) {
                case 75: case 'a': // Esquerda
                    if (y > 1) { // Evita sair da borda esquerda
                        y--;
                    }
                    break;
                case 77: case 'd': // Direita
                    if (y < TAM - 5) { // Evita sair da borda direita
                        y++;
                    }
                    break;
            }
        }
        if(bolaY == 14){

            system("cls");
            cout<< "Voce perdeu";
            Sleep(2000);
            system("cls");
            menu();
        }


//        if(pontuacao == 5800){
//            system ("cls");
//            cout << " voc� ganhou o jogo. " << endl;
//            return 0;
//        }
//        else if(vida < 1){
//            system ("cls");
//            cout << " Voc� perdeu o jogo. " << endl;
        //    return 0;
        //}
        //cout << endl;
        //}

        // Adiciona uma pausa para controle do jogo
        Sleep(55); // 55 milissegundos
    }


    apagarTela();
    cout << " voc� ganhou o jogo. " << endl;
    Sleep(20000000);
    menu();

}

void _SetColor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

void _ResetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void _ReposicionarCursor() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool verificarColisaoBloco(int bolaX, int bolaY, int blocoX, int blocoY, int& direcaoX, int& direcaoY, int& pontuacao, int m[][15]) {
    bool colidiu = false;

    // Checa colis�o com o bloco (os blocos t�m tamanho 1x1)
    if (bolaX >= blocoX && bolaX < blocoX + 1 && bolaY >= blocoY && bolaY < blocoY + 1) {
        // Verifica se o bloco � do tipo 2 ou 4
        if (m[blocoX][blocoY] == 2 || m[blocoX][blocoY] == 4) {
            colidiu = true;
            direcaoX = -direcaoX; // Inverte a dire��o horizontal
            direcaoY = -direcaoY; // Inverte a dire��o vertical

            // Atualiza a pontua��o
            pontuacao += 100;

            // Quebra o bloco, definindo-o como 0
            m[blocoX][blocoY] = 0;

            return colidiu;
        }

    }
    return colidiu;
}

int main() {
    menu();
    return 0;
}
