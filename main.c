/**
 * @file    main.c
 * @author  Austin Kugler
 * @brief   Custom version of the Linux shell.
 * 
 *          Compile: gcc -g -Wall main.c shell.c -o nicshell
 *          Usage:   ./nicshell
 * @date    2021-03-26
 * 
 */
#include <stdio.h>
#include <string.h>
#include "shell.h"


int main(int argc, char *argv[])
{
    printf("Created By: Austin Kugler\n");
    printf("Type \"help\" to see implemented commands\n\n");

    // Create an array of char * (C-strings) and
    // set all elements to NULL
    char *cmdAsArray[MAX_WORDS] = {NULL};

    bgjob_t bglist[MAX_BG];

    for (int i = 0; i < MAX_BG; ++i)
    {
        bglist[i].pid = INVALID_PID;
    }

    // Loop forever until user enters "exit"
    while (1)
    {
        char cmd[MAX_CMD];

        printf("nicshell> "); // Print prompt
        fgets(cmd, MAX_CMD, stdin);

        if (cmd == NULL)
        {
            printf("Unrecognized Command\n");
        }

        // break cmd into an array of strings (cmdAsArray)
        int args = asArrayOfSubstrings(cmd, cmdAsArray);
        // terminate the array of strings with NULL 
        // necessary for execvp to execute properly
        cmdAsArray[args] = NULL;

        // the blank case, when user just press enter in the shell
        if (cmdAsArray[0] == NULL)
        {
            // do nothing
        }
        else if (strcmp(cmdAsArray[0], "exit") == 0)
        {
            // Do other clean up
            cleanUp(cmdAsArray, MAX_WORDS);
            break;
        }
        else if (strcmp(cmdAsArray[0], "help") == 0)
        {
            help();
        }
        else if (strcmp(cmdAsArray[0], "bg") == 0)
        {
            bgClean(bglist);
            bg(cmdAsArray, bglist);
        }
        else if (strcmp(cmdAsArray[0], "bglist") == 0)
        {
            bgClean(bglist);
            bgList(bglist);
        }
        else if (strcmp(cmdAsArray[0], "bgkill") == 0)
        {
            bgClean(bglist);
            bgKill(atoi(cmdAsArray[1]), bglist);
        }
        else
        {
            bgClean(bglist);
            executeInFork(cmdAsArray);
        }

    } // End while loop

    return 0;
}