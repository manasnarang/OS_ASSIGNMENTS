#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
// typedef struct string_and_index{
// 	char str[10];
// 	int index;
// }string_and_index;

int main(){
	int fd1,fd2;		//file descriptor
	char *p1_to_p2="/tmp/p1_to_p2";		//file path
	char *p2_to_p1="/tmp/p2_to_p1";
	mkfifo(p2_to_p1,0666);
	int highest_index=0;
	char received_strings[10];
	// string_and_index* received_strings[5];
	// for(int i=0;i<5;i++){
	// 	received_strings[i]=(string_and_index *)malloc(sizeof(string_and_index));
	// 	// strcpy(received_strings[i],"");
	// }
	char write_index[2];
	char index[2];
	while(highest_index!=49){
		// sleep(10);
		//read strings
		fd1=open(p1_to_p2,O_RDONLY);
		if(fd1==-1){
			printf("ERROR in P2 read\n");
			break;
		}
		for(int i=0;i<5;i++){
			read(fd1,received_strings,sizeof(received_strings));
			printf("%s",received_strings);
		}
		index[0]=received_strings[6];
		index[1]=received_strings[7]!='\n'?received_strings[7]:'\0';
		sscanf(index,"%d",&highest_index);
		// highest_index=atoi(index);

		// if(received_strings[7]!='\n'){
		// 	highest_index=((int)received_strings[6])*10;
		// 	highest_index+=(int)received_strings[7];
		// }
		// else{
		// 	highest_index=(int)received_strings[6];
		// }

		// read(fd1,received_strings,sizeof(received_strings));
		close(fd1);
		//Print received strings
		// for(int i=0;i<5;i++){
		// 	// printf("%s",received_strings[i]);
		// 	printf("%s %d\n",received_strings[i]->str,received_strings[i]->index);
		// }
		// printf("P2 working till line 35\n");
		//get highest index
		// char *index=&received_strings[4][6];
		// highest_index=atoi(index);
		// highest_index=received_strings[4]->index;
		//write highest index
		fd2=open(p2_to_p1,O_WRONLY);
		if(fd2==-1){
			printf("ERROR in P2 write\n");
			break;
		}
		// sprintf(write_index,"%d",highest_index);
		write(fd2,&highest_index,sizeof(highest_index));
		close(fd2);
		// printf("P2 working till line 49\n");
	}
	unlink(p1_to_p2);
	unlink(p2_to_p1);
	exit(0);
}