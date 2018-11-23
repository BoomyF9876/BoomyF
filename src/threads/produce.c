// Use this to see if a number has an integer square root
#define EPS 1.E-7


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

double g_time[2];

typedef struct {
	int *buffer;
	int start;
	int end;
} circuler_arr;

circuler_arr arr ={
	NULL,
	0,
	0
};
int maxmsg;
int num;
int num_p;
int num_c;
int count = 0;

sem_t e;
sem_t f;
sem_t a;

void* produce( void* param ) {
	int newend = 0;
	for (int i = *(int*)param; i < num; i += num_p) {
        sem_wait( &f );
        sem_wait( &a );
			newend = arr.end + 1;
			if (newend == maxmsg) {
				newend = 0;
			}
 			arr.buffer[newend] = i;
			arr.end = newend;
			if (newend == 1) {
				arr.start++;
			}
			printf("%d\n", i);
		sem_post( &e );
        sem_post( &a );
	}
    printf("A producer has finished.\n");
	pthread_exit(0);
}

void* consume( void* param ) {
	int popnum, newstart  = 0;
	while (1) {
		if (count > num - num_c) {
			printf("A consumer has finished.\n");
			pthread_exit(0);
		}
        sem_wait( &e );
        sem_wait( &a );
			newstart = arr.start + 1;
			if (newstart == maxmsg) {
				newstart = 0;
			}
			popnum = arr.buffer[arr.start];
			arr.buffer[arr.start] = -1;
			arr.start = newstart;
			count++;
			printf("consumed %d\n", popnum);
			if(sqrt((double)popnum) - floor(sqrt((double)popnum)) == 0){
				printf("%d       %d        %d\n", *(int*)param, popnum, (int)sqrt((double)popnum));
			}
		sem_post( &f );
        sem_post( &a );
	}
	//pthread_exit(0);
}

int main(int argc, char *argv[])
{
	int i,j,k,l;
	struct timeval tv;
	
	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	pthread_t tid_p[num_p];
	pthread_t tid_c[num_c];

	int ps[num_p];
	int cs[num_c];

	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	arr.buffer = malloc(sizeof(int)*maxmsg);
	arr.start = -1;
	arr.end = -1;

	sem_init( &a, 0, 1 );
	sem_init( &f, 0, maxmsg);
	sem_init( &e, 0, 0);


	for ( i = 0; i < num_p; ++i ) {
		ps[i] = i;
		pthread_create(&tid_p[i], NULL, produce, &ps[i]);
    }

	for ( j = 0; j < num_c; ++j ) {
		cs[j] = j;
		pthread_create(&tid_c[j], NULL, consume, &cs[j]);
    }

	for ( k = 0; k < num_p; ++k ) {
        pthread_join( tid_p[k], NULL );
    }

	for ( l = 0; l < num_c; ++l ) {
        pthread_join( tid_c[l], NULL );
    }
	
    sem_destroy( &a );
	sem_destroy( &f );
	sem_destroy( &e );

    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
	exit(0);
}


