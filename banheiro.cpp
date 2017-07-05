/*
UNIVERSIDADE FEDERAL DO AMAZONAS - UFAM
SISTEMAS OPERACIONAIS
BANHEIRO UNISSEX
ALUNOS:
CRISTHIAN G. L. DE OLIVEIRA
IVO STINGHEN
*/

#include <pthread.h>
#include <queue>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

/* Para ser utilizado no lock e unlock */
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Números de threads que vão ser criados */
#define NUM_THREADS 5

int numMulher = 0;
int numHomem = 0; 
sem_t homens;
sem_t mulheres;
sem_t banheiro;

queue<string> filaBanheiro;

void UsaBanheiro() {
    usleep(rand() % 200000);
}


void *AdicionaMulher(void *threadid) {
    string pessoa = *reinterpret_cast<string*>(threadid);
    
    usleep(rand() % 200000);

    sem_wait(&mulheres);

    numMulher ++;

    if (numMulher == 1) {
        sem_wait(&banheiro);
        cout << "Estado: Com mulher\n";
    }
    printf("Entrou mulher, total: %d\n", numMulher);
    sem_post(&mulheres);
    

    UsaBanheiro();

    //sai do banheiro



    sem_wait(&mulheres);

    numMulher --;

    printf("Saiu mulher, total: %d\n", numMulher);
    if (numMulher == 0) {
        sem_post(&banheiro);
        printf("Estado: Vazio\n");
    }
   
    sem_post(&mulheres);
    




    sem_post(&mulheres);
}


// void MulherQuerEntrar() {
//     sem_wait(&numMulher);
//     conta_mulheres += 1;
//     if (conta_mulheres == 1) {
//         sem_wait(&banheiro);
//         printf("--------- Estado: Com mulher ---------\n");
//     }
//     printf("Entrou mulher, total: %d\n", conta_mulher);
//     sem_post(&numMulher);
// }

/*tempo em que o homem ou mulher demora pra usar o banheiro*/




/* Função principal */
int main (){
    /* inicializa a semente do random */
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];

    int rc, i;
    /* variável para identificar qual pessoa entrará no banheiro */
    string pessoa[] = {"homem", "mulher"};


    sem_init(&homens, 0, 3);
    sem_init(&mulheres, 0, 3);
    sem_init(&banheiro, 0, 3);





    /* loop para criar as threads desejadas  */
    for(i=0; i < NUM_THREADS; i++){
        /* gerar aleatoriamente numHomens e numMulher */
        int aleato = rand()%2;
       
        cout << "main() : criando thread, " << i << endl;
        //if(pessoa[aleato] == "mulher")
        {
          //  rc = pthread_create(&threads[i], NULL, AdicionaMulher, &pessoa[aleato]);
        }
        //else if(pessoa[aleato] == "homem")
        {
            
            rc = pthread_create(&threads[i], NULL, AdicionaMulher, &pessoa[aleato]);
        }
       
        if (rc){
            cout << "Error: não foi possível criar thread," << rc << endl;
            exit(-1);
        }
    }

    /* Dar join nas threads criadas */
    for (i=0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}
