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
	int * buffer;
	int start;
	int end;
	int size;
} circuler_arr;

typedef struct {
	int size;
	int index;
	int num_producer;
	circuler_arr * buffer_ptr;
} container_p;


int count = 0;
sem_t e;
sem_t f;
sem_t a;

void* produce( container_p param ) {
	printf('p1');
	for (int i = param.index; i < param.size; i += param.num_producer) {
		printf('p2');
        sem_wait( &f );
        sem_wait( &a );
		int newend = param.buffer_ptr->end + 1;
		if (newend == param.buffer_ptr->size) {
			param.buffer_ptr->buffer[0] = i;
			param.buffer_ptr->end = 0;
		}
		param.buffer_ptr->buffer[newend] = i;
		param.buffer_ptr->end = newend;
		sem_post( &e );
        sem_post( &a );
	}
    printf("A producer has finished.\n");
	pthread_exit(0);
}

void* consume( container_p param ) {
	printf('c1');
	while (count < param.size) {
		printf('c2');
        sem_wait( &e );
        sem_wait( &a );
		int popnum, newstart = 0;
		if (param.buffer_ptr->start != param.buffer_ptr->end) {
			newstart = param.buffer_ptr->start + 1;
			popnum = param.buffer_ptr->buffer[param.buffer_ptr->start];
			param.buffer_ptr->buffer[param.buffer_ptr->start] = -1;
			param.buffer_ptr->start = (newstart == param.buffer_ptr->size) ? 0 : newstart;
		} else {
			pthread_exit(0);
		}
		count++;
		if (count >= param.size - param.num_producer) {
			pthread_exit(0);
		}
		printf("$d       $d       ", param.index, popnum);
		if(sqrt((double)popnum) - floor(sqrt((double)popnum)) == 0){
			printf("$d       $d        $d", param.index, popnum, sqrt((double)popnum));
		}
		sem_post( &f );
        sem_post( &a );
	}
    printf("A consumer has finished.\n");
	//pthread_exit(0);
}

int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
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


	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	circuler_arr* buff;
	int buffer[maxmsg];
	buff->buffer = buffer;
	buff->start = 0;
	buff->end = 0;
	buff->size = maxmsg;

	sem_init( &a, 0, 1 );
	sem_init( &f, 0, maxmsg);
	sem_init( &e, 0, 0);

	container_p p;

	p.size = num;
	p.buffer_ptr = buff;
	for ( int i = 0; i < num_p; ++i ) {
		p.index = i;
		p.num_producer = num_p;
		pthread_create(&tid_p[i], NULL, produce, &p);
    }

	for ( int i = 0; i < num_c; ++i ) {
		p.index = i;
		p.num_producer = num_c;
		pthread_create(&tid_c[i], NULL, consume, &p);
    }


	for ( int j = 0; j < num_p; ++j ) {
        pthread_join( tid_p[j], NULL );
    }

	for ( int j = 0; j < num_c; ++j ) {
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


