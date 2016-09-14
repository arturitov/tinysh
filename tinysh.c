#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024

int main(){
    int quit = 0;

    do{
        char line[BUFFER];  //get command line
        char* commands[400];
        printf("Type Away Your Sin$ ");                    //print shell prompt

            if(!fgets(line, BUFFER, stdin)){  //get command and put it in line
            break;                                //if user hits CTRL+D break
        }
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
            line[length - 1] = '\0';

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


            char *token;                  //split command into separate strings
            token = strtok(commands[x]," ");
            int i=0;
            while(token!=NULL){
                argv[i]=token;      
                token = strtok(NULL," ");
                i++;
            }
            argv[i]=NULL;                     //set last value to NULL for execvp
            argv[i+1]=NULL;
            argc=i; 
            for(int yy=0; yy<strlen(argv[0]); yy++){    //delete newline
                if(argv[0][yy]=='\n'){      
                    argv[0][yy]='\0';
                }
            }
            if(strcmp(argv[0], "quit")==0){            //check if command is quit
                quit = 1;
                continue;
            }

            printf("%d\n", quit);

            if ((childPids[x] = fork()) < 0) {
                perror("fork");
                abort();
              } 
            else if (childPids[x] == 0) {
                execvp(argv[0],argv);
                printf("%s with %s\n", "Error",argv[0]);
                exit(0);
              }
              argv[0][0] = '\0';
        
        }
        int status;
        pid_t pid;
        int n = numberOfChildren;
        while (n > 0) {
          pid = wait(&status);
          printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
          --n; 
        }

    }while(!quit);
} 