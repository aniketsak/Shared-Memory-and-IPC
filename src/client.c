#include "client.h"

void asynch_compress(int argc, char *argv []){
if(argc < 2)
{ printf("File Name Not Specified\n");
  exit(1);
}
else
{
ipcbuf ipc_buf;
int n=1;
for(; n<argc; n++)
{
int fc=0;
char *ff, *shm, *s, *oshm;
FILE *pfile = NULL;
pfile = fopen(argv[n], "r+");
if (pfile == NULL) perror ("Error opening file");
else
{
int msgflg = IPC_CREAT | 0666;
/*
while(*s!=NULL)
{	putchar(*s++);
}
*/

char c;

int shmid, out_shmid, shmkey_id;
 int msqid, msqid2;
    key_t key, shmkey;
clientbuf c_buf;
    shmkey = 3456;
    message_buf  sbuf;
    size_t buf_length;
size_t SHMSZ = 100;
ipcbuf *shmemkey;
  key = 1234;


    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

if ((shmkey_id = shmget(shmkey, sizeof(ipcbuf), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

  if ((shmemkey = shmat(shmkey_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }



fseek(pfile, 0, SEEK_END); // seek to end of file
sbuf.size = ftell(pfile); // get current file pointer
fseek(pfile, 0, SEEK_SET);

sbuf.mtype = 1;
 (void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
sbuf.key = ftok(argv[n],1);
sbuf.msg_q = ftok(argv[n],2);
(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);

if ((msqid2 = msgget(sbuf.msg_q, msgflg )) < 0) {
        	perror("msgget");
        	exit(1);
   	 }
    	else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid2);


buf_length = sizeof (message_buf);
 if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %#lx, %d\n", msqid, sbuf.mtype, sbuf.key, buf_length);
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %#lx and %#lx Sent\n", sbuf.key, sbuf.msg_q);


if (msgrcv(msqid2, &c_buf, sizeof(c_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}

printf("Max required output memory received\n");

out_size[n-1] = c_buf.size;

if(c_buf.size > shmemkey->shmsize_max) {
fprintf(stderr,"File size too high, Max Allowed = %lu\n", shmemkey->inputsize_max);
msgctl(msqid2,IPC_RMID, NULL);
exit(1);
}

while(1) {
if(shmemkey->shmsize >= c_buf.size)
{
	pthread_mutex_lock(&(shmemkey->mmutex));
		if(shmemkey->shmsize >= c_buf.size)
			{	shmemkey->shmsize -= c_buf.size;
				pthread_mutex_unlock(&(shmemkey->mmutex));				
				break;
			}
		else
			pthread_mutex_unlock(&(shmemkey->mmutex));
	
}
}			


if ((shmid = shmget(sbuf.key, c_buf.size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }


if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

printf("Shared Memory created\n");

ff = shm;
/*
while(fc!=EOF)
	{ 
	fc = fgetc(pfile); 
	*ff++ = (char )fc;
	}	
*(ff+1) = NULL;
 */
do {
	*ff = fgetc(pfile);
}while(*ff++ != EOF);

if (msgsnd(msqid2, &c_buf, sizeof(c_buf), IPC_NOWAIT) < 0) {
       printf ("%d, %d, %d, %d\n", msqid2, c_buf.mtype, c_buf.size, sizeof(c_buf));
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %d Sent\n", c_buf.size);


}

}

}

}


void compressed_files(int argc, char *argv[]){

key_t shmk[argc-1];
key_t msg_q[argc-1];
char* shm;
int shmid;
int i=1;
for(;i<argc;++i){
int msqid;
shmk[i-1] = ftok(argv[i],1);
msg_q[i-1] = ftok(argv[i],2);
clientbuf c_buf;
if ((msqid = msgget(msg_q[i-1],0666 )) < 0) {
        	perror("msgget");
        	exit(1);
   	 }
    	else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);


if ((shmid = shmget(shmk[i-1], out_size[i-1], 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }



if (msgrcv(msqid, &c_buf, sizeof(c_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}

printf("Acknowledgement to retrieve compressed files\n");

FILE *pf;

char fooo[100];
strcpy(fooo, argv[i]);



char* fp = strcat(fooo,".snp");

pf = fopen(fp,"w");


if(pf!=NULL) {
int ij=0; 
for(;ij<c_buf.size; ++ij)
fputc(*(shm+ij), pf);
fclose(pf);
}

key_t shmkey = 3456;
int shmkey_id;
ipcbuf *shmemkey;


if ((shmkey_id = shmget(shmkey, sizeof(ipcbuf), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

  if ((shmemkey = shmat(shmkey_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }




pthread_mutex_lock(&(shmemkey->mmutex));
shmemkey->shmsize += out_size[i-1];
pthread_mutex_unlock(&(shmemkey->mmutex));

printf("Asynchronous File compression done\n");

shmdt(shm);
shmctl(shmid, IPC_RMID, NULL);
msgctl(msqid,IPC_RMID, NULL);

//shmctl(out_shmid, IPC_RMID, NULL);

}

}

void synch_compress(int argc, char *argv []){

if(argc < 2)
{ printf("File Name Not Specified\n");
  exit(1);
}
else
{
ipcbuf ipc_buf;
int n=1;
for(; n<argc; n++)
{
int fc=0;
char *ff, *shm, *s, *oshm;
FILE *pfile = NULL;
pfile = fopen(argv[n], "r+");
if (pfile == NULL) perror ("Error opening file");
else
{
int msgflg = IPC_CREAT | 0666;
/*
while(*s!=NULL)
{	putchar(*s++);
}
*/

char c;

int shmid, out_shmid, shmkey_id;
 int msqid, msqid2;
    key_t key, shmkey;
clientbuf c_buf;
    shmkey = 3456;
    message_buf  sbuf;
    size_t buf_length;
size_t SHMSZ = 100;
ipcbuf *shmemkey;
  key = 1234;


    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

if ((shmkey_id = shmget(shmkey, sizeof(ipcbuf), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

  if ((shmemkey = shmat(shmkey_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }



fseek(pfile, 0, SEEK_END); // seek to end of file
sbuf.size = ftell(pfile); // get current file pointer
fseek(pfile, 0, SEEK_SET);

sbuf.mtype = 1;
 (void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
sbuf.key = ftok(argv[n],1);
sbuf.msg_q = ftok(argv[n],2);
(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);

if ((msqid2 = msgget(sbuf.msg_q, msgflg )) < 0) {
        	perror("msgget");
        	exit(1);
   	 }
    	else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid2);


buf_length = sizeof (message_buf);
 if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %#lx, %d\n", msqid, sbuf.mtype, sbuf.key, buf_length);
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %#lx and %#lx Sent\n", sbuf.key, sbuf.msg_q);


if (msgrcv(msqid2, &c_buf, sizeof(c_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}

printf("Max required output memory received\n");

if(c_buf.size > shmemkey->shmsize_max) {
fprintf(stderr,"File size too high, Max Allowed = %lu\n", shmemkey->inputsize_max);
msgctl(msqid2,IPC_RMID, NULL);
exit(1);
}

while(1) {
if(shmemkey->shmsize >= c_buf.size)
{
	pthread_mutex_lock(&(shmemkey->mmutex));
		if(shmemkey->shmsize >= c_buf.size)
			{	shmemkey->shmsize -= c_buf.size;
				pthread_mutex_unlock(&(shmemkey->mmutex));				
				break;
			}
		else
			pthread_mutex_unlock(&(shmemkey->mmutex));
	
}
}			


if ((shmid = shmget(sbuf.key, c_buf.size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }


if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

printf("Shared Memory created\n");

ff = shm;
/*
while(fc!=EOF)
	{ 
	fc = fgetc(pfile); 
	*ff++ = (char )fc;
	}	
*(ff+1) = NULL;
 */
do {
	*ff = fgetc(pfile);
}while(*ff++ != EOF);

if (msgsnd(msqid2, &c_buf, sizeof(c_buf), IPC_NOWAIT) < 0) {
       printf ("%d, %d, %d, %d\n", msqid2, c_buf.mtype, c_buf.size, sizeof(c_buf));
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: %d Sent\n", c_buf.size);



if (msgrcv(msqid2, &c_buf, sizeof(c_buf), 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
}

printf("Acknowledgement to retrieve compressed files\n");

FILE *pf;

char fooo[100];
strcpy(fooo, argv[n]);



char* fp = strcat(fooo,".snp");

pf = fopen(fp,"w");


if(pf!=NULL) {
int ij=0; 
for(;ij<c_buf.size; ++ij)
fputc(*(shm+ij), pf);
fclose(pf);
}

pthread_mutex_lock(&(shmemkey->mmutex));
shmemkey->shmsize += c_buf.size;
pthread_mutex_unlock(&(shmemkey->mmutex));


printf("Synchronous File compression done\n");

shmdt(shm);
shmctl(shmid, IPC_RMID, NULL);
msgctl(msqid2,IPC_RMID, NULL);

//shmctl(out_shmid, IPC_RMID, NULL);
  
}
}
return;
}
}
