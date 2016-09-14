#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUFFER 1024

int main(int argc, char **argv){
    int quit = 0;
    int reg;
    int bash_mode = 0;
    regex_t regex;
    FILE *f;
    if (argc == 2)
    {
        f = fopen(argv[1], "r");
        bash_mode = 1;
    }
    else if(argc > 2){
        // error
    }


    do{
        char line[BUFFER];  //get command line
        char* commands[512];
        //check to see if we received a fike
        if (bash_mode)
        {
            if (fgets(line, BUFFER - 1, f) != NULL){
                printf(">>>$%s\n", line);
            }
            else{
                break;
            }
        }
        else{
            printf("Type Away Your Sin$ ");                   

            if(!fgets(line, BUFFER, stdin)){  
             break;                                //if user hits CTRL+D break
            }

        }
        size_t length = strlen(line);
        // removes newline character if its there
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
            char* argv_t[400];     
            int argc_t;               


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
                argv_t[i]=token;      
                token = strtok(NULL," ");
                i++;
            }
            argv_t[i]=NULL;                     //set last value to NULL for execvp
            argv_t[i+1]=NULL;
            argc_t=i; 
            for(int yy=0; yy<strlen(argv_t[0]); yy++){    //delete newline
                if(argv_t[0][yy]=='\n'){      
                    argv_t[0][yy]='\0';
                }
            }
            if(strcmp(argv_t[0], "quit")==0){            //check if command is quit
                quit = 1;
                number_quits++;
                continue;
            }

            if ((childPids[x] = fork()) < 0) {
                perror("fork");
                abort();
              } 
            else if (childPids[x] == 0) {
                execvp(argv_t[0],argv_t);
                printf("%s with %s\n", "Error",argv_t[0]);
                exit(0);
              }
              argv_t[0][0] = '\0';
        
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