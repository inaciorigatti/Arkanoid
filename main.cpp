#include <iostream> // Cria e gerencia inputs e outputs
#include <windows.h> // Configurações windows, criar janelas, etc
#include <conio.h> // Para controle das teclas
#include <ctime>   // Para time()
#include <mmsystem.h> // Para musica

using namespace std;

void menu();                  // Função que exibe o menu do jogo
int verificador(int escolha); // Função que valida as opções
int escolhaMenu(int escolha); // Função que trata as opções do menu
void comoJogar();             // Função que explica como jogar o jogo
void sobreJogo();             // Função que explica o que é o jogo
void _SetColor(int cor);      // Função que configura como é exibido um caracter no console
void _ResetColor();           // Função que reseta a cor para o branco
void _ReposicionarCursor();   // Função que reposiciona o cursor para as posições 0 no eixo X e 0 no eixo Y do console, para dar a impressão que o jogo está com FPS adequado
bool quebrarBlocosProximos(int blocoX, int blocoY, int m[][16], int& pontuacao);
bool verificarColisaoBloco(int bolaX, int bolaY, int blocoX, int blocoY, int& direcaoX, int& direcaoY, int& pontuacao, int m[15][15]); // Função que verifica a colisão dos blocos quando atingidos pela bolinha
void apagarTela();            // Função que contém um comando para apagar os elementos no console
void jogar();                 // Função que contém um texto auxiliar que ajuda o usuario a identificar que o jogo está iniciando
void jogoJogar();             // Função que contém o jogo em si
int vida = 3;                 // Variável vida do jogador
int tamanhoBola = 1; // Tamanho inicial da bolinha

// Função que configura como é exibido um caracter no console
void _SetColor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

// Função que reseta a cor para o branco
void _ResetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Função que reposiciona o cursor para as posições 0 no eixo X e 0 no eixo Y do console
void _ReposicionarCursor() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Função que contém um comando para apagar os elementos no console
void apagarTela() {
    system("cls");
}

// Função que quebra blocos ao redor do principal caso seja um bloco em formato L (à acima ou abaixo)
bool quebrarBlocosProximos(int blocoX, int blocoY, int m[][16], int& pontuacao) {
    // Verifica e quebra o bloco acima
    if (blocoY - 1 >= 0 && m[blocoX][blocoY - 1] != 0) {
        m[blocoX][blocoY - 1] = 0; // Quebra o bloco
        pontuacao += 100; // Adiciona pontos
        return true; // Retorna verdadeiro indicando que um bloco foi quebrado
    }
    // Verifica e quebra o bloco abaixo
    else if (blocoY + 1 < 15 && m[blocoX][blocoY + 1] != 0) {
        m[blocoX][blocoY + 1] = 0; // Quebra o bloco
        pontuacao += 100; // Adiciona pontos
        return true; // Retorna verdadeiro indicando que um bloco foi quebrado
    }
    return false; // Retorna falso se nenhum bloco adjacente foi quebrado
}

// Função que verifica se a bolinha colidiu com um bloco
bool verificarColisaoBloco(int bolaX, int bolaY, int blocoX, int blocoY, int& direcaoX, int& direcaoY, int& pontuacao, int m[][16]) {
    bool colidiu = false;

    // Verifica se a bolinha está sobre o bloco (os blocos têm tamanho 1x1)
    if (bolaX >= blocoX && bolaX < blocoX + 1 && bolaY >= blocoY && bolaY < blocoY + 1) {
        // Verifica se o bloco é do tipo 2 ou 4
        if (m[blocoX][blocoY] == 2 || m[blocoX][blocoY] == 4) {
            colidiu = true; // Marca a colisão

            // Adiciona aleatoriedade na direção da bolinha
            direcaoX = -direcaoX; // Inverte a direção horizontal
            direcaoY = -direcaoY; // Inverte a direção vertical

            // Tenta quebrar blocos ao redor do bloco atingido
            if (!quebrarBlocosProximos(blocoX, blocoY, m, pontuacao)) {
                // Se nenhum bloco ao redor foi quebrado, quebra o bloco atual
                m[blocoX][blocoY] = 0; // Quebra o bloco
                pontuacao += 100; // Adiciona pontos
            }

            return colidiu; // Retorna verdadeiro indicando que a colisão foi processada
        }
    }
    return colidiu; // Retorna falso se não houve colisão com o bloco
}

// Função que exibe o menu do jogo
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

// Função que valida a escolha do usuário
int verificador(int escolha) {
    if (escolha <= 0 || escolha > 4) {
        cout << "Opcao invalida, tente novamente: " << endl;
        cin >> escolha;
    }
    apagarTela();
    return escolhaMenu(escolha); // Chama a função escolhaMenu com a escolha válida
}

// Função que processa a escolha do usuário
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

// Função que explica como jogar o jogo
void comoJogar() {
    apagarTela();
    cout << "\nComo Jogar Arkanoid: \n";
    cout << "1. Voce controla uma plataforma horizontal (barra) na parte inferior da tela.\n";
    cout << "2. O objetivo eh rebater a bola para destruir todos os blocos na parte superior.\n";
    cout << "3. Use as setas (<) e (>) ou (A) e (D) do teclado para mover a barra para a esquerda e direita respectivamente.\n";
    cout << "4. Se a bola sair do alcance do paddle (cair abaixo dele) e encostar na parte inferior do mapa, voce perde uma vida.\n";
    cout << "5. O jogo termina quando voce destroi todos os blocos ou perde todas as vidas (3 vidas).\n";
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl << endl << endl;
    menu();
}

// Função que explica o que é o jogo
void sobreJogo() {
    apagarTela();
    cout << "\nSobre Arkanoid:\n";
    cout << "Arkanoid eh um classico jogo de arcade lançado em 1986 pela Taito.\n";
    cout << "Ele eh uma evolucao do conceito do jogo Breakout, criado pela Atari nos anos 70.\n";
    cout << "No jogo, voce controla uma barra que se move lateralmente para rebater uma bola.\n";
    cout << "Seu objetivo eh destruir todos os blocos em cada nivel, sem deixar a bola encostar na parte inferior do mapa.\n";
    cout << "Desenvolvido por: Inacio Radin Rigatti, Lucas de Amorim Coelho e Nilson Hoffmann Neto. Setembro/2024. \n" << endl << endl;
    cout << "Responsabilidades durante o desenvolvimento: " << endl << endl;
    cout << "Inacio Radin Rigatti - Criacao do menu, funcionalidades tais como o sistema de vida, musica e organizacao do codigo." << endl;
    cout << "Lucas de Amorim Coelho - Verificacao de contato com a parede, movimentacao da bola, direcao da bola e resolucao de bugs." << endl;
    cout << "Nilson Hoffmann Neto - Cenario, colisao de bloco , personagem(Paddle) e resolucao bugs." << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl << endl << endl;
    menu();
}

// Função que contém um texto auxiliar que ajuda o usuario a identificar que o jogo está iniciando
void jogar() {
    cout << "Iniciando o jogo Arkanoid... Bom Jogo!" << endl;
    Sleep(1200);
    apagarTela();
    jogoJogar();
}

// Função que contém o jogo em si
void jogoJogar() {

    // Comando para executar o arquivo da musica contido na pasta do jogo
    PlaySound(("JOGO_MUSICA.wav"), NULL, SND_ASYNC);

    // Configura o cursor para não piscar
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // Esconde o cursor
    SetConsoleCursorInfo(out, &cursorInfo);

    // Definindo tamanho do mapa
    const int TAM = 16;


    int m[TAM][TAM] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 1},
    {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1},
    {1, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 1},
    {1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // Line 16 now correctly initialized
};

    // Posição inicial do paddle e da bolinha
    int x = 12, y = 6;
    int bolaX = 11, bolaY = 8;
    int dirX = 1, dirY = 1;
    char tecla;
    int pontuacao = 0;

    // Inicializa a semente para números aleatórios
    srand((time(0)));

    while (pontuacao < 4800) {  // Enquanto a pontuação não atingir o máximo (todos os blocos quebrados), o jogo vai continuar

        _ReposicionarCursor(); // Resetar posição

        // Verifica colisão da bolinha com o paddle antes de atualizar a posição
        if (bolaY >= y && bolaY < y + 4) { // Verifica se a bolinha está na posição vertical onde o paddle está
            // Colisão com a lateral esquerda do paddle
            if (bolaX == x) { // Verifica se a bolinha atingiu o paddle
                dirX = -dirX; // Inverte a direção horizontal
                bolaX = x - 1; // Garante que a bolinha encoste no paddle
            }
            // Colisão com a lateral direita do paddle
            else if (bolaX == x + 5) { // Verifica se a bolinha atingiu o outro lado do paddle
                dirX = -dirX; // Inverte a direção horizontal
                bolaX = x + 5; // Garante que a bolinha encoste no paddle
            }
        }

        // Atualiza a posição da bolinha
        bolaX += dirX;
        bolaY += dirY;

        // Verifica colisão com as paredes e inverte a direção se necessário
        if (bolaX <= 1 || bolaX >= TAM - 2) { // Se chegar nas extremidades do mapa
            dirX = -dirX; // Inverte o sinal da variável, para ir para a outra direção possivel
        }
        if (bolaY <= 1 || bolaY >= TAM - 2) { // Se chegar nas extremidades do mapa
            dirY = -dirY; // Inverte o sinal da variável, para ir para a outra direção possivel
        }

        // Verifica colisão com os blocos
        for (int i = 0; i < TAM; i++) {
            for (int j = 0; j < TAM; j++) {
                if (m[i][j] == 2 || m[i][j] == 4) { // Se o bloco tipo 2 ou 4
                    if (verificarColisaoBloco(bolaX, bolaY, i, j, dirX, dirY, pontuacao, m)) {
                        // Bloco quebrado
                    }
                }
            }
        }

        // Imprime o jogo: mapa e personagem
        for (int i = 0; i < TAM; i++) {
            for (int j = 0; j < TAM; j++) {
                if (i == x && j >= y && j < y + 4) { // Quando a posição do iterador estiver na largura do paddle, vai imprimir o paddle
                    _ResetColor();
                    cout << char(220); // Paddle
                } else if (i == bolaX && j == bolaY) { // Caso contrário, se a posição do iterador estiver na posição da bola, vai imprimir a bola
                    _ResetColor();
                    cout << '0'; // Bolinha
                } else { // E se não estiver em nenhuma dessas condições, resta apenas imprimir o mapa
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

        // Exibe a pontuação
        _ResetColor();
        cout << "Pontos: " << pontuacao << endl;
        cout << "Vidas: "<< vida << endl;
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

        // Se a bola cair no limite inferior do mapa perde uma vida
        if(bolaX > 13){
            vida = vida - 1;
        }

        // Caso a quantidade de vidas seja igual a zero (de um total de 3 vidas), o jogador perde
        if(vida == 0){
            apagarTela();
            cout<<"O jogador perdeu, mais sorte na proxima!!!";
            vida = 3;
            Sleep(3000);
            apagarTela();
            menu();
        }
        // Adiciona uma pausa para controle do jogo
        Sleep(75); // 75 milissegundos
    }
    // Caso o jogador atinja a quantidade de pontos máxima, ou seja, quebre todos os blocos, o jogador ganha
    apagarTela();
    cout << "O jogador venceu, parabens!!!" << endl;
    Sleep(2000);
    menu();

}

int main() {
    menu();
    return 0;
}
