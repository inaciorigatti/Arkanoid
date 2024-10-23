#include <iostream>     // Cria e gerencia inputs e outputs
#include <windows.h>    // Configura��es windows, criar janelas, etcaa
#include <conio.h>      // Para controle das teclas
#include <ctime>        // Para time()
#include <mmsystem.h>   // Para musica
#include <stdlib.h>     // Controle de processos, convers�es, etc.
#include <unistd.h>     // API POSIX, interface port�vel do sistema de interface
#include <iomanip>      // � utilizada para facilitar o display visual do cronometro
#include <string>       // Para utiliza��o de vari�veis tipo strings
#include <fstream>      // Manipular arquivos
#include <vector>       // Para manipula��o de vari�veis tipo vetor
#include <algorithm>    // Usada no controle de ranges
#include <sstream>      // Para manipula��o de arquivos

using namespace std;

void menu();                        // Fun��o que exibe o menu do jogo
int verificador(int escolha);       // Fun��o que valida as op��es
int escolhaMenu(int escolha);       // Fun��o que trata as op��es do menu
void comoJogar();                   // Fun��o que explica como jogar o jogo
void sobreJogo();                   // Fun��o que explica o que � o jogo
string dataAtual();                 // Fun��o para pegar da data atual do sistema
void _SetColor(int cor);            // Fun��o que configura como � exibido um caracter no console
void _ResetColor();                 // Fun��o que reseta a cor para o branco
void _ReposicionarCursor();         // Fun��o que reposiciona o cursor para as posi��es 0 no eixo X e 0 no eixo Y do console, para dar a impress�o que o jogo est� com FPS adequado
void apagarTela();                  // Fun��o que cont�m um comando para apagar os elementos no console
void jogar();                       // Fun��o que cont�m um texto auxiliar que ajuda o usuario a identificar que o jogo est� iniciando
void jogoJogar();                   // Fun��o que cont�m o jogo em si
void dificuldadeEscolher();         // Fun��o que auxilia a escolha da dificuldade
int dificuldadeSeletor(int dificuldade);    // Vari�vel que recebe a dificuldade escolhida
void ranking();                             // Fun��o respons�vel pelo ranking
void gerarFase(int m[][16], int fase);      // Fun��o para gerar a fase
void imprimirMapa(int m[][16]);             // Fun��o para imprimir o mapa
bool todosBlocosQuebrados(int m[][16]);     // Fun��o para verificar se todos os blocos foram quebrados
int salvarTempoFinal();

// Variaveis globais
int jogoDificuldade = 2;      // Dificuldade Padrao Medio Ser 2
int vida = 3;                 // Vari�vel vida do jogador
int horas = 0;                // Vari�vel de horas
int minutos = 0;              // Vari�vel de minutos
int segundos = 0;              // Vari�vel de segundos
bool modoJogoAutomatico = false; // Vari�vel para controlar o modo autom�tico
int qtdBlocosQuebrados = 0;     // Qtde de blocos quebrados
bool mostrarInformacoes = false;

// Vari�vel para controlar a velocidade do paddle
int velocidadePaddle = 1;

const int TAM = 16;         // Tamanho da matriz do mapa
int paddleSizeInicial = 4;  // Tamanho inicial do paddle
int paddleSize = 4;         // Tamanho do paddle
int x = 12, y = 6;          // Posi��o do paddle
int bolaX = 11, bolaY = rand() % 7 + 2;  // Posi��o da bola // 8
int dirX = 1, dirY = 1;     // Dire��o da bola

// Vari�veis especiais
int ItensPegos = 0;
int vidaextra = 0;
int vidaextraInicial = 0;
int vidainicial = 3;
float itemX = -1, itemY = -1; // Coordenadas do item (inicialmente fora da tela)
int itemEffect = -1; // Tipo do efeito especial (inicialmente nenhum)
int tempoItem = 0; // Vari�vel de controle para o tempo de queda do item
const int velocidadeItem = 3; // Controla a velocidade do item (quanto maior, mais lento)
bool itemAtivo = false; // Se o item est� em queda ou n�o

int ItemAumentoPaddle = 0;
int ItemDiminuiPaddle = 0;
int PontosExtras = 0;
int Multiplicador = 0;
int Itemvida = 0;
int ItenInversor = 0;
bool multiplicadorAtivo = false;

// Formul�rio criado para o jogador
struct Jogador {
    string nome;
    int dificuldade;
    int pontuacao;
    int vidas;
    int pontosDuplos;
    int inversosPontos;
    int pontoExt;
    int modPaddle;
    int tempo;
    string data;
};

// Fun��o criada para salvar informa��o do jogo
void salvarInfo(int pontuacao) {

    cout << "Nome do Jogador: ";
    string jogadorX;
    cin >> jogadorX;

    // Cria um objeto do tipo `Jogador` com as informa��es do jogador
    Jogador j1 = {jogadorX, jogoDificuldade, pontuacao, Itemvida, Multiplicador, ItenInversor, PontosExtras, ItemAumentoPaddle + ItemDiminuiPaddle, salvarTempoFinal(), dataAtual()};

    // Abre o arquivo "ranking.dat" em modo de escrita (ios::out) e adiciona no final do arquivo (ios::app)
    fstream f;
    f.open("ranking.dat", ios::out | ios::app);

    if (f.is_open()) {  // Verifica se o arquivo foi aberto corretamente
        // Grava os dados do jogador no arquivo, separados por ponto e v�rgula
        f << j1.nome << ";\t" << j1.dificuldade << ";\t" << j1.pontuacao << ";\t"
          << j1.vidas << ";\t" << j1.pontosDuplos << ";\t" << j1.pontoExt << ";\t"
          << j1.modPaddle << ";\t" << j1.tempo << ";\t" << j1.data << endl;
        f.close();  // Fecha o arquivo ap�s a grava��o
    } else {
        cout << "Erro ao abrir o arquivo" << endl;  // Exibe uma mensagem de erro se o arquivo n�o for aberto
    }

    // Reset das vari�veis de controle para o pr�ximo jogo
    ItemAumentoPaddle = 0;
    ItemDiminuiPaddle = 0;
    PontosExtras = 0;
    Multiplicador = 0;
    Itemvida = 0;
    ItenInversor = 0;

    apagarTela();  // Chama uma fun��o para limpar a tela
}

// Fun��o criada para ler as informa��es do arquivo de ranking e exibi-las
void lerInfo() {
    fstream f;
    f.open("ranking.dat", ios::in);  // Abre o arquivo "ranking.dat" no modo de leitura (ios::in)

    if (f.is_open()) {  // Verifica se o arquivo foi aberto
        string linha;
        // L� o arquivo linha por linha at� o final
        while (getline(f, linha)) { //getline � usada para ler uma linha completa de um arquivo ; f � o arquivo ; getline � usada para ler uma linha completa de um arquivo
            cout << linha << endl;
        }
        f.close();  // Fecha o arquivo ap�s a leitura
    } else {
        cout << "Erro ao abrir o arquivo" << endl;  // Exibe uma mensagem de erro se o arquivo n�o for aberto
    }

    cout << endl;  // Adiciona uma linha em branco
    menu();
}


// Definindo a estrutura visual do cronometro
void displayClock() {
    cout << "Tempo: " << setfill('0') << setw(2) << horas << ":";
    cout << setfill('0') << setw(2) << minutos << ":";
    cout << setfill('0') << setw(2) << segundos << endl;
}

int salvarTempoFinal() {
    int salvarSeg = segundos;
    int salvarMin = minutos;
    int salvarHr = horas;

    int tempoMin = 0; //Vai salvar em segundos jogados

    tempoMin = (salvarHr*3600)  + (salvarMin*60) + salvarSeg;

    return tempoMin;

}

// Fun��o criada para resetar totalmente os efeitos
void resetarEfeitos() {
    paddleSize = paddleSizeInicial; // Reseta o tamanho do paddle para o tamanho inicial
    vidaextra = vidaextraInicial;
    vida = vidainicial;
    // Se precisar de mais resets, adicione aqui, como pontuacao ou vidaExtra
}


// Fun��o que configura como � exibido um caracter no console
void _SetColor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

// Fun��o criada com o objetivo de atuar como cronometro do jogo
void timer() {
    static time_t tempoInicial = time(0); // Salva o tempo de in�cio ao iniciar o jogo

    // Obt�m o tempo atual
    time_t tempoAtual = time(0);

    // Calcula a diferen�a entre o tempo atual e o tempo de in�cio
    int tempoPercorrido = static_cast<int>(difftime(tempoAtual, tempoInicial));

    // Atualiza horas, minutos e segundos com base no tempo decorrido
    horas = (tempoPercorrido / 3600); // Convers�o de horas para segundos
    minutos = (tempoPercorrido % 3600) / 60; // Convers�o para minutos
    segundos = (tempoPercorrido % 60); // Segundos

    // Define a posi��o do cron�metro no console
    COORD coord = { 0, 18 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    _ResetColor();
    displayClock();  // Exibe o cron�metro atualizado
}

// Fun��o que reseta a cor para o branco
void _ResetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Fun��o que reposiciona o cursor para as posi��es 0 no eixo X e 0 no eixo Y do console
void _ReposicionarCursor() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fun��o que cont�m um comando para apagar os elementos no console
void apagarTela() {
    system("cls");
}

// Fun��o para gerar uma fase aleat�ria
void gerarFase(int m[][TAM], int fase) {
    // Limpar o mapa e criar as paredes
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (i == 0 || i == TAM - 1 || j == 0 || j == TAM - 1) {
                m[i][j] = 1;  // Paredes
            } else {
                m[i][j] = 0;  // Espa�o vazio
            }
        }
    }

    // Define o n�mero de blocos baseado na fase
    int numBlocos = fase == 1 ? 15 : fase == 2 ? 20 : 25;

    int blocosNormais = (numBlocos * 2) / 3;  // Mais blocos normais (2)
    int blocosEspeciais = numBlocos - blocosNormais;  // Menos blocos especiais (3)

    // Coloca blocos normais (2)
    while (blocosNormais > 0) {
        int x = rand() % (TAM - 8) + 2;  // Posi��o aleat�ria, evitando a borda superior
        int y = rand() % 12 + 2;  // Garante que o valor de y seja de 1 at� 9 (y < 10)
        if (m[x][y] == 0) {  // Coloca bloco se a posi��o estiver vazia
            m[x][y] = 2;
            blocosNormais--;
        }
    }

    // Coloca blocos especiais (3) com durabilidade aleat�ria (2 ou 3)
    while (blocosEspeciais > 0) {
        int x = rand() % (TAM - 8) + 2;  // Posi��o aleat�ria, evitando a borda superior
        int y = rand() % 12 + 2;  // Garante que o valor de y seja de 1 at� 9 (y < 10)
        if (m[x][y] == 0) {  // Coloca bloco se a posi��o estiver vazia
            m[x][y] = 3;  // Coloca bloco especial
            blocosEspeciais--;
        }
    }
}

// Fun��o para imprimir o mapa
void imprimirMapa(int m[][16]) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

// Fun��o para verificar se todos os blocos foram quebrados
bool todosBlocosQuebrados(int m[][TAM]) {
    for (int i = 1; i < TAM - 1; i++) {
        for (int j = 1; j < TAM - 1; j++) {
            if (m[i][j] == 2 || m[i][j] == 3) {  // Verifica blocos normais ou especiais
                return false;  // Ainda h� blocos restantes
            }
        }
    }
    return true;  // Todos os blocos foram quebrados
}

// Fun��o para exibir o mapa (matriz de blocos)
void exibirMapa(int m[][TAM]) {
    _ReposicionarCursor(); // Resetar posi��o

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (i == x && j >= y && j < y + paddleSize) {
                _ResetColor();
                cout << char(220); // Paddle
            } else if (i == bolaX && j == bolaY) {
                _ResetColor();
                cout << 'O'; // Bolinha
            } else if (itemAtivo && i == itemY && j == itemX) { // Verifica se o item especial est� ativo e desenha o item caindo
                _SetColor(14); // Cor verde ou outra de sua escolha
                cout << "?"; // Desenha o item especial caindo
            } else {
                switch (m[i][j]) {
                    case 0:
                        _ResetColor();
                        cout << " "; // Caminho vazio
                        break;
                    case 1:
                        _SetColor(1); // Azul
                        cout << char(219); // Bloco azul
                        break;
                    case 2:
                        _SetColor(10); // Amarelo
                        cout << char(219); // Bloco amarelo
                        break;
                    case 3:
                        _SetColor(12); // Vermelho
                        cout << char(219); // Bloco vermelho
                        break;
                }
            }
        }
        cout << endl;
    }
}

void moverPaddleAutomatico() {
    // L�gica para mover o paddle automaticamente
    if (bolaY > y + paddleSize / 2) {
        y++;
    } else if (bolaY < y + paddleSize / 2) {
        y--;
    }
    // Limitar os movimentos do paddle
    if (y < 1) {
        y = 1;
    } else if (y + paddleSize > TAM - 1) {
        y = TAM - 1 - paddleSize;
    }
}

void moverPaddle(int &velocidadePaddle) {
    if (modoJogoAutomatico) {
        moverPaddleAutomatico();
    } else {
        // L�gica atual para mover o paddle com as teclas
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == 'a' && y > 1) { // Mover para a esquerda
                y--;
            }
            else if (tecla == 'd' && y < TAM - paddleSize - 1) { // Mover para a direita
                y++;
            }
        }
    }
}

// Fun��o criada com o objetivo de exibir o HUD do jogo
void exibirHUD(int &pontuacao, int &vida) {
    // Define a posi��o para a pontua��o
    COORD pontosCoord = { 0, 16 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pontosCoord);
    _ResetColor();
    cout << "Pontuacao: " << pontuacao;

    // Define a posi��o para a vida
    COORD vidaCoord = { 0, 17 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), vidaCoord);
    _ResetColor();
    cout << "Vida: " << vida;
}

// Nova fun��o para mover a bola e verificar colis�es
void SistemaJogo(int m[][TAM], int& pontuacao, int& paddleSize, int& vidaextra) {
    _ReposicionarCursor(); // Resetar posi��o

    // Verifica colis�o da bolinha com o paddle
    if (bolaY >= y && bolaY < y + paddleSize) {
        if (bolaX == x) {
            dirX = -dirX;
            bolaX = x - 1;
        } else if (bolaX == x + paddleSize) {
            dirX = -dirX;
            bolaX = x + paddleSize;
        }
    }

    // Atualiza a posi��o da bolinha
    bolaX += dirX;
    bolaY += dirY;

    // Verifica colis�o com as paredes
    if (bolaX <= 1 || bolaX >= TAM - 2) {
        dirX = -dirX;
    }
    if (bolaY <= 1 || bolaY >= TAM - 2) {
        dirY = -dirY;
    }

    // Verifica colis�o com os blocos
    if (m[bolaX][bolaY] == 2) {
        m[bolaX][bolaY] = 0;
        qtdBlocosQuebrados += 1;
        PlaySound("QUEBRA.wav", NULL, SND_ASYNC);

        // Aplica multiplicador na pontua��o dos blocos normais
        pontuacao += 100 * (multiplicadorAtivo ? 2 : 1); // Dobra a pontua��o se o multiplicador estiver ativo
        dirY = -dirY;
    } else if (m[bolaX][bolaY] == 3) {
        static int durabilidade[TAM][TAM] = {0};
        if (durabilidade[bolaX][bolaY] == 0) {
            durabilidade[bolaX][bolaY] = (rand() % 2) + 2;
        }

        durabilidade[bolaX][bolaY]--;
        if (durabilidade[bolaX][bolaY] <= 0) {
            m[bolaX][bolaY] = 0;
            qtdBlocosQuebrados += 1;
            PlaySound("QUEBRA.wav", NULL, SND_ASYNC);

            // Aplica multiplicador na pontua��o dos blocos especiais
            pontuacao += 200 * (multiplicadorAtivo ? 2 : 1); // Dobra a pontua��o se o multiplicador estiver ativo

            // Gera o item especial
            itemX = bolaX; // Posi��o do bloco destru�do
            itemY = bolaY;
            itemEffect = rand() % 6; // Tipo de efeito aleat�rio
            itemAtivo = true;

            dirY = -dirY;
        } else {
            dirY = -dirY;
        }
    }

    // Verifica se h� um item ativo caindo
    if (itemAtivo) {
        // Controla a velocidade do item
        if (tempoItem % velocidadeItem == 0) {
            itemY += 1; // Movimenta o item para baixo mais lentamente
        }
        tempoItem++; // Incrementa o tempo a cada frame

        // Exibe o item caindo
        exibirMapa(m); // Atualiza o mapa com o item caindo

        // Verifica colis�o do item com o paddle
        if (itemY == x && itemX >= y && itemX < y + paddleSize) { // Verifica se o item caiu exatamente na linha do paddle
            // Aplica o efeito ao paddle ou pontua��o
            switch (itemEffect) {
                case 0:
                    paddleSize = (paddleSize <= 12) ? paddleSize + 2 : 12; // aumenta o tamanho do paddle
                    ItensPegos++;
                    ItemAumentoPaddle++;
                    break;
                case 1:
                    paddleSize = (paddleSize > 2) ? paddleSize - 2 : 2; // Diminui o tamanho do paddle
                    ItensPegos++;
                    ItemDiminuiPaddle++;
                    break;
                case 2:
                    pontuacao += 500; // Adiciona pontos
                    ItensPegos++;
                    PontosExtras++;
                    break;
                case 3:
                    multiplicadorAtivo = true; // Ativa o multiplicador
                    Multiplicador++;
                    ItensPegos++;
                    break;
                case 4:
                    vidaextra++; // Ganha vida extra
                    ItensPegos++;
                    Itemvida++;
                    break;
                case 5:
                    string pontuacaoStr = to_string(pontuacao);
                    reverse(pontuacaoStr.begin(), pontuacaoStr.end());
                    pontuacao = stoi(pontuacaoStr); // Inverte a pontua��o
                    ItensPegos++;
                    ItenInversor++;
                    break;
            }
            itemAtivo = false; // O item foi capturado, desativar
        }

        // Se o item atingir a linha 14, ele desaparece
        if (itemY >= 14) {
            itemAtivo = false; // Desativa o item se atingir a linha 13
        }
    }
}

// Fun��o criada com o objetivo de servir como bot�o de TAB, para observar as info's geral do jogo
void exibirTab(int &pontuacao, int &ItensPegos, int &qtdBlocosQuebrados, int &segundos) {
    apagarTela(); // Limpa a tela
    // Define a posi��o para o PopUp
    COORD popupCoord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), popupCoord);
    _ResetColor();
    cout << "Informacoes do jogo ate o momento" << endl << endl;
    cout << "Pontuacao: " << pontuacao << endl;
    cout << "Itens pegos: " << ItensPegos << endl;
    cout << "Blocos quebrados: " << qtdBlocosQuebrados << endl;
    cout << "Tempo jogado (em segundos): " << segundos << endl;
    cout << "Pressione qualquer tecla para voltar ao jogo..." << endl;
    _getch(); // Aguarda uma tecla ser pressionada
    apagarTela();
}

// Fun��o que exibe o menu do jogo
void menu() {

    PlaySound(NULL, 0, 0); //Se voltar para o menu ele para de tocar a musica

    cout << "-= Arkanoid =-" << endl;
    cout << "1. Jogar"       << endl;
    cout << "2. Dificuldade" << endl;
    cout << "3. Ranking"     << endl;
    cout << "4. Como jogar"  << endl;
    cout << "5. Sobre"       << endl;
    cout << "6. Sair"        << endl;
    cout << "Digite sua escolha: ";

    int escolha;
    cin >> escolha;
    verificador(escolha);
}

// Fun��o que valida a escolha do usu�rio
int verificador(int escolha) {
    while (escolha <= 0 || escolha > 6) {
        cout << "Opcao invalida, tente novamente: " << endl;
        cin >> escolha;
    }
    apagarTela();
    return escolhaMenu(escolha); // Chama a fun��o escolhaMenu com a escolha v�lida
}

// Fun��o que processa a escolha do usu�rio
int escolhaMenu(int escolha) {
    switch (escolha) {
        case 1:
            jogar();
            break;
        case 2:
            dificuldadeEscolher();
            break;
        case 3:
            lerInfo();
            break;
        case 4:
            sobreJogo();
            break;
        case 5:
            comoJogar();
            break;
        case 6:
            cout << "Saindo do jogo..." << endl;
            Sleep(2000);
            exit(0); //Encerra o programa
        default:
            return 0;
        }
    return escolha;
}

void dificuldadeEscolher(){
    cout<<"Escolha a dificuldade: "<<endl<<endl;
    cout<<"Dificuldade selecionada por padrao: Medio."<<endl<<endl;
    cout<<"Selecione uma opcao: "<<endl;
    cout<<"1. Facil"    <<endl;
    cout<<"2. Medio"    <<endl;
    cout<<"3. Dificil"  <<endl;
    cout<<"4. Retornar"  <<endl;

    int escolha;
    cout << ">>";
    cin>>escolha;

    while (escolha <= 0 || escolha > 4){
        cout << "Opcao invalida, tente novamente: ";
        cin >> escolha;
    }

    // Switch criado para auxiliar na op��o da dificuldade do jogo
    switch(escolha){
        case 1:
            jogoDificuldade = 1;
            paddleSize = 6;
            break;
        case 2:
            jogoDificuldade = 2;
            paddleSize = 4;
            break;
        case 3:
            jogoDificuldade = 3;
            paddleSize = 2;
            break;
        case 4:
            break;
    }
    apagarTela();
    menu();
}

// Fun��o criada com objetivo de selecionar a dificuldade do jogo
int dificuldadeSeletor(int jogoDificuldade)
{
    switch(jogoDificuldade){
    case 1:
         return paddleSize = 6; // Paddle maior para f�cil
    case 2:
        return paddleSize = 4; // Paddle m�dio para m�dio
    case 3:
        return paddleSize = 2; // Paddle menor para dif�cil
    }
    return 4;
}

// Fun��o que explica como jogar o jogo
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

// Fun��o que explica o que � o jogo
void sobreJogo() {
    apagarTela();
    cout << "\nSobre Arkanoid:\n";
    cout << "Arkanoid eh um classico jogo de arcade lan�ado em 1986 pela Taito.\n";
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

// Fun��o que retorna a data atual no formato DD/MM/AAAA
string dataAtual() {
  // Obter a data atual
  time_t now = time(0);

  // Converter para a hora local
  tm* localTime = localtime(&now);

  // Formatar a data no formato dia/m�s/ano
  stringstream data;
  data << localTime->tm_mday << "/"
     << localTime->tm_mon + 1 << "/"
     << localTime->tm_year + 1900;

  return data.str();
}


// Fun��o que cont�m um texto auxiliar que ajuda o usuario a identificar que o jogo est� iniciando
void jogar() {
    int escolha;
    cout << "Iniciando o jogo Arkanoid... Bom Jogo!" << endl << endl << endl;
    Sleep(1200);
    cout << "Opa, pera la, voce deseja que a nossa IA, apelidada de NLI, jogue o jogo ao inves de voce?" << endl;
    cout << "1. Sim" << endl;
    cout << "2. Nao" << endl << endl;
    cout << ">> ";
    cin>>escolha;

    while (escolha <= 0 || escolha > 2){
        cout << "Opcao invalida, tente novamente: ";
        cin >> escolha;
    }

    // Switch criado para auxiliar na op��o do modo manual ou automatico
    switch(escolha){
        case 1:
            modoJogoAutomatico = true;
            apagarTela();
            cout << endl << endl << "Ativando IA NLI, prepare-se!!!" << endl;
            Sleep(1500);
            apagarTela();
            jogoJogar();
            break;
        case 2:
            apagarTela();
            cout << endl << endl << "Sentimos muito que nao tenha selecionado nossa IA, esperamos que da proxima voce confie nela! Bom jogo!" << endl;
            Sleep(3000);
            apagarTela();
            jogoJogar();
            break;
    }
}

// Fun��o que cont�m o jogo em si
void jogoJogar() {

    // Comando para executar o arquivo da musica contido na pasta do jogo
    //PlaySound(("JOGO_MUSICA.wav"), NULL, SND_ASYNC);

    // Configura o cursor para n�o piscar
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // Esconde o cursor
    SetConsoleCursorInfo(out, &cursorInfo);

    int mapa[TAM][TAM]; // Inicializa o mapa
    int fase = 1; // Come�a na fase 1
    int pontuacao = 0;

    srand(time(0)); // Semente para n�meros aleat�rios

    while (fase <= 3) {
        gerarFase(mapa, fase); // Gera uma fase com blocos
        while (fase <= 3) {
            exibirMapa(mapa); // Exibe a matriz com os blocos
            moverPaddle(velocidadePaddle);

            // DAR UM JEITO DE INTEGRAR ESSA PARTE NO CODIGO POSTERIORMENTE, POIS ELA � O BOT�O POPUP
//            if (_kbhit()) {
//                char tecla = _getch();
//                if (tecla == '\t') { // Tecla TAB
//                    exibirTab(pontuacao, ItensPegos, qtdBlocosQuebrados, segundos);
//                }
//            }
            // DAR UM JEITO DE INTEGRAR ESSA PARTE NO CODIGO POSTERIORMENTE, POIS ELA � O BOT�O POPUP


            SistemaJogo(mapa,pontuacao,paddleSize,vida); // Move a bola e verifica colis�es
            if (todosBlocosQuebrados(mapa)) { // Verifica se todos os blocos foram quebrados
                cout << "Voc� completou a fase " << fase << "!" << endl;
                fase++; // Avan�a para a pr�xima fase
                resetarEfeitos();
                break; // Sai do loop da fase atual
            }
            _ResetColor();
            // Exibe a pontua��o
            exibirHUD(pontuacao, vida);

            // Se a bola cair no limite inferior do mapa perde uma vida
            if(bolaX > 13){
                vida = vida - 1;
            }

            // Caso a quantidade de vidas seja igual a zero (de um total de 3 vidas), o jogador perde
            if(vida == 0){
                apagarTela();
                salvarTempoFinal();
                resetarEfeitos();
                cout<<"O jogador perdeu, mais sorte na proxima!!!";
                vida = 3;
                vida += vidaextra;
                Sleep(3000);
                apagarTela();
                salvarInfo(pontuacao);
                menu();
            }
            Sleep(85); // Aguarda um tempo para o movimento da bola
            timer();
        }
    }
    apagarTela();
    salvarTempoFinal();
    resetarEfeitos();
    cout<<"O jogador venceu, parabens!!";
    vida = 3;
    Sleep(3000);
    apagarTela();
    salvarInfo(pontuacao);
    menu();
    }

int main() {
    menu();
    return 0; // Retorna 0 ao final do programa
}
