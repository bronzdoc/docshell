#include <stdio.h>
#include <stdlib.h>
#include "dsh.h"

void dsh_loop()
{
    char *line;
    char ** args;
    int status;

    do {
        printf("> ");
        line = dsh_read_line();
        args = dsh_split_line(line);
        status = dsh_execute(args);

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
        if (character == EOF || character == '\n') {
            buffer[position] = '\0';
            return buffer;

        // Fill buffer with user input
        } else
            buffer[position] = character;

        position++;
    }

    // Check if user input overflow's default buffer
    if (position >= bufsize) {
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

char **dsh_split_line(char *line)
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
	    tokens = realloc(tokens, bufsize * sizeof(char*));

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
        if (execvp(args[0], args) == -1)
	    perror("dsh");

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

// List of builint commands
// This is an array of pointers to char (strings)
char *builtins_str[] = {
    "cd",
    "help",
    "exit"
};

// This is an array of pointers to functions accepting char** as argumnet returning an int
int (*builtin_func[]) (char **) = {
    &dsh_cd,
    &dsh_help,
    &dsh_exit
};

// Number of builtins
int dsh_num_builtins()
{
    return sizeof(builtins_str) / sizeof(char *);
}

// Builtin to change directory
int dsh_cd(char **args)
{
    if (args[1] == NULL)
        fprintf(stderr, "dsh: cd is expceting argument to \"cd\"\n");
    else {
        if (chdir(args) != 0)
            perror("dsh");
    }
}

// Print the list of builtins of the shell
int dsh_help()
{
    int i;
    printf("Luis Sagastume DSH: Doc Shell\n");
    printf("Builints available:\n");

    for (i = 0; i < dsh_num_builtins(); i++)
        printf(" %s\n", builtins_str[i]);
}

int dsh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
        // Check if empry command was entered
	return 1;

    for (i = 0; i < dsh_num_builtins(); i++) {
        if (strcmp(args[0], builtins_str[i]) == 0)
	    // Call the builtin function
	    return (*builtin_func[i])(args);
    }
}

void run_shell()
{
  dsh_loop();
}
int dsh_exit()
{
    return 0;
}

