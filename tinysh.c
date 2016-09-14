#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

int main(){
    // char* path= "/bin/";    //set path at bin


while(1){
    char line[BUFFER_LEN];  //get command line
    char* commands[400];
    printf("My shell>> ");                    //print shell prompt

        if(!fgets(line, BUFFER_LEN, stdin)){  //get command and put it in line
        break;                                //if user hits CTRL+D break
    }
    size_t length = strlen(line);
    if (line[length - 1] == '\n')
        line[length - 1] = '\0';
    if(strcmp(line, "quit\n")==0){            //check if command is quit
        break;
    }

    char *token1;
    const char delim[2] = ";";

    token1 = strtok(line,delim);
    int j=0;
    while(token1!=NULL){
        commands[j]=token1;      
        token1 = strtok(NULL,delim);
        j++;
    }
    commands[j]=NULL; 
    int numberOfChildren = j;
    pid_t *childPids = NULL;
    pid_t p;
    childPids = malloc(numberOfChildren * sizeof(pid_t));
    for (int x = 0; x < j; ++x)
    {
        char* argv[400]; 
        char progpath[200];      //full file path
        int argc;               //arg count


        // printf( " %s\n", commands[x]);
        

        char *token;                  //split command into separate strings
        token = strtok(commands[x]," ");
        int i=0;
        while(token!=NULL){
            argv[i]=token;      
            token = strtok(NULL," ");
            i++;
        }
        argv[i]=NULL;                     //set last value to NULL for execvp

        argc=i;                           //get arg count
        // for(i=0; i<argc; i++){
        //     printf("%s\n", argv[i]);      //print command/args
        // }
        // strcpy(progpath, path);           //copy /bin/ to file path
        strcat(progpath, argv[0]);            //add program to path

        for(int yy=0; yy<strlen(progpath); yy++){    //delete newline
            if(progpath[yy]=='\n'){      
                progpath[yy]='\0';
            }
        }

        if ((childPids[x] = fork()) < 0) {
            perror("fork");
            abort();
          } 
        else if (childPids[x] == 0) {
            execvp(progpath,argv);
            printf("%s with %s\n", "Error",progpath);
            exit(0);
          }
          progpath[0] = '\0';
    
    }
    int status;
    pid_t pid;
    int n = numberOfChildren;
    while (n > 0) {
      pid = wait(&status);
      printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
      --n;  // TODO(pts): Remove pid from the pids array.
    }
}
} 