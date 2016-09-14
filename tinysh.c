#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUFFER 1024

int main(){
    int quit = 0;
    int reg;
    regex_t regex;

    do{
        char line[BUFFER];  //get command line
        char* commands[512];
        printf("Type Away Your Sin$ ");                   

            if(!fgets(line, BUFFER, stdin)){  
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
        int number_quits = 0;
        childPids = malloc(numberOfChildren * sizeof(pid_t));
        for (int x = 0; x < j; ++x)
        {
            char* argv[400];     
            int argc;               


            int c_i = 0;
            int non_white = 0;
            while(commands[x][c_i]){
                if (!isspace(commands[x][c_i]))
                {
                    non_white = 999;
                }
                c_i++;
            }

            if (non_white!=999)
            {
                number_quits++;
                continue;
            }

            char *token;                  
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
                number_quits++;
                continue;
            }

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
        int n = numberOfChildren - number_quits;
        while (n > 0) {
          pid = wait(&status);
          printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
          --n; 
        }

    }while(!quit);
} 