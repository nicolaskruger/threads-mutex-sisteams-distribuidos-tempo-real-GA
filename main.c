#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool done = false;
int vector[10] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

void log(char * mensagem, int temperatura){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char str[200];
    FILE *f = fopen("log_sensor.txt", "a");

    sprintf(str, "time: %d-%02d-%02d %02d:%02d:%02d | ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(str + strlen(str), "temp: %d °C | mensagem: ", temperatura);
    sprintf(str + strlen(str), mensagem);
    sprintf(str + strlen(str), "\n");
    fprintf(f, str);
    fclose(f);
    printf(str);
}

void *ta(void *args)
{
    pthread_mutex_lock(&mutex);
    for (size_t i = 0; i < 10; i++)
    {
        vector[i] = rand() * 60.0 / RAND_MAX;
    }
    done = true;
    pthread_mutex_unlock(&mutex);
}

void await(){
    pthread_mutex_lock(&mutex);
    bool await = !done;
    pthread_mutex_unlock(&mutex);
    while (await)
    {
        pthread_mutex_lock(&mutex);
        bool await = !done;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void *tb(void *args)
{
    await();
    pthread_mutex_lock(&mutex);
    for (size_t i = 0; i < 10; i++)
    {
        int temperatura = vector[i];
        if(temperatura < 20){
            log("temperatura menor que 20 graus celsios", temperatura);
        }
        if(temperatura > 30){
            log("temperatura maior que 30 graus celsios", temperatura);
        }
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{

    pthread_t thread_a, thread_b;

    pthread_create( & thread_a, NULL, & ta, NULL );
    pthread_create( & thread_b, NULL, &tb, NULL);

    pthread_join( thread_a, NULL);
    pthread_join( thread_b, NULL);

    exit(0);
}

