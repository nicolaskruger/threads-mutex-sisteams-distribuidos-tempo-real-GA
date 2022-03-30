#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int vector[10];

void *ta(void *args)
{
    for (size_t i = 0; i < 10; i++)
    {
        vector[i] = rand();
        printf("%d \n", vector[i]);
    }
}
int main()
{

    pthread_t thread_a;

    pthread_create( & thread_a, NULL, & ta, NULL );

    pthread_join( thread_a, NULL);

    exit(0);
}

