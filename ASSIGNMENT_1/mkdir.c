#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// implementation of mkdir command with -m and -p options using mkdir() function of c
int main(int num, char* args[]){
    int status;     // variable to check whether directory creation was successful or not
    if(args[1]==NULL){
        printf("mkdir: missing operand\n");
    }
    else if(strcmp(args[1],"-m")==0){       // create directory with mode provided
        if(args[2]==NULL || args[3]==NULL){
            printf("mkdir: missing operand\n");
        }
        else{
            char *mode=args[2];
            char *dir=args[3];
            status=mkdir(dir,mode);
            if(status!=0){
                printf("Unable to create directory or directory already exists\n");
            }
        }
    }
    else if(strcmp(args[1],"-p")==0){       // create directory with parent directories if they don't exist
        if(args[2]==NULL){
            printf("mkdir: missing operand\n");
        }
        else{
            char *dir=args[2];
            status=mkdir(dir,0777);
            if(status!=0){
                printf("Unable to create directory or directory already exists\n");
            }
        }
    }
    else{
        char *dir=args[1];
        status=mkdir(dir,0777);
        if(status!=0){
            printf("Unable to create directory or directory already exists\n");
        }
    }
}