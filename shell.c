/**
 * @file shell.c
 * @author Austin Kugler
 * @brief Function definitions for the shell program
 * @date 2021-03-26
 * 
 */
#include "shell.h"


/**
 * @brief Creates a child process and executes the command
 *        in cmdAsArray in the child process using execvp. 
 *        Parent process is blocked until child process 
 *        terminates.
 * 
 * @param cmdAsArray  array of strings where [0] holds the 
 *                    name of the program to execute in the shell
 */
void executeInFork(char **cmdAsArray)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Failed to fork()\n");
        return;
    }
    else if (pid == 0) // child process
    {
        execvp(cmdAsArray[0], cmdAsArray);
        fprintf(stderr, "%s: command not found\n", cmdAsArray[0]);
        _exit(127);
    }
    else // parent process
    {
        waitpid(pid, NULL, 0); // blocking
    }
}

/**
 * @brief Tokenizes *cmd* into an array of substrings stored
 *        in the output parameter *argv*.
 * 
 * @param cmd   original string command to tokenize
 * @param argv  array of C-strings
 * @return int  positive value representing number of arguments
 *              0 if argv has no strings
 */
int asArrayOfSubstrings(char *cmd, char **argv) 
{
    // make a copy of the command
    char cmdCopy[MAX_CMD];
    strcpy(cmdCopy, cmd);

    // strtok is initialize to tokenize/break cmdCopy
    // into words using " " space as delimiter
    // strtok returns the first C-string (token) 
    char* token = strtok(cmdCopy, " ");

    // while token is not NULL
    int argC = 0;
    while (token)
    {
        // if element in argv is not null, it means
        // its used previously and data inside needs
        // to be freed up
        if (argv[argC] != NULL)
        {
            free(argv[argC]);
        }

        // request for dynamic memory enough to store
        // the C-string in token + '\0'
        argv[argC] = (char *)malloc(sizeof(char) * (strlen(token) + 1));

        // verify if last element in token is '\n' 
        // then replace with '\0'
        if (token[strlen(token) - 1] == '\n')
        {
            token[strlen(token) - 1] = '\0'; // NULL terminate the string
        }

        // copy string 
        strncpy(argv[argC], token, strlen(token) + 1);
        // get the next token/word in cmdCopy
        token = strtok(NULL, " ");
        // increment argC to store next word 
        // in the next index of argv
        argC++;
    }

    return argC;
}

/**
 * @brief Clean up heap memory
 * 
 * @param argv  array of dynamically allocated C-strings
 * @param size  size of the array
 */
void cleanUp(char **argv, int size)
{
    for (int argC = 0; argC < size; ++argC)
    {
        if (argv[argC] != NULL)
        {
            free(argv[argC]);
        }
    }
}

/**
 * @brief Displays the help message in the shell.
 * 
 */
void help()
{
    printf("pwd  - Print the current working directory\n");
    printf("exit - Exit Shell\n");
    printf("help - Display this message\n\n");
    printf("bglist        - List background programs\n");
    printf("bgkill <int>  - Kill background process <int> int values: 1-5\n");
    printf("<UNIX cmd>    - Spawn child process, execute <UNIX cmd> in the foreground\n");
    printf("bg <UNIX cmd> - Spawn child process, execute <UNIX cmd> in the background\n\n");
}

/**
 * @brief Runs a command in the background.
 * 
 * @param cmdAsArray the current command
 * @param bglist all background processes being executed
 */
void bg(char **cmdAsArray, bgjob_t *bglist)
{
    // Index of first empty background process
    int firstIndex;

    // Find the index of the first empty background process
    for (int i = 0; i < MAX_BG; ++i)
    {
        if (bglist[i].pid == INVALID_PID)
        {
            firstIndex = i;
            break;
        }
        else if (i == MAX_BG - 1)
        {
            printf("Maximum background jobs reached.\n");
            return;
        }
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Failed to fork()\n");
        return;
    }
    else if (pid == 0) // child process
    {
        int stdin_fd = open("/dev/null", O_WRONLY);
        if (stdin_fd == -1)
        {
            _exit(127);
        }
        dup2(stdin_fd, STDOUT_FILENO);
        close(stdin_fd);

        execvp(cmdAsArray[1], cmdAsArray + 1);
        fprintf(stderr, "%s: command not found\n", cmdAsArray[1]);
        _exit(127);
    }
    else // parent process
    {
        bglist[firstIndex].pid = pid;
        strncpy(bglist[firstIndex].command, cmdAsArray[1], MAX_CMD);
    }
}

/**
 * @brief Lists all commands running in the background.
 * 
 * @param bglist all background processes being executed
 */
void bgList(bgjob_t *bglist)
{
    for (int i = 0; i < MAX_BG; ++i)
    {
        if (bglist[i].pid == INVALID_PID)
        {
            printf("Total Background Jobs: %i\n", i);
            break;
        }
        printf("%d: %s\n", i, bglist[i].command);
    }
}

/**
 * @brief Kills a specified background command.
 * 
 * @param bgNumber the process number to kill
 * @param bglist all background processes being executed
 */
void bgKill(int bgNumber, bgjob_t *bglist)
{
    kill(bglist[bgNumber].pid, SIGKILL);
}

void bgClean(bgjob_t *bglist)
{
    for (int i = 0; i < MAX_BG; ++i)
    {
        if (waitpid(bglist[i].pid, NULL, WNOHANG) != 0)
        {
            bglist[i].pid = INVALID_PID;
        }
    }
}
