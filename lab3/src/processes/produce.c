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

double g_time[2];


int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i,j,x,k,h, counter;
	struct timeval tv;

	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;
	//init queue as reference to the given code example
	mqd_t qdes;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	struct mq_attr attr_1;
	int prod[num_p];
	int cons[num_c];
	attr.mq_maxmsg  = maxmsg;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */
	char *filename = "/msg_q";
	//clear queue and open it
	mq_unlink(filename);
	qdes = mq_open(filename, O_RDWR | O_CREAT, mode, &attr);

	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}
	counter = 0;
	for (i=0; i<num_p; i++) {
		int producer = fork();
		//store parent pid in parent prod[] array
		prod[i] = producer;
		//the correct counter value will be stored in parent
		counter++;
		if (producer == 0) {
			printf("producers starts \n");
			for(j = 0; j< num; j++){
				if(j % num_p == i){
					mq_send(qdes, &j, sizeof(int), NULL);
					//printf("sending %d \n", j);
				}
			}
			exit(0);
		}
	}

	for (k=0; k<num_c; k++) {
		int consumer = fork();
		//store consumer pid in parent cons[] array
		cons[k] = consumer;
		if (consumer == 0) {
			int receiver;
			while(1){
				//will kill consumer in parent after all the tasks are conusmed
				mq_receive(qdes, &receiver, sizeof(int), NULL);
				//printf("Entered and received %d \n", receiver);
				if(sqrt((double)receiver) - floor(sqrt((double)receiver)) == 0){
					printf("%d       %d        %d\n", i, receiver, (int)sqrt((double)receiver));
				}
			}
			//exit(0);
		}
	}
	//logic here is after all the tasks are produced, check if all the producers are killed
	//then if no tasks are right now queued in qdes, it means all the tasks are consumed
	//then it's time to kill all the consumers
	while (1) {
		if (counter == num_p) {
			for(k=0; k<num_p; k++){
				waitpid(prod[k], NULL, 0);
			}

			while(1){
				mq_getattr(qdes, &attr_1);
				if(attr_1.mq_curmsgs == 0){
					for(h = 0;h < num_c; h++){
						kill(cons[h], 9);
					}
					break;
				}
			}
			break;
		}
	}
    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
	exit(0);
}
