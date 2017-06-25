/*
UNIVERSIDADE FEDERAL DO AMAZONAS - UFAM
SISTEMAS OPERACIONAIS
BARBEARIA GOURMET - USANDO THREAD
ALUNOS:
CRISTHIAN G. L. DE OLIVEIRA
IVO STINGHEN
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 20
#define N_CADEIRAS 3
#define N_BARBEIROS 3

sem_t sem_cadeiras;
sem_t sem_sofa;
sem_t sem_cad_barbeiro;
sem_t sem_cabelo_cortado;
sem_t sem_cliente_cadeira;
sem_t sem_cliente_sofa;
sem_t sem_espera;

//função usada na thread para o barbeiro cortar cabelo
void* f_barbeiro(void *v) {
    int id_barbeiros = *(int*) v;

    while(1) {
        sem_wait(&sem_cliente_cadeira); //espera por cliente na cadeira
        printf("Barbeiro %d cortou o cabelo de um cliente.\n", id_barbeiros);
        sem_post(&sem_cabelo_cortado); //corta o cabelo
    }
    return NULL;
}

//função usada na thread para cliente entrar, sentar na cadeira e deixar barbearia
void* f_cliente(void* v) {
    int id = *(int*) v;

    if (sem_trywait(&sem_cadeiras) == 0) {//cliente só entra se tiver uma cadeira de barbeiro vazia
        printf("Cliente %d entrou na barbearia.\n", id); //cliente entrou
        sem_wait(&sem_cad_barbeiro); //espera pela cadeira do barbeiro
        printf("Cliente %d sentou na cadeira do barbeiro.\n", id);
        sem_post(&sem_cliente_cadeira); //cliente senta na cadeira
        sem_post(&sem_cadeiras); //cadeira ocupada
        sem_wait(&sem_cabelo_cortado); //corta cabelo
        sem_post(&sem_cad_barbeiro); //ocupa cadeira do barbeiro
        printf("Cliente %d deixou a barbearia.\n", id); //cliente sai
    } else if(sem_trywait(&sem_sofa) == 0){
        printf("Cliente %d entrou na barbearia.\n", id); //cliente entrou
        //sem_wait(&sem_sofa);
        printf("Cliente %d está esperando no sofá\n", id);
        sem_post(&sem_sofa);
    }else if(sem_trywait(&sem_espera) == 0){
        printf("Cliente %d entrou na barbearia.\n", id); //cliente entrou
        //sem_wait(&sem_espera);
        printf("Cliente %d está esperando em pé\n", id);
        sem_post(&sem_espera);
    }else{
        printf("Cliente %d não entrou na barbearia porque está lotada.\n", id); //barbearia cheia
    }
    return NULL;
}

int main() {
    //crio as threads que vão ser os clientes e o barbeiro
    pthread_t thr_clientes[N_CLIENTES], thr_barbeiro[N_BARBEIROS];
    int i, j, id[N_CLIENTES], id_barbeiros[N_BARBEIROS];

    //inicia semafaro com 3 cadeiras
    sem_init(&sem_cadeiras, 0, 3);
    //inicia semafaro com sofa de 4 lugares
    sem_init(&sem_sofa, 0, 4);
    //inicia semafaro com 3 barbeiros
    sem_init(&sem_cad_barbeiro, 0, 3);
    //inicia semafaro com 0 clientes na cadeira
    sem_init(&sem_cliente_cadeira, 0, 0);
    //inicia semafaro com 0 clientes no sofa
    sem_init(&sem_cliente_sofa, 0, 0);
    //inicia semafaro com 0 cabelos cortados
    sem_init(&sem_cabelo_cortado, 0, 0);
    //inicia semafaro com 16 espaços livres
    sem_init(&sem_espera, 0, 16);

    //loop para criar as threads clientes
    for (i = 0; i < N_CLIENTES; i++) {
        id[i] = i;
        pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id[i]);
    }

    //loop para criar threads barbeiro
    for (j = 0; j < N_BARBEIROS; j++) {
        id_barbeiros[j] = j;
        pthread_create(&thr_barbeiro[j], NULL, f_barbeiro, (void*) &id_barbeiros[j]);
    }


    //join nas threads clientes criadas
    for (i = 0; i < N_CLIENTES; i++){
        pthread_join(thr_clientes[i], NULL);
    }

    return 0;
}
