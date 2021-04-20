/*Zachery Abbas
  1019463
  Due: Feb 5, 2020
*/
#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    //declare variables here
    char *token;
    //giving enough space for 200 characters 
    char dir[200];
    int num = 0;
    int state = 0;
    int tokenCounter;
    int shellLooper = 0;
    int waiter = 0;
    char *userInput;
    char **cmd;
    pid_t *childBack;
    pid_t childpid;

    //malloc space here for input, commands, and child back, multiplying it by that x amount just to give a 
    //resonable amount of space 

    userInput = malloc(sizeof(char) * 400);
    cmd = malloc(sizeof(char) * 200);
    childBack = malloc(sizeof(pid_t) * 100);
    //Shell loop

    while (shellLooper == 0)
    {
        //use getcwd fucntion
        getcwd(dir, sizeof(dir));

        printf("%s$ ", dir);
        fgets(userInput, 399, stdin);

        //when user types exit string compare function
        if (strcmp("exit\n", userInput) == 0)
        {

            printf("myShell terminating...\n");
            //tell the shell looper too be 1 so that it gets out of loop
            shellLooper = 1;
        }
        //parsed through ths to make sure my code is able to understand the userInputs 
        //adds null terminator.
        userInput[strlen(userInput) - 1] = '\0';
        //using strtok to parse through token
        token = strtok(userInput, " ");
        //token counter is at -1
        tokenCounter = -1;

        //got this algorithm from class lectures and notes how to fork, parents and childs

        while (token != NULL)
        {
            //start of loop when token has a value that isnt exit
            tokenCounter++;
            //malloc space
            cmd[tokenCounter] = malloc(sizeof(char) * strlen(token));
            strcpy(cmd[tokenCounter], token);
            token = strtok(NULL, " ");
        }

        //background
        if (strcmp("&", cmd[tokenCounter]) == 0)
        {

            cmd[tokenCounter] = '\0';
            waiter = 1;
        }

        tokenCounter++;
        //set to NULL
        cmd[tokenCounter] = NULL;

        //this is when user uses cd using
        if (strcmp("cd", cmd[0]) == 0)
        {
            //https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
            //used the sight above to help me use the chdir 
            chdir(cmd[1]);
        }
        //if doesn't use CD
        else
        {

            childpid = fork();

            if (childpid >= 0)
            {
                if (childpid == 0 && shellLooper != 1)
                {

                    execvp(cmd[0], cmd);
                    printf("Instruction not found please enter another one\n");
                    exit(state);
                }
                else
                {

                    if (waiter != 1)
                    {

                        waitpid(childpid, &state, 0);
                    }
                    else
                    {

                        childBack[num] = childpid;
                        num++;
                    }
                }
            }
            else
            {

                perror("fork");
                exit(-1);
            }
        }

        while (tokenCounter >= 0)
        {
            //free the cmd[counter]
            free(cmd[tokenCounter]);
            tokenCounter--;
        }
    }
    
    free(cmd);
    free(userInput);

    while (num > -1)
    {
        //kills shell
        kill(childBack[num], SIGKILL);
        num--;
    }

    return 0;
}
