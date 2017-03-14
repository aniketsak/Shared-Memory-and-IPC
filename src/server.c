#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "snappy.h"
#include "map.h"
#include "util.h"
#include <string.h>
#include <pthread.h>



typedef struct msgbuf {
         long    mtype;
         key_t   key, msg_q;
	 size_t size;
         } message_buf;

typedef struct ipcbuf {
	size_t shmsize;
	size_t shmsize_max;
	size_t inputsize_max;
	pthread_mutex_t mmutex;
	pthread_mutexattr_t attrmutex;
}ipcbuf;	
	

typedef struct clientbuf{
	long mtype;
	size_t size;
} clientbuf;

main(int argc, char *argv[])

{
if(argc < 2)
{	printf("Enter the Number of segments and the segment size\n");
	exit(1);
}
else
{	ipcbuf ipc_buf;

	int shmid, out_shmid;
	int shmkey_id;
	char *shm, *s;
	int msqid, msqid2;
	int msgflg = IPC_CREAT | 0666;
	key_t key, shmkey;
	message_buf rbuf;
	clientbuf c_buf;
	ipcbuf *shmemkey;
	shmkey = 3456;
    	size_t buf_length;
size_t SHMSZ = 100;

	if ((shmkey_id = shmget(shmkey, sizeof(ipc_buf), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
	

	if ((shmemkey = shmat(shmkey_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

	pthread_mutexattr_init(&(shmemkey->attrmutex));
	pthread_mutexattr_setpshared(&(shmemkey->attrmutex), PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(shmemkey->mmutex), &(shmemkey->attrmutex));

	shmemkey->shmsize = (size_t)(atoi(argv[1])*atoi(argv[2]));
	shmemkey->shmsize_max = shmemkey->shmsize;
	if(shmemkey->shmsize >= 33)
{
	shmemkey->inputsize_max = (6*((shmemkey->shmsize) -32 ))/7;
}
	else{
	printf("Memory Allocation Too low\n");
	shmdt(shmemkey);
	shmctl(shmkey_id, IPC_RMID, NULL);
	exit(1);
		}
	key = 1234;
	(void) fprintf(stderr, "\nmsgget: Calling msgget(%#lx,\%#o)\n", key, msgflg);

   	if ((msqid = msgget(key, msgflg )) < 0) {
        	perror("msgget");
        	exit(1);
   	 }
    	else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);

while(1)
{
if (msgrcv(msqid, &rbuf, sizeof(message_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}

printf("Input file size received\n");

if ((msqid2 = msgget(rbuf.msg_q, 0666 )) < 0) {
        	perror("msgget");
        	exit(1);
   	 }
    	else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid2);



c_buf.size = snappy_max_compressed_length(rbuf.size);
c_buf.mtype = 1;

if (msgsnd(msqid2, &c_buf, sizeof(c_buf), IPC_NOWAIT) < 0) {
       printf ("%d, %d, %d, %d\n", msqid2, c_buf.mtype, c_buf.size, sizeof(c_buf));
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %d Sent\n", c_buf.size);


(void) fprintf(stderr, "\msgget: Calling msgget(%#lx,\%#o)\n", rbuf.msg_q, msgflg);



if (msgrcv(msqid2, &c_buf, sizeof(c_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}



if ((shmid = shmget(rbuf.key, c_buf.size, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }


char* oshm = (char *) malloc((int) c_buf.size);

struct snappy_env env;
snappy_init_env(&env);
int err;

err = snappy_compress(&env, shm, rbuf.size, oshm, &c_buf.size);
int ij=0;
for(; ij<c_buf.size; ++ij)
*(shm+ij) = *(oshm+ij);

free(oshm);

//do{
//printf("%c\n", *oshm);
//}
//while(*oshm++ != EOF);

/*
printf("Received\n");
do
{
*out++ = *in;
printf("%d\n",i++);
}while(*in++!= EOF);
*/

if (msgsnd(msqid2, &c_buf, sizeof(c_buf), IPC_NOWAIT) < 0) {
       printf ("%d, %d, %d, %d\n", msqid2, c_buf.mtype, c_buf.size, sizeof(c_buf));
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %d Sent\n", c_buf.size);
printf("File compressed and put in the shared memory\n");

}
 
	exit(0);

}
}
