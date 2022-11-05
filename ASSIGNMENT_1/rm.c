#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// implement rm command with -i and -f options usingremove() function
int main(int num, char* args[]){
    if(args[1]==NULL){
        printf("rm: missing operand\n");
    }
    else if(strcmp(args[1],"-i")==0){   // prompt user for confirmation before removing each file
        if(args[2]==NULL){
            printf("rm: missing operand\n");
        }
        else{
            char *file=args[2];
            printf("rm: remove regular file '%s'? (y/n)",file);
            char ch;
            scanf("%c",&ch);
            if(ch=='y'){
                remove(file);
            }
        }
    }
    else if(strcmp(args[1],"-f")==0){   // ignore nonexistent files and arguments, never prompt user before removing
        char *file=args[2];
        remove(file);    
    }
    else{
        char *file=args[1];
        remove(file);
    }
}