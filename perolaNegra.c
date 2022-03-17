#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STR 50

// === FUNÇÕES COMPLEMENTARES === 

/* ADAPTAR EM FUNÇÃO DE COMO OS DADOS SERÃO ARMAZENADOS NO SEU BOT */
void readData(int h, int w, int mapa[h][w], char myId[MAX_STR], int seaport[], int bot[], int mapa_copia[h][w], int mapa_cr[h][w]) {

  char id[MAX_STR];
  int n, x, y;

  // lê os dados da área de pesca
  for (int i = 0; i < h; i++) {   
    for (int j = 0; j < w; j++) {
      scanf("%i", &mapa[i][j]);
      mapa_copia[i][j] = 0;
      mapa_cr[i][j] = 0;
      // procura a posição de cada porto e joga na matriz seaport.
      if (mapa[i][j] == 1) { 
        seaport[0] = i; seaport[1] = j; //Demarca a posição de cada porto.
      }
      else if (mapa[i][j] > 31 && mapa[i][j] < 40){
        mapa_cr[i][j] = mapa[i][j];
      }
    }
  }

  // lê os dados dos bots
  scanf(" BOTS %i", &n);
  // o " " antes de BOTS é necessário para ler o '\n' da linha anterior
  for (int i = 0; i < n; i++) {
    scanf("%s %i %i", id, &x, &y);
    // verifica cada bot, se não for o bot em questão, marca na matriz mapa_copia a posicao dos outros bots.
    if (strcmp(id, myId) == 0){
      bot[0] = x; bot[1] = y;
    }else{
      mapa_copia[x][y] = 2; // Demarca com 2 a posição de cada barco que não for o seu.
    }
  }
}

char* procuraPorto(int cordx, int cordy, int seaport[2]) {
  if (cordy > seaport[1]) {
    return "LEFT";
  }
  else if (cordy < seaport[1]) {
    return "RIGHT";
  }
  else if (cordx > seaport[0]) {
    return "UP";
  }
  else if (cordx < seaport[0]) {
    return "DOWN";
  }
}

char* verificaInimigo(int h, int w, int cordx, int cordy, int mapa_copia[h][w]){
  if (cordy - 1 > 0 
  && mapa_copia[cordx][cordy-1] != 2){
    return "LEFT";
  }
  else if (cordx - 1 > 0 
  && mapa_copia[cordx+1][cordy] != 2){
    return "UP";
  }
  else if (cordy + 1 < w 
  && mapa_copia[cordx][cordy+1] != 2){
    return "RIGHT";
  }
  else if (cordx + 1 < h
  && mapa_copia[cordx][cordy-1] != 2){
    return "DOWN";
  }
}

// === FUNÇÃO MAIN ===

int main() {
  char line[MAX_STR];   // dados temporários
  char myId[MAX_STR];   // identificador do bot em questão

  setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);


  // === INÍCIO DA PARTIDA ===
  int h, w, storage, aux;
  aux = 1;
  int bot[2];
  int seaport[2];
  storage = 0;
  scanf("AREA %i %i", &h, &w);  // lê a dimensão da área de pesca: altura (h) x largura (w)
  int mapa[h][w];
  int mapa_copia[h][w];
  int mapa_cr[h][w];
  scanf(" ID %s", myId);        // ...e o id do bot
  // obs: o " " antes de ID é necessário para ler o '\n' da linha anterior

  // === PARTIDA === 
  // O bot entra num laço infinito, mas não se preocupe porque o simulador irá matar
  // o processo quando o jogo terminar.
  srand((unsigned)time(NULL));
  while (aux) {

    // LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT
    readData(h, w, mapa, myId, seaport, bot, mapa_copia, mapa_cr);

    int cordx, cordy, local_barco, random;
    random = 1+(rand()%4);
    cordx = bot[0];
    cordy = bot[1];
    local_barco = mapa[cordx][cordy];

    // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA
    // Se o barco estiver no porto, vende os peixes e zera o estoque.
    if (local_barco == 1 && storage > 0) {
      printf("SELL\n");
      storage = 0;

    // Se não estiver em um porto, verifica se tem peixe e se o local possui mais do que um peixe (para evitar multa). Além disso, verifica se o estoque está cheio.

    } else if ((storage < 10 && local_barco > 11 && local_barco < 20) 
    || (storage < 10 && local_barco > 21 && local_barco < 30) 
    || (storage < 10 && local_barco > 31 && local_barco < 40)) {
        storage++; //Acumula o montante com os peixes.
        printf("FISH\n");

    // Se o estoque estiver cheio, retorna uma função que verifica as linhas e colunas em relação a posição do porto mais próximo.

    } else if (storage == 10) { 
      printf("%s\n", procuraPorto(cordx, cordy, seaport));
    
    /*
    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx][cordy+1] == 21)
    || (mapa[cordx][cordy+1] == 0 && mapa[cordx][cordy] == 21)){
        printf("RIGHT\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx+1][cordy] == 21)
    || (mapa[cordx+1][cordy] == 0 && mapa[cordx][cordy] == 21)){
        printf("DOWN\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx-1][cordy] == 21)
    || (mapa[cordx-1][cordy] == 0 && mapa[cordx][cordy] == 21)){
        printf("UP\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx][cordy-1] == 21)
    || (mapa[cordx][cordy-1] == 0 && mapa[cordx][cordy] == 21)){
        printf("LEFT\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx][cordy+1] == 31)
    || (mapa[cordx][cordy+1] == 0 && mapa[cordx][cordy] == 31)){
        printf("RIGHT\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx+1][cordy] == 31)
    || (mapa[cordx+1][cordy] == 0 && mapa[cordx][cordy] == 31)){
        printf("DOWN\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx-1][cordy] == 31)
    || (mapa[cordx-1][cordy] == 0 && mapa[cordx][cordy] == 31)){
        printf("UP\n");

    } else if ((mapa[cordx][cordy] == 0 && mapa[cordx][cordy-1] == 31)
    || (mapa[cordx][cordy-1] == 0 && mapa[cordx][cordy] == 31)){
        printf("LEFT\n");
    */

    //Verifica todos os lados ao redor do barco, se houver algum peixe (robalo) e não tiver nenhum outro barco, ele se movimenta.
    
    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-1] > 31 
    && mapa[cordx][cordy-1] < 40 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n"); 
         
    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+1] > 31 
    && mapa[cordx][cordy+1] < 40 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-1][cordy] > 31 
    && mapa[cordx-1][cordy] < 40 
    && mapa_copia[cordx-1][cordy] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+1][cordy] > 31 
    && mapa[cordx+1][cordy] < 40 
    && mapa_copia[cordx+1][cordy] != 2){
        printf("DOWN\n");

    // VERIFICAR DOIS ESPAÇOS AOS LADOS

    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-2] > 31 
    && mapa[cordx][cordy-2] < 40 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n");

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+2] > 31 
    && mapa[cordx][cordy+2] < 40 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-2][cordy] > 31 
    && mapa[cordx-2][cordy] < 40 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+2][cordy] > 31 
    && mapa[cordx+2][cordy] < 40 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");
    
    // VERIFICAR TRÊS ESPAÇOS AOS LADOS
    /*
    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-3] > 31 
    && mapa[cordx][cordy-3] < 40 
    && mapa_copia[cordx][cordy-1] != 2 
    && cordy + 1 < w 
    && mapa[cordx][cordy+3] > 31 
    && mapa[cordx][cordy+3] < 40 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("LEFT\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+3][cordy] > 31 
    && mapa[cordx+3][cordy] < 40 
    && mapa_copia[cordx+1][cordy-1] != 2 
    && cordx - 1 > 0 
    && mapa[cordx-3][cordy] > 31 
    && mapa[cordx-3][cordy] < 40 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("DOWN\n");

    /*
    // VERIFICAR DIAGONAIS

    } else if (cordy - 1 > 0 
    && mapa[cordx-1][cordy-1] > 31 
    && mapa[cordx-1][cordy-1] < 40 
    && mapa_copia[cordx-1][cordy-1] != 2){
        printf("LEFT\n");
    } else if (cordy + 1 < w 
    && mapa[cordx+1][cordy+1] > 31 
    && mapa[cordx+1][cordy+1] < 40 
    && mapa_copia[cordx+1][cordy+1] != 2){
        printf("RIGHT\n");
    } else if (cordx - 1 > 0 
    && mapa[cordx-1][cordy+1] > 31 
    && mapa[cordx][cordy+1] < 40 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");
    } else if (cordx + 1 < h 
    && mapa[cordx+1][cordy-1] > 31 
    && mapa[cordx][cordy-1] < 40 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");*/

    //Verifica todos os lados ao redor do barco, se houver algum peixe (cioba) e não tiver nenhum outro barco, ele se movimenta.

    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-1] > 21 
    && mapa[cordx][cordy-1] < 30 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n");  

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+1] > 21 
    && mapa[cordx][cordy+1] < 30 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-1][cordy] > 21 
    && mapa[cordx-1][cordy] < 30 
    && mapa_copia[cordx-1][cordy] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+1][cordy] > 21 
    && mapa[cordx+1][cordy] < 30 
    && mapa_copia[cordx+1][cordy] != 2){
        printf("DOWN\n");

    // VERIFICAR DOIS ESPAÇOS AOS LADOS
    
    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-2] > 21 
    && mapa[cordx][cordy-2] < 30 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n");

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+2] > 21 
    && mapa[cordx][cordy+2] < 30 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-2][cordy] > 21 
    && mapa[cordx-2][cordy] < 30 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+2][cordy] > 21 
    && mapa[cordx+2][cordy] < 30 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");
    
    // VERIFICAR TRÊS ESPAÇOS AOS LADOS
    
    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-3] > 21 
    && mapa[cordx][cordy-3] < 30 
    && mapa_copia[cordx][cordy-1] != 2 
    && cordx - 1 > 0 
    && mapa[cordx-3][cordy] > 21 
    && mapa[cordx-3][cordy] < 30 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("LEFT\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+3][cordy] > 21 
    && mapa[cordx+3][cordy] < 30 
    && mapa_copia[cordx+1][cordy-1] != 2 
    && cordy + 1 < w 
    && mapa[cordx][cordy+3] > 21 
    && mapa[cordx][cordy+3] < 30 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");
    
    // VERIFICAR DIAGONAIS

    /*} else if (cordy - 1 > 0 
    && mapa[cordx-1][cordy-1] > 21 
    && mapa[cordx-1][cordy-1] < 30 
    && mapa_copia[cordx-1][cordy-1] != 2){
        printf("LEFT\n");
    } else if (cordy + 1 < w 
    && mapa[cordx+1][cordy+1] > 21 
    && mapa[cordx+1][cordy+1] < 30 
    && mapa_copia[cordx+1][cordy+1] != 2){
        printf("RIGHT\n");
    } else if (cordx - 1 > 0 
    && mapa[cordx-1][cordy+1] > 21 
    && mapa[cordx][cordy+1] < 30 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");
    } else if (cordx + 1 < h 
    && mapa[cordx+1][cordy-1] > 21 
    && mapa[cordx][cordy-1] < 30 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");*/

    //Verifica todos os lados ao redor do barco, se houver algum peixe (tainha) e não tiver nenhum outro barco, ele se movimenta.

    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-1] > 11 
    && mapa[cordx][cordy-1] < 20 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n");  

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+1] > 11 
    && mapa[cordx][cordy+1] < 20 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-1][cordy] > 11 
    && mapa[cordx-1][cordy] < 20 
    && mapa_copia[cordx-1][cordy] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+1][cordy] > 11 
    && mapa[cordx+1][cordy] < 20 
    && mapa_copia[cordx+1][cordy] != 2){
        printf("DOWN\n");

    // VERIFICAR DOIS ESPAÇOS AOS LADOS
    
    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-2] > 11 
    && mapa[cordx][cordy-2] < 20 
    && mapa_copia[cordx][cordy-1] != 2){
        printf("LEFT\n");

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+3] > 11 
    && mapa[cordx][cordy+3] < 20 
    && mapa_copia[cordx][cordy+1] != 2 
    && cordx - 1 > 0 
    && mapa[cordx-3][cordy] > 11 
    && mapa[cordx-3][cordy] < 20 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");  

    } else if (cordy + 1 < w 
    && mapa[cordx][cordy+2] > 11 
    && mapa[cordx][cordy+2] < 20 
    && mapa_copia[cordx][cordy+1] != 2){
        printf("RIGHT\n");

    } else if (cordy - 1 > 0 
    && mapa[cordx][cordy-3] > 11 
    && mapa[cordx][cordy-3] < 20 
    && mapa_copia[cordx][cordy-1] != 2 
    && cordx + 1 < h 
    && mapa[cordx+3][cordy] > 11 
    && mapa[cordx+3][cordy] < 20 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");

    } else if (cordx - 1 > 0 
    && mapa[cordx-2][cordy] > 11 
    && mapa[cordx-2][cordy] < 20 
    && mapa_copia[cordx-1][cordy+1] != 2){
        printf("UP\n");

    } else if (cordx + 1 < h 
    && mapa[cordx+2][cordy] > 11 
    && mapa[cordx+2][cordy] < 20 
    && mapa_copia[cordx+1][cordy-1] != 2){
        printf("DOWN\n");
    
    // VERIFICAR TRÊS ESPAÇOS AOS LADOS

    /*    
    // VERIFICAR DIAGONAIS
    
    } else if (cordy - 1 > 0 
    && mapa[cordx-2][cordy-2] > 11 
    && mapa[cordx-2][cordy-2] < 20 
    && mapa_copia[cordx-2][cordy-2] != 2){
        printf("LEFT\n");
    } else if (cordy + 1 < w 
    && mapa[cordx+2][cordy+2] > 11 
    && mapa[cordx+2][cordy+2] < 20 
    && mapa_copia[cordx+2][cordy+2] != 2){
        printf("RIGHT\n");
    } else if (cordx - 1 > 0 
    && mapa[cordx-2][cordy+2] > 11 
    && mapa[cordx-2][cordy+2] < 20 
    && mapa_copia[cordx-2][cordy+2] != 2){
        printf("UP\n");
    } else if (cordx + 1 < h 
    && mapa[cordx+2][cordy-2] > 11 
    && mapa[cordx+2][cordy-2] < 20 
    && mapa_copia[cordx+2][cordy-2] != 2){
        printf("DOWN\n");*/

    // Aleatório.
    
    } else if (cordy - 1 > 0 
    && random == 0 
    && mapa_copia[cordx][cordy-1] != 2){
      printf("LEFT\n");

    } else if (cordy + 1 < w
    && random == 1 
    && mapa_copia[cordx][cordy+1] != 2){
      printf("RIGHT\n");

    } else if (cordx - 1 > 0 
    && random == 2 
    && mapa_copia[cordx-1][cordy] != 2){
      printf("UP\n");

    } else if (cordx + 1 < h 
    && random == 3 
    && mapa_copia[cordx+1][cordy] != 2){
      printf("DOWN\n");

    // Verifica se existe outro barco nas posições a seguir.   

    } else {
      printf("%s\n", verificaInimigo(h,w,cordx,cordy,mapa_copia));
    }
    
    // lê qual foi o resultado da ação (e eventualmente atualiza os dados do bot).
    
    scanf("%s", line);
  }
  return 0;
}