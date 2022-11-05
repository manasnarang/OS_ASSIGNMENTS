#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// implement cat command with -n and -E options
int main(int num, char* args[]){
    if(args[2]==NULL){
        printf("cat: missing operand\n");
    }
    else if(strcmp(args[1],"-n")==0){   // print line number before each line
        if(args[3]==NULL){
            printf("cat: missing operand\n");
        }
        else{
            char *file=args[3];
            file=strcat(file,"/");
            FILE *fp=fopen(strcat(file,args[2]),"r");
            if(fp==NULL){
                printf("cat: %s: No such file or directory\n",file);
            }
            else{
                char ch;
                int line=1;
                printf("%d ",line);
                ch=fgetc(fp);
                while(ch!=EOF){
                    printf("%c",ch);
                    if(ch=='\n'){
                        line++;
                        ch=fgetc(fp);
                        if(ch!=EOF){
                            printf("%d ",line);
                        }
                    }
                    else{
                        ch=fgetc(fp);
                    }
                }
                fclose(fp);
            }
        }
    }
    else if(strcmp(args[1],"-E")==0){   // print $ at the end of each line
        if(args[3]==NULL){
            printf("cat: missing operand\n");
        }
        else{
            char *file=args[3];
            file=strcat(file,"/");
            FILE *fp=fopen(strcat(file,args[2]),"r");
            if(fp==NULL){
                printf("cat: %s: No such file or directory\n",file);
            }
            else{
                char ch;
                while((ch=fgetc(fp))!=EOF){
                    if(ch=='\n'){
                        printf("$ ");
                    }
                    printf("%c",ch);
                }
                fclose(fp);
            }
        }
    }
    else{
        // printf("here\n");
        char *file=args[2];
        file=strcat(file,"/");
        FILE *fp=fopen(strcat(file,args[1]),"r");
        // printf("%s\n",file);
        if(fp==NULL){
            printf("cat: %s: No such file or directory\n",file);
        }
        else{
            char ch;
            while((ch=fgetc(fp))!=EOF){
                printf("%c",ch);
            }
            fclose(fp);
        }
    }
}