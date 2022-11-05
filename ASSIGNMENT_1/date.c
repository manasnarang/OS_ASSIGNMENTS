#define X_OPEN_SOURCE
#define __USE_XOPEN
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// #include <dos.h>

// implement linux date command with -u and -R options
int main(int argc, char *argv[]){
    // printf("argv[0]=%s\n", argv[0]);
    if(argc<2){
        time_t t=time(NULL);
        struct tm *tm=localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%c", tm);
        printf("%s %s \n", s, "IST");
    }
    else if(strcmp(argv[1],"-u")==0){   // print UTC date time
        time_t t=time(NULL);
        struct tm *tm=gmtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%c", tm);
        printf("%s %s \n", s, "UTC");
    }
    else if(strcmp(argv[1],"-R")==0){   // print RFC 5322 date time
        time_t t=time(NULL);
        struct tm *tm=localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%a, %d %b %Y %H:%M:%S %z\n", tm);
        printf("%s", s);
    }
    else{
        printf("Invalid option\n");
    }
}