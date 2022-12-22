#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#define PORT 8080

char *generate_string(int len){
	char alphabets[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *String=malloc(len+1);
	for(int i=0;i<len;i++){
		String[i]=alphabets[rand()%51];
	}
	String[len]='\0';		//add null char
	return String;
}

int main(int argc, char const *argv[]){
	int server_fd, new_socket;
	struct sockaddr_in sock_addr;
	int opt=1;
	int addrlen=sizeof(sock_addr);
	char buffer[1024]={0};

	// Creating socket file descriptor
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	sock_addr.sin_family=AF_INET;
	sock_addr.sin_addr.s_addr=INADDR_ANY;
	sock_addr.sin_port=htons(PORT);

	// Forcefully attaching socket to the port 8080
	if(bind(server_fd,(struct sockaddr *)&sock_addr,sizeof(sock_addr))<0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd,3)<0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if((new_socket=accept(server_fd,(struct sockaddr *)&sock_addr,(socklen_t*)&addrlen))<0){
		perror("accept");
		exit(EXIT_FAILURE);
	}

	//Generating array of 50 strings(length of string =5)
	char string_arr[50][6];		//array of 50 strings
	for(int i=0;i<50;i++){
		strcpy(string_arr[i],generate_string(5));
	}
	// for(int i=0;i<50;i++){
	// 	printf("%s\n",string_arr[i]);
	// }

	//Socket communication

	// char socket_arr[5][10];
	int start_index=0;
	int return_id=0;
	// printf("Hello\n");
	// printf("%s\n\n",string_arr[10]);
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
	while(1){
		char str[50];
		char socket_arr[5][10];
		// sprintf(socket_arr[0],"%s %d\n",string_arr[start_index],start_index);
		for(int i=0;i<5;i++){
			sprintf(socket_arr[i],"%s %d\n",string_arr[i+start_index],start_index+i);
			// if(start_index<10){
			// 	socket_arr[i][8]='\0';
			// }
			// else{
			// 	socket_arr[i][9]='\0';
			// }
		}
		sprintf(str,"%s",socket_arr[0]);
		// strcpy(str,socket_arr[0]);
		strcat(str,socket_arr[1]);
		strcat(str,socket_arr[2]);
		strcat(str,socket_arr[3]);
		strcat(str,socket_arr[4]);
		// strcat(str,"\n\0");

		// int len=strlen(str);
		// str[len-1]='\n';
		// str[len]='\0';
		// for(int i=0;i<5;i++){
		// 	strcpy(socket_arr[i],"");
		// }


		// printf("%s",str);
		// strcpy(str,"");
		// start_index+=5;
		send(new_socket,str,strlen(str),0);

		read(new_socket,&return_id,sizeof(int));

		start_index=return_id+1;
		// printf("P1: Recieved index %i\n",first_id);

		if(start_index>48){
			break;
		}
	}
    clock_gettime(CLOCK_REALTIME, &end);
	printf("\nTime taken = %lf seconds\n",( (end.tv_sec*1000000000 - start.tv_sec*1000000000)+(end.tv_nsec - start.tv_nsec))/1000000000.0);
}