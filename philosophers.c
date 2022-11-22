#include <pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define LEFT i-1
#define RIGHT i+1
#define up sem_post
#define down sem_wait

enum{
    HUNGRY,
    EATING,
    THINKING
}philosopher_states;

typedef sem_t semaphore;

semaphore mutex;
semaphore *s;

int * state;

/// @brief Administra las acciones de los filósofos.
/// @param i ID del filosofo
/// @return 
void * philosopher(int i);

/// @brief Actualiza el estado de un filosofo a espera de tenedor y verifica si hay tenedores disponibles.
/// @param i ID del filósofo.
void take_forks(int i);

/// @brief Devuelve los tenedores adyacentes y verifica si los filosofos adyacentes pueden comer.
/// @param i 
void put_forks(int i);

/// @brief Revisa si los tenedores adyacentes se encuentran disponibles.
/// @param i 
void test(int i);

int main(int argc, char* argv[])
{
    //Numero de filosofos
    int N;
    //Comprobar si el número de argumentos es correcto, igual a 2
    if(argc != 2)
    {
        fprintf(stderr,"Número de argumentos no valido.\n");
        exit(EXIT_FAILURE);
    }
    //Leer el numero de filososfos de la cadena de comandos: argv[0] = philosophers argv[1] = entero > 2.
    N  = atoi(argv[1]);
    //Comprobar si el número de filosofos cumple con ser mayor a 2.
    if(N <= 2)
    {
        fprintf(stderr,"Número de filósofos no válido.\n");
        exit(EXIT_FAILURE);
    }
    //Resevar memoria para los semaforos.
    s = malloc(N*sizeof(semaphore));
    //Reservar espacio para  la matriz de estados
    state = malloc(N*sizeof(int));
    //Inicializacion de los semaforos y matriz de estados
    for(int i = 0; i < N; i++)
    {
        sem_init(&s[i],0,0);
        state[i] = 0;
    }
    sem_init(&mutex,0,1);

    exit(EXIT_SUCCESS);
}

void * philosopher(int i)
{
    while(1)
    {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
}

void take_forks(int i)
{
    down(&mutex);
    state[i] = HUNGRY;
    test(i);
    up(&mutex);
    down(&s[i]);
}

void put_forks(int i)
{
    down(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(int i)
{
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        up(&s[i]);
    }
}
