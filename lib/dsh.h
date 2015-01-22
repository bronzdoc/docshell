#ifndef
#define DSH_DF_BUFSIZE 1024
#endif

#ifndef
#define DSH_TOK_BUFSIZE 64
#endif

#ifndef
#define DSH_TOK_DELIM " \t\r\n\a"
#endif


void dsh_loop();
char *dsh_read_line();
char **dsh_split_line(*char line);
int exec_command(char **args)

