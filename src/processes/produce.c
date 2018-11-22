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
	int i,j,x,k;
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

	mqd_t qdes;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	attr.mq_maxmsg  = maxmsg;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */
	char *filename = "/msg_q";
	mq_unlink(filename);
	qdes = mq_open(filename, O_RDWR | O_CREAT, mode, &attr);

	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}

	for (i=0; i<num_p; i++) {
		int producer = fork();
		if (producer == 0) {
			/*for (j=i;j<num;j+=num_p) {
				if (i == num_p -1 && j+num_p>num) {
					for(i = 0; i < num_c; i++){
						int sign = -1;
						mq_send(qdes, &sign, sizeof(int), NULL);
					}
				}
			}*/
		printf("producers starts \n");
			for(j = 0; j< num; j++){
				if(j % num_p == i){
					mq_send(qdes, &j, sizeof(int), NULL);
					printf("sending %d \n", j);
				}
			}

			exit(0);
		}
	}

	for (k=0; k<num_c; k++) {
		int consumer = fork();
		if (consumer == 0) {
			int receiver;
			// for(detector = mq_receive(qdes, &receiver, sizeof(int), NULL);receiver != -1;){
			// 	if (detector == -1) {
			// 		perror("mq_receive() failed");
			// 	}
			// 	if(sqrt((double)receiver) - floor(sqrt((double)receiver)) == 0){
			// 		printf("%d       %d        %d", i, receiver, sqrt((double)receiver));
			// 	}
			// }
			while(1){
				mq_receive(qdes, &receiver, sizeof(int), NULL);
				printf("Entered and received %d \n", receiver);
				if(sqrt((double)receiver) - floor(sqrt((double)receiver)) == 0){
					printf("%d       %d        %f\n", i, receiver, sqrt((double)receiver));
				}
			}
			exit(0);
		}
	}

    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
	exit(0);
}



// waitpid(int pid, NULL, 0);//回收
// producer是否全回收
// mq_getattr(mqid, &testAttr)//testAttr is struct mq_attr
// attr.mq_curmsgs//剩余数量
// kill(pid, 9);
