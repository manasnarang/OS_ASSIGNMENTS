#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[]){
	int sock=0;
	struct sockaddr_in serv_addr;
	char buffer[1024]={0};
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
		printf("\nConnection Failed \n");
		return -1;
	}
	int highest_index=0;
	char index[2];
	int newline_count;
	while (1){
		char received_strings[50];
		newline_count=0;
		read(sock,received_strings,50);
		int len=strlen(received_strings);
		// printf("Len = %d\n",len);

		for(int i=0;i<len;i++){
			if(newline_count==5){
			}
			else{
				if(received_strings[i]=='\n'){
					newline_count++;
				}
				printf("%c",received_strings[i]);
			}
		}

		// printf("%s",received_strings);

		// printf("%c\n",received_strings[6]);

		index[0]=received_strings[6];
		index[1]=received_strings[7]!='\n'?received_strings[7]:'\0';
		sscanf(index,"%d",&highest_index);
		highest_index+=4;
		// if(len<46){
		// 	highest_index=(int)received_strings[38];
		// }
		// else{
		// 	highest_index=(int)received_strings[42]*10;
		// 	highest_index+=(int)received_strings[43];
		// }
		send(sock,&highest_index,sizeof(highest_index),0);
		if(highest_index>48){
			break;
		}
	}
	return 0;
}