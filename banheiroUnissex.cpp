/*
UNIVERSIDADE FEDERAL DO AMAZONAS - UFAM
SISTEMAS OPERACIONAIS
BANHEIRO UNISSEX
ALUNOS:
CRISTHIAN G. L. DE OLIVEIRA
IVO STINGHEN
*/

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <mutex>
#include <queue>

using namespace std;

/* Para ser utilizado no lock e unlock */
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Números de threads que vão ser criados */
#define NUM_THREADS 5

queue<string> filaBanheiro;

/* Função chamada dentro da Thread para os índios comerem no caldeirão */
void *EntrarBanheiro(void *threadid) {
    string pessoa = *reinterpret_cast<string*>(threadid);
    /* Dar lock na thread */
    pthread_mutex_lock(&count_mutex);
        /* adiciona uma pessoa na fila do banheiro */
        filaBanheiro.push(pessoa);

        if(filaBanheiro.back() == "homen"){
            if(filaBanheiro.size() <= 3){
                cout << pessoa << " entrou no banheiro "<< endl;
                //filaBanheiro.pop();
            }
        }else{
            if(filaBanheiro.size() <= 3){
                cout << pessoa << " entrou no banheiro "<< endl;
                //filaBanheiro.pop();
            }
        }
    /* Dar unlock na thread */
    pthread_mutex_unlock(&count_mutex);

    /* sai da thread */
    pthread_exit(NULL);
}

/* Função principal */
int main (){
    /* inicializa a semente do random */
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];

    int rc, i;
    /* variável para identificar qual pessoa entrará no banheiro */
    string pessoa[] = {"homen", "mulher"};

    /* loop para criar as threads desejadas  */
    for(i=0; i < NUM_THREADS; i++){
        /* gerar aleatoriamente homens e mulheres */
        int aleato = rand()%2;

        cout << "main() : criando thread, " << i << endl;
        rc = pthread_create(&threads[i], NULL, EntrarBanheiro, &pessoa[aleato]);

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
