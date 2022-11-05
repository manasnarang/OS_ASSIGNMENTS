#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


char *join_strings(char **arr,char *delimiter, int count){
    char *result = (char*)malloc(100*sizeof(char));
    strcpy(result,arr[0]);
    for(int i=1;i<count;i++){
        strcat(result,delimiter);
        strcat(result,arr[i]);
    }
    return result;
}
void* call_external_command_with_system(void *args){
    char *command = (char*)args;
    system(command);
    return NULL;
}

// Basic Linux Shell that supports echo, cd, pwd internal commands and ls, rm, mkdir, cat, date external commands

int main(){
    char* argv[100];
    char delim[]=" ";
    char input[100];      // stores the user input
    int cnt=0;      // stores number of arguments in argv (command and its operands)
    char *commands;
    commands=(char*)malloc(100*sizeof(char));
    char *token;
    token=(char*)malloc(100*sizeof(char));
    int rc=1;

    while(true){
        cnt=0;        
        printf("$ ");
        fgets(input,30,stdin);
        commands=strtok(input,"\n");
        token = strtok(commands," "); //divides based on " " delimiter

        while(token!=NULL){
            argv[cnt] = token;      // stores the command and its operands in argv
            token = strtok(NULL," ");
            cnt++;
        }

        if(argv[0]==NULL){
            continue;
        }
        else if(strcmp(argv[0],"exit")==0){
            break;
        }
        else{
            if(strcmp(argv[0],"echo")==0){
                if(cnt<2){
                    printf("echo: missing operand\n");
                    continue;
                }
                else{
                    if(strcmp(argv[1],"-n")==0){        // prints without newline character
                        if(cnt<3){
                            printf("echo: missing operand\n");
                        }
                        else{       // looping in case of multiple words in string
                            for(int i=2;i<cnt;i++){
                                printf("%s ",argv[i]);
                            }
                        }
                    }
                    else if(strcmp(argv[1],"--help")==0){       // prints help menu and exits shell
                        printf("Usage: echo [SHORT-OPTION]... [STRING]...\n");
                        printf("Echo the STRING(s) to standard output. \n");
                        printf("SHORT-OPTIONS: \n");
                        printf("-n do not output the trailing newline\n");
                        printf("--help display this help and exit\n");
                        break;
                    }
                    else{ // looping in case of multiple words in string
                        for(int i=1;i<cnt;i++){
                            printf("%s ",argv[i]);
                        }
                        printf("\n");
                    }
                }
            }
            else if(strcmp(argv[0],"cd")==0){
                if(cnt==1){
                    printf("cd: missing operand \n");
                }
                else{
                    if(strcmp(argv[1],"-L")==0){
                        if(cnt==3){
                            chdir(argv[2]);
                        }
                        else{
                            printf("cd: missing operand \n");
                        }
                    }
                    else if(strcmp(argv[1],"-P")==0){
                        if(cnt==3){
                            chdir(argv[2]);
                        }
                        else{
                            printf("cd: missing operand \n");
                        }                    }
                    else{
                        chdir(argv[1]);
                    }
                }
            }
            else if(strcmp(argv[0],"pwd")==0){
                if(cnt==1){
                    char cwd[1024];
                    getcwd(cwd,sizeof(cwd));
                    printf("%s \n",cwd);
                }
                else{ 
                    if(strcmp(argv[1],"-L")==0){
                        char cwd[1024];
                        getcwd(cwd,sizeof(cwd));
                        printf("%s \n",cwd);
                    }
                    else if(strcmp(argv[1],"-P")==0){
                        char cwd[1024];
                        getcwd(cwd,sizeof(cwd));
                        printf("%s \n",cwd);
                    }
                    else{
                        printf("pwd: invalid option -- '%s'\n",argv[1]);
                    }
            }
            }
            // External commands now
            
            else if(strcmp(argv[0],"ls")==0){
                if(strcmp(argv[cnt-1],"&t")!=0){
                    // using fork to create new process and execute ls.c file
                    rc=fork();
                    if(rc<0){
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    }
                    else if(rc==0){     //child process
                        argv[cnt]=NULL;     // last element of argv should be NULL
                        // int check=0;
                        argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/ls";    // 1st arugment is file path
                        execv(argv[0],argv);
                        // printf(check==-1?"command failed\n":"command success\n");
                        exit(0);
                    }
                    else{       //parent process
                        wait(NULL);     //wait for child process to complete
                    }
                    fflush(stdout);
                }
                else{
                    // using pthread_create to create new thread and execute ls.c file using system()
                    pthread_t thread;
                    argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/ls";
                    argv[cnt-1]=NULL;
                    char *argument=join_strings(argv," ",cnt-1);
                    int return_code;
                    pthread_create(&thread,NULL,call_external_command_with_system,(void*)argument);
                    return_code=pthread_join(thread,NULL);
                    // printf(return_code==0?"command success\n":"command failed\n");
                    // pthread_exit(NULL);
                    // exit(1);
                }
            }
            else if(strcmp(argv[0],"mkdir")==0){
                if(strcmp(argv[cnt-1],"&t")!=0){
                    rc=fork();
                    if(rc<0){
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    }
                    else if(rc==0){     //child process
                        argv[cnt]=NULL;
                        argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/mkdir";
                        execv(argv[0],argv);
                        exit(0);
                    }
                    else{       //parent process
                        int rc_wait=wait(NULL);
                    }          
                    fflush(stdout);
                }
                else{
                    pthread_t thread;
                    argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/mkdir";
                    argv[cnt-1]=NULL;
                    char *argument=join_strings(argv," ",cnt-1);
                    int return_code;
                    pthread_create(&thread,NULL,call_external_command_with_system,(void*)argument);
                    return_code=pthread_join(thread,NULL);

                }
            }
            else if(strcmp(argv[0],"rm")==0){
                if(strcmp(argv[cnt-1],"&t")!=0){
                    rc=fork();
                    if(rc<0){
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    }
                    else if(rc==0){     //child process
                        argv[cnt]=NULL;
                        argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/rm";
                        execv(argv[0],argv);
                        exit(0);
                    }
                    else{       //parent process
                        int rc_wait=wait(NULL);
                    }
                    fflush(stdout);
                }
                else{
                    pthread_t thread;
                    argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/rm";
                    argv[cnt-1]=NULL;
                    char *argument=join_strings(argv," ",cnt-1);
                    int return_code;
                    pthread_create(&thread,NULL,call_external_command_with_system,(void*)argument);
                    return_code=pthread_join(thread,NULL);
                }
            }
            else if(strcmp(argv[0],"cat")==0){
                if(strcmp(argv[cnt-1],"&t")!=0){
                    rc=fork();
                    if(rc<0){
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    }
                    else if(rc==0){     //child process
                        argv[cnt+1]=NULL;
                        // set argv[cnt]=user's current location
                        char cwd[1024];
                        getcwd(cwd,sizeof(cwd));
                        argv[cnt]=cwd;
                        argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/cat";
                        // printf("argv[0] is %s\n",argv[0]);
                        // printf("argv[1] is %s\n",argv[1]);
                        // printf("argv[2] is %s\n",argv[2]);
                        execv(argv[0],argv);
                        exit(0);
                    }
                    else{       //parent process
                        int rc_wait=wait(NULL);
                    }
                    fflush(stdout);
                }
                else{
                    pthread_t thread;
                    argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/cat";
                    char cwd[1024];
                    getcwd(cwd,sizeof(cwd));
                    argv[cnt-1]=cwd;
                    argv[cnt]=NULL;
                    char *argument=join_strings(argv," ",cnt);
                    int return_code;
                    pthread_create(&thread,NULL,call_external_command_with_system,(void*)argument);
                    return_code=pthread_join(thread,NULL);
                }
            }
            else if(strcmp(argv[0],"date")==0){
                if(strcmp(argv[cnt-1],"&t")!=0){
                    rc=fork();
                    if(rc<0){
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    }
                    else if(rc==0){     //child process
                        argv[cnt]=NULL;
                        argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/date";
                        execv(argv[0],argv);
                        exit(0);
                    }
                    else{       //parent process
                        int rc_wait=wait(NULL);
                    }
                    fflush(stdout);
                }
                else{
                    pthread_t thread;
                    argv[0]="/mnt/c/Users/ELL/Desktop/OS_ASSIGNMENT_1_FILES/date";
                    argv[cnt-1]=NULL;
                    char *argument=join_strings(argv," ",cnt-1);
                    int return_code;
                    pthread_create(&thread,NULL,call_external_command_with_system,(void*)argument);
                    return_code=pthread_join(thread,NULL);
                }
            }
            else{
                printf("Command not found\n");
            }
        }
    }
    return 0;
}