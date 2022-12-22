#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

//Length of string=5
//The code adds index at end of string and writes to a fifo line by line
//2 FIFOs are used. 1 for p1 to p2 writing and other for p2 to p1 writing

char *generate_string(int len){
	char alphabets[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *String=malloc(len+1);
	for(int i=0;i<len;i++){
		String[i]=alphabets[rand()%51];
	}
	String[len]='\0';		//add null char
	return String;
}

// typedef struct string_and_index{
// 	char str[5];
// 	int index;
// }string_and_index;

int main(){

	//creating the fifos
	int fd1,fd2;		//file descriptor
	char *p1_to_p2="/tmp/p1_to_p2";		//file path
	char *p2_to_p1="/tmp/p2_to_p1";

	int check_fifo=mkfifo(p1_to_p2,0666);		//fifo creation
	// if(check_fifo==-1){
	// 	printf("ERROR making fifo\n");
	// 	exit;
	// }

	// printf("%d\n",check_fifo);

	//Generating array of 50 strings(length of string =5)
	char string_arr[50][10];		//array of 50 strings
	for(int i=0;i<50;i++){
		strcpy(string_arr[i],generate_string(5));
	}

	// for(int i=0;i<50;i++){
	// 	// printf("%lu\n",strlen(string_arr[i]));
	// 	printf("%s\n",string_arr[i]);
	// }

	char fifo_arr[5][10];		//array of 5 strings to be written to fifo

	// string_and_index* fifo_arr[5];
	// for(int i=0;i<5;i++){
	// 	fifo_arr[i]=(string_and_index *)malloc(sizeof(string_and_index));
	// }

	int start_index=0;
	for(int i=0; i<5; i++){
		sprintf(fifo_arr[i],"%s %d\n",string_arr[start_index+i],(start_index+i));		//add index at the end of string
		// strcpy(fifo_arr[i]->str,string_arr[i+start_index]);
		// fifo_arr[i]->index=start_index+i;
	}

	// for(int i=0;i<5;i++){
	// 	printf("%s",fifo_arr[i]);
	// }
	// start_index=30;
	// for(int i=0; i<5; i++){
	// 	sprintf(fifo_arr[i],"%s %d\n",string_arr[start_index+i],(start_index+i));		//add index at the end of string
	// 	// strcpy(fifo_arr[i]->str,string_arr[i]);
	// 	// fifo_arr[i]->index=i;
	// }
	// for(int i=0;i<5;i++){
	// 	printf("%s",fifo_arr[i]);
	// }


	int *returned_index;
	int return_ind=0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
	while(1){
		fd1=open(p1_to_p2,O_WRONLY);		//open in write mode
		if(fd1==-1){
			printf("ERROR in P1 write\n");
			break;
		}
		// printf("%d\n",fd);
		int c;
		//write 5 strings to the fifo
		// write(fd1,fifo_arr,sizeof(fifo_arr));
		for(int i=0; i<5;i++){
			c=write(fd1,fifo_arr[i],sizeof(fifo_arr[i]));
			// printf("%d %s\n",c,fifo_arr[i]);
		}
		// printf("P1 executed till line 83\n");
		close(fd1);

		// sleep(5);
		// printf("P1 executed till line 85\n");
		fd2=open(p2_to_p1,O_RDONLY);		//now open in read mode
		if(fd2==-1){
			printf("ERROR in P1\n");
			break;
		}
		// read(fd2,returned_index,sizeof(int));
		read(fd2,&return_ind,sizeof(int));
		close(fd2);
		// return_ind=*(returned_index);

		// start_index+=5;
		start_index=return_ind+1;
		if(start_index==50){
			break;
		}
		//get next 5 strings

		// for(int i=start_index; i<(start_index+5); i++){
		// 	sprintf(fifo_arr[i],"%s %d\n",string_arr[i],i);		//add index at the end of string
		// }
		for(int i=0; i<5; i++){
			sprintf(fifo_arr[i],"%s %d\n",string_arr[start_index+i],(start_index+i));		//add index at the end of string
			// strcpy(fifo_arr[i]->str,string_arr[i+start_index]);
			// fifo_arr[i]->index=start_index+i;
		}
	}
	// printf("P1 working\n");
    clock_gettime(CLOCK_REALTIME, &end);
	unlink(p1_to_p2);
	unlink(p2_to_p1);
	printf("Time taken = %lf seconds\n",( (end.tv_sec*1000000000 - start.tv_sec*1000000000)+(end.tv_nsec - start.tv_nsec))/1000000000.0);
	exit(0);
	return 0;
}