#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <mutex>

using namespace std;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Números de índios */
#define NUM_THREADS 5
/* Caldeirão começa vazio */
int caldeirao = 0;

/* Thread para o cacique caçar */
void *Cacar(void *threadid) {
    pthread_mutex_lock(&count_mutex);
    /* Pega uma caça aleatoriamente*/
    int aleato = rand()%3;
    string cacas[] = {"capivara", "mico leão ourado", "onça"};

    /* Atribui ao caldeirão a porção da respectiva caça */
    if(aleato == 0){
        caldeirao = 10;
    }else if(aleato == 1){
        caldeirao = 2;
    }else{
        caldeirao = 15;
    }

    //cout << "qtde no caldeirao " << caldeirao << endl;

    /* Printa qual animal foi caçado */
    cout << "animal foi caçado: " << cacas[aleato] << endl;

    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}


/* Cria thread para chamar o cacique */
void chamaCacique(){
    pthread_t thread;

    int rc;

    cout << "chamaCacique(): criando thread para o cacique caçar " << endl;
    rc = pthread_create(&thread, NULL, Cacar, NULL);

    if (rc){
        cout << "Error: não foi possível criar thread," << rc << endl;
        exit(-1);
    }

    pthread_join(thread, NULL);
}


/* Thread para os índios comerem no caldeirão */
void *Comer(void *threadid) {
    pthread_mutex_lock(&count_mutex);
    long tid;
    tid = (long)threadid;

    /* verifica se o caldeirão está vazio */
    if(caldeirao == 0){
        /* se o caldeirão ta vazio chama o cacique para caçar */
        chamaCacique();
    }

    caldeirao--; /* comer = retirar uma unidade do caldeirão */
    cout << "comeu a porção índio: " << tid << endl;
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

/* Função principal */
int main (){

    /* inicializa a semente do random */
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];

    int rc;
    int i;

    /* caldeirão começa vazio e chama o cacique pra caçar */
    chamaCacique();

    /* Gera as 5 threads correspondentes aos 5 índios */
    for( i=0; i < NUM_THREADS; i++ ){
        cout << "main() : criando thread para os índios comerem, " << i << endl;
        rc = pthread_create(&threads[i], NULL, Comer, (void *)i);

        if (rc){
            cout << "Error: não foi possível criar thread," << rc << endl;
            exit(-1);
        }

        pthread_join(threads[i], NULL);
    }
}
