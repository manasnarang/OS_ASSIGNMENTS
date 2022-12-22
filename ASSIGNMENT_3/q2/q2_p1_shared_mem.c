#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

typedef struct string_and_index{
	char str[5];
	int index;
}string_and_index;

#define SHM_SIZE 8*sizeof(string_and_index)

char *generate_string(int len){
	char alphabets[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *String=malloc(len+1);
	for(int i=0;i<len;i++){
		String[i]=alphabets[rand()%51];
	}
	String[len]='\0';		//add null char
	return String;
}

//used 1st struct in shared_mem as lock to avoid race condition


int main(){
	//Generating array of 50 strings(length of string =5)
	int fd_1,fd_2;
	char string_arr[50][10];		//array of 50 strings
	for(int i=0;i<50;i++){
		strcpy(string_arr[i],generate_string(5));
	}

	// char* shared_mem;	//will use to share the strings
	// char mem[5][10];

	// string_and_index *shared_memory;
	// string_and_index* memory[5];
	// for(int i=0;i<5;i++){
	// 	memory[i]=(string_and_index *)malloc(sizeof(string_and_index));
	// }
	// void *mem_ptr_1, *mem_ptr_2;

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
	int start_index=0;
	// for(int i=0; i<5; i++){
	// 	sprintf(shared_mem[i],"%s %d\n",string_arr[start_index+i],(start_index+i));	  //add index at the end of string
	// }
	int *returned_index=malloc(2*sizeof(int));
	//creating the shared memory object
	fd_1=shm_open("/mem_obj",O_CREAT | O_RDWR,0600);
	if(fd_1<0){
		printf("Error creating shm object\n");
		return EXIT_FAILURE;
	}
	ftruncate(fd_1, SHM_SIZE );
	string_and_index *shared_mem=(string_and_index *)mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd_1,0);
	// char lock[1]="0";
	while(1){
		shared_mem[0].index=0;
		for(int i=0;i<5;i++){
			strcpy(shared_mem[i+1].str,string_arr[i+start_index]);
			shared_mem[i+1].index=i+start_index;
		}
		shared_mem[0].index=1;		//lock freed
		while(shared_mem[0].index!=0);
		start_index=shared_mem[6].index+1;
		if(start_index==50){ break;}
		// char lock='0';
		// for(int i=0;i<5;i++){
		// 	sprintf(mem[i],"%s %d\n",string_arr[i+start_index],start_index+i);
		// }
		// strcpy(shared_mem,"");
		// strcat(lock,mem[0]);
		// char *shared_mem=(char *)mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd_1,0);
		// shared_mem=malloc(SHM_SIZE);
		// sprintf(shared_mem,"%c%s",lock,mem[0]);
		// strcat(shared_mem,mem[1]);
		// strcat(shared_mem,mem[2]);
		// strcat(shared_mem,mem[3]);
		// strcat(shared_mem,mem[4]);
		// shared_mem[0]='1';		//frees the lock so P2 can read

		// shared_memory=(string_and_index *)mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd_1,0);
		// for(int i=0; i<5; i++){
		// 	// sprintf(mem[i],"%s %d\n",string_arr[start_index+i],(start_index+i));	  //add index at the end of string
		// 	// strcat(shared_mem,mem[i]);
		// 	shared_memory[i].index=i+start_index;
		// 	strcpy(shared_memory[i].str,string_arr[i+start_index]);
		// 	// shared_memory[i]=memory[i];
		// }
		// printf("Working till p1 line 70\n");
		// printf("Shared mem is %s",shared_mem);
		// for(int i=0;i<5;i++){
		// 	sprintf(mem_ptr_1,"%s",shared_mem[i]);
		// }
		// munmap(shared_mem,SHM_SIZE);
		// printf("1st unmap working in p1\n");
		// // close(fd_1);
		// // sleep(5);
		// printf("Works till line 76\n");
		// // fd_2=shm_open("/mem_obj",O_RDONLY,0666);
		// returned_index=(int *)mmap(0,2*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd_1,0);
		// while(returned_index[0]!=1);
		// printf("Works till p1 line 79\n");
		// // sprintf(returned_index,"%s",(char *)mem_ptr_2);
		// munmap(returned_index,2*sizeof(int));
		// // close(fd_2);
		// printf("Works till p1 line 83\n");
		// // close(fd_1);
		// printf("Works till p1 line 85\n");
		// start_index=atoi(returned_index)+1;
		// start_index=returned_index[1]+1;
		// printf("Works till p1 line 88\n");
	}
	munmap(shared_mem,SHM_SIZE);
	close(fd_1);
    clock_gettime(CLOCK_REALTIME, &end);
	printf("Time taken = %lf seconds\n",( (end.tv_sec*1000000000 - start.tv_sec*1000000000)+(end.tv_nsec - start.tv_nsec))/1000000000.0);
	return EXIT_SUCCESS;
	return 0;
}