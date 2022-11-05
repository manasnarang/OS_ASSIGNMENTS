#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// implement ls command with -a and -A options using opendir() and readdir() functions
int main(int argc, char *argv[]){
    // printf("argv[0]=%s\n", argv[0]);
    if(argc==1){
        // printf("Works fine till here\n");
        DIR *dir=opendir(".");      // open current directory
        // printf("Works fine till here\n");
        struct dirent *entry;
        while((entry=readdir(dir))!=NULL){
            if(!(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)){   // check if file is . or ..
                printf("%s ",entry->d_name);
            }
        }
        printf("\n");
        closedir(dir);      // close directory
    }
    else if(strcmp(argv[1],"-a")==0){       // list all files including hidden files
        // printf("Works fine till here\n");
        DIR *dir=opendir(".");
        // printf("Works fine till here\n");
        struct dirent *entry;
        while((entry=readdir(dir))!=NULL){
            printf("%s ",entry->d_name);
        }
        printf("\n");
        closedir(dir);
    }
    else if(strcmp(argv[1],"-A")==0){    // list all files except . and ..
        // printf("Works fine till here\n");
        DIR *dir=opendir(".");
        // printf("Works fine till here\n");
        struct dirent *entry;
        while((entry=readdir(dir))!=NULL){
            if(!(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)){   // check if file is . or ..
                printf("%s ",entry->d_name);
            }
        }
        printf("\n");
        closedir(dir);
    }
    else{
        // printf("Works fine till here\n");
        char *dir=argv[1];
        DIR *d=opendir(dir);
        // printf("Works fine till here\n");
        struct dirent *entry;
        while((entry=readdir(d))!=NULL){
            if(!(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)){   // check if file is . or ..
                printf("%s ",entry->d_name);
            }
        }
        printf("\n");
        closedir(d);
    }
}
