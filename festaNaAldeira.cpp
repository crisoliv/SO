#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>

using namespace std;

/* Números de índios */
#define NUM_THREADS 5
/* Caldeirão começa vazio */
int caldeirao = 0;

/* Thread para os índios comerem no caldeirão */
void *Comer(void *threadid) {
    long tid;
    tid = (long)threadid;
    caldeirao--; /* comer = retirar uma unidade do caldeirão */
    cout << "comeu a porção índio : " << tid << endl;
    pthread_exit(NULL);
}

/* Thread para o cacique comer caçar */
void *Cacar(void *threadid) {
    int aleato = rand()%3;
    string cacas[] = {"capivara", "mico leão ourado", "onça"};
    cout << "animal foi caçado: " << cacas[aleato] << endl;
    pthread_exit(NULL);
}

int main (){
    /* inicializa a semente do random */
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    for( i=0; i < NUM_THREADS; i++ ){
        cout << "main() : creating thread, " << i << endl;
        rc = pthread_create(&threads[i], NULL, Comer, (void *)i);

        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    pthread_exit(NULL);
}
