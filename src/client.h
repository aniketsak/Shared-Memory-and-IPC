#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

size_t out_size[1000];

typedef struct msgbuf {
         long    mtype;
         key_t   key, msg_q;
	 size_t size;
         } message_buf;

typedef struct clientbuf{
	long mtype;
	size_t size;
	
} clientbuf;

typedef struct ipcbuf {
	size_t shmsize;
	size_t shmsize_max;
	size_t inputsize_max;
	pthread_mutex_t mmutex;
	pthread_mutexattr_t attrmutex;
}ipcbuf;

void synch_compress(int argc, char *argv []);
void asynch_compress(int argc, char *argv []);
void compressed_files(int argc, char *argv []);
