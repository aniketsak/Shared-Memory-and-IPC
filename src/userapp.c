#include "client.h"

main(int argc, char* argv[]){

int k = 0; //k=0-> synchronous, k=1-> asynchronous. User can call the functions accordingly.
printf("Enter 0 for synchronous mode or 1 for asynchronous mode \n");
scanf("%d" , &k);
if(k==0)
{
printf("Working of Synchronous file compress\n");
synch_compress(argc, argv);
printf("Synchronous compress done\n");
}
else
{
printf("Working of Asynchronous file compress\n");
asynch_compress(argc, argv);
printf("The calling user app continues to do its work\n");
compressed_files(argc, argv);
printf("Asynchronous compress done\n");
}
}
