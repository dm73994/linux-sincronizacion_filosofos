#include <pthread.h> //-pthread
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

typedef sem_t semaphore;

#define up sem_post
#define down sem_wait

semaphore mutex;
semaphore empty;
semaphore full;

int *items; //Arreglo para almacenar los items
int pos; //Posición del siguiente item a insertar

int N;//Espacio en buffer.

void * producer(void * arg);
void * consumer(void * arg);
int  produceItem();
void insert_item(int item);
int remove_item();
void consume_item(int item);

int main(int argc, char* argv[])
{
    pthread_t t_prod;
    pthread_t t_cons;
    //Comprobar el numero de parámetros por linea de comandos
    if(argc != 2)
    {
        fprintf(stderr,"Debe ingresar el tamaño del buffer\n");
        exit(EXIT_FAILURE); 
    }
    //Iniciar la variable del buffer
    N = atoi(argv[1]);
    //Comprobar el tamaño del buffer
    if(N <= 0)
    {
        fprintf(stderr,"Tamaño de buffer no válido, tamaño del buffer debe ser mayor a 0.\n");
        exit(EXIT_FAILURE);
    }
    //reservar memoria
    items = (int*) malloc(N*sizeof(int));
    //Iniciar los semaforos
    sem_init(&empty,0,N);
    sem_init(&mutex,0,1);
    sem_init(&full,0,0);

    //crear dos hilos
    pthread_create(&t_prod,NULL,producer,NULL);
    pthread_create(&t_cons,NULL,consumer,NULL);

    //Dar espacio a que los hilos se ejecuten
    sleep(5);
    exit(EXIT_SUCCESS);
}

void * producer(void * arg)
{
    //Variable donde se almacena el item a insertar
    int item;
    while(1)
    {
        item = produceItem();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void * consumer(void * arg)
{
    //Variable donde se almacena el item recuperado.
    int item;
    while(1)
    {
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
    }
}

int  produceItem()
{
    printf("Produciendo item...\n");
    int item = rand()%N;
    usleep(rand()%500000);
    printf("Nuevo item: %d.\n", item);
    return item;
}

void insert_item(int item)
{
    items[pos] = item;
    pos++;
}

int remove_item()
{
    printf("Removiendo item...\n");
    usleep(rand()%500000);
    int item = items[pos-1];
    pos--;
    items[pos] = -1;
    return item;
}

void consume_item(int item)
{
    printf("Consumiento el item: %d.\n",item);
}