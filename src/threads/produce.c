// Use this to see if a number has an integer square root
#define EPS 1.E-7


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
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

circuler_arr arr;
int maxmsg;
int num;
int num_p;

sem_t e;
sem_t f;
sem_t a;

void* produce( container_p param ) {
	container_p upper = param;
	printf('p1');
	for (int i = upper.index; i < num; i += num_p) {
		printf('p2');
        sem_wait( &f );
        sem_wait( &a );
			int newend = arr.end + 1;
			if (newend == maxmsg) {
				newend = 0;
			}
 			arr.buffer[newend] = i;
			arr.end = newend;
		sem_post( &e );
        sem_post( &a );
	}
    printf("A producer has finished.\n");
	pthread_exit(0);
}

void* consume( container_p param ) {
	container_p upper = param;
	int count, popnum, newstart  = 0;
	printf('c1');
	while (count < upper.size) {
		printf('c2');
        sem_wait( &e );
        sem_wait( &a );
			int newstart = arr.start + 1;
			if (newstart == maxmsg) {
				newstart = 0;
			}
			popnum = arr.buffer[arr.start];
			arr.start = newstart;
			count++;
			if (count >= upper.size - num_p) {
				pthread_exit(0);
			}
			printf("%d       %d       ", upper.index, popnum);
			if(sqrt((double)popnum) - floor(sqrt((double)popnum)) == 0){
				printf("%d       %d        %d", upper.index, popnum, (int)sqrt((double)popnum));
			}
		sem_post( &f );
        sem_post( &a );
	}
    printf("A consumer has finished.\n");
	//pthread_exit(0);
}

int main(int argc, char *argv[])
{
	printf("I am here 4");
	int i,j,k,l;
	int num_c;
	int p;
	struct timeval tv;
	
	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}
	printf("I am here1");
	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	pthread_t tid_p[num_p];
	pthread_t tid_c[num_c];


	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	arr.buffer = malloc(sizeof(int)*maxmsg);
	arr.start = 0;
	arr.end = 0;

	sem_init( &a, 0, 1 );
	sem_init( &f, 0, maxmsg);
	sem_init( &e, 0, 0);

	printf("I am here2");
	for ( int i = 0; i < num_p; ++i ) {
		p = i;
		printf("I am here3");
		pthread_create(&tid_p[i], NULL, produce, &p);
    }
	printf("I am here4");
	for ( int j = 0; j < num_c; ++j ) {
		p = i;
		printf("I am here5");
		pthread_create(&tid_c[i], NULL, consume, &p);
    }
	printf("I am here6");


	for ( int k = 0; k < num_p; ++k ) {
        pthread_join( tid_p[j], NULL );
    }

	for ( int l = 0; l < num_c; ++l ) {
        pthread_join( tid_c[j], NULL );
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


