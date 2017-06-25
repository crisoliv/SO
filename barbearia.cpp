/*
* Problema do barbeiro dorminhoco.
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 10
#define N_CADEIRAS 5

sem_t sem_cadeiras;
sem_t sem_cad_barbeiro;
sem_t sem_cabelo_cortado;
sem_t sem_cliente_cadeira;

//função usada na thread para o barbeiro cortar cabelo
void* f_barbeiro(void *v) {

    while(1) {
        sem_wait(&sem_cliente_cadeira);
        printf("Barbeiro cortou o cabelo de um cliente.\n");
        sem_post(&sem_cabelo_cortado);
    }
    return NULL;
}

//função usada na thread para cliente entrar, sentar na cadeira e deixar barbearia
void* f_cliente(void* v) {
    int id = *(int*) v;

    sleep(id%3);
    if (sem_trywait(&sem_cadeiras) == 0) {
        printf("Cliente %d entrou na barbearia.\n", id);
        sem_wait(&sem_cad_barbeiro);
        printf("Cliente %d sentou na cadeira do barbeiro.\n", id);
        sem_post(&sem_cliente_cadeira);
        sem_post(&sem_cadeiras);
        sem_wait(&sem_cabelo_cortado);
        sem_post(&sem_cad_barbeiro);
        printf("Cliente %d deixou a barbearia.\n", id);
    } else
    printf("Cliente %d não entrou na barbearia.\n", id);
    return NULL;
}

int main() {
    //crio as threads que vão ser os clientes e o barbeiro
    pthread_t thr_clientes[N_CLIENTES], thr_barbeiro;
    int i, id[N_CLIENTES];

    //inicia semafaro com 5 cadeiras
    sem_init(&sem_cadeiras, 0, 5);
    //inicia semafaro com 1 barbeiro
    sem_init(&sem_cad_barbeiro, 0, 1);
    //inicia semafaro com 0 clientes na cadeira
    sem_init(&sem_cliente_cadeira, 0, 0);
    //inicia semafaro com 0 cabelos cortados
    sem_init(&sem_cabelo_cortado, 0, 0);

    //loop para criar as threads clientes
    for (i = 0; i < N_CLIENTES; i++) {
        id[i] = i;
        pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id[i]);
    }

    //cria thread barbeiro
    pthread_create(&thr_barbeiro, NULL, f_barbeiro, NULL);

    //join nas threads clientes criadas
    for (i = 0; i < N_CLIENTES; i++){
        pthread_join(thr_clientes[i], NULL);
    }

    /* Barbeiro é assassinado */

    return 0;
}
