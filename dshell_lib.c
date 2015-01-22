#include <stdio.h>
#include <stdlib.h>
#include "lib/dsh.h"

void dsh_loop()
{
    char *line;
    char ** args;
    int status;

    do {
        prinf("> ");
        line = dsh_read_line();
        args = dsh_split_line(line);
        status dsh_exec(args);

        free(line);
        free(args);

    } while(status);
}

char *dsh_read_line()
{
    // Default size of buffer
    int bufsize = DSH_DF_BUFSIZE;
    int position;
    char *buffer = malloc(sizeof(char) * bufsize);
    int character;

    while(1) {
        // Read character
        character = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (character == EOF || character = '\n') {
            buffert[position] = '\0';
            return buffer;

        // Fill buffer with user input
        } else
            buffer[position] = character;

        position++;
    }

    // Check if user input overflow's default buffer
    if (positon >= bufsize) {
        bufsize += DSH_DF_BUFSIZE;

        //reallocate a bigger buffer in memory
        buffer = realloc(buffer, bufsize);

        // Check if buffer couldn't reallocate and die with honor
        if (!buffer) {
            fprintf(stderr, "dsh: allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
}

char **dsh_split_line(*char line)
{
    int bufsize = DSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    // Check if memory was allocated correctly
    if (!tokens) {
        fprintf(stderr, "dsh: allocation error\n");
	exit(EXIT_FAILURE);
    }

    token = strtok(line, DSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        // Check if line overflow's default buffer
	if (position >= bufsize) {
	    bufsize += DSH_TOK_BUFSIZE;
	    tokens = realloc(tokens, bufsize, sizeof(char*));

	    // Check for memory reallocation error
	    if (!tokens) {
                fprintf(stderr, "dsh: allocation error\n");
		exit(EXIT_FAILURE);
	    }
	}

	// Get next token
        token = strtok(NULL, DSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int dsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
	    perror("dsh");
	}

	exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("dsh");
    } else {
        // Parent process
        do {
	    wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int exec_command(char **args)

