#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct string_and_index{
	char str[5];
	int index;
}string_and_index;

#define SHM_SIZE 8*sizeof(string_and_index)

int main(){
	char index[2];
	int highest_index=0;
	int *write_ind=malloc(2*sizeof(int));
	// char *data;
	// data=malloc(SHM_SIZE);
	int fd = shm_open("/mem_obj",O_RDWR | O_CREAT, 0666);
	// void *mem_ptr1, *mem_ptr2;
	if(fd<0){
		printf("Error opening shm\n");
		return EXIT_FAILURE;
	}
	string_and_index *data=(string_and_index *)mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	while(1){
		// sleep(5);

		// printf("Working till P2 line 33\n");

		while(data[0].index!=1);

		for(int i=1;i<=5;i++){
			printf("%s %d\n",data[i].str,data[i].index);
		}


		highest_index=data[5].index;
		data[6].index=highest_index;
		data[0].index=0;		//lock released
		if(highest_index==49){
			break;
		}
		// printf("Working till P2 line 35\n");
		
		// int len=strlen(data);
		// for(int i=1;i<len;i++){
		// 	printf("%c",data[i]);
		// }
		// mem_ptr1=mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		// string_and_index *data=(string_and_index *)mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		// printf("P2 1st map working\n");
		// for(int i=0;i<5;i++){
		// 	strcpy(data[i],(char *)(mem_ptr1+i));
		// }
		// printf("%lu\n",sizeof(mem_ptr1));
		// printf("%s",(char *)mem_ptr1);

		// strcpy(data,(char *)mem_ptr1);

		// printf("%s",data);
		
		
		// printf("Len = %d\n",len);
		// for(int i=0;i<5;i++){
		// 	printf("%s %d\n",data[i].str,data[i].index);
		// }
		// if(data[47]!='\n'){
		// 	index=((int)data[47])*10;
		// 	index+=(int)data[46];
		// }
		// else{
		// 	index=(int)data[46];
		// }
		// index[0]=data[6];
		// index[1]=data[7]!='\n'?data[7]:'\0';
		// sscanf(index,"%d",&highest_index);
		// close(fd);
		// int fd_2=shm_open("/mem_obj", O_RDWR, 0600);
		// write_ind=(int *)mmap(0,2*sizeof(highest_index),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		// write_ind[0]=0;		//lock acquired
		// write_ind[1]=highest_index;
		// write_ind[0]=1;		//lock released
		// sprintf(mem_ptr2,"%d",index);
		// munmap(write_ind,2*sizeof(highest_index));
		// close(fd_2);
	}
	munmap(data, SHM_SIZE);
	close(fd);
	shm_unlink("/mem_obj");
	return EXIT_SUCCESS;
	return 0;
}