#ifndef __DSH_HEADER__H_
#define __DSH_HEADER__H_

#define DSH_DF_BUFSIZE 1024

#define DSH_TOK_BUFSIZE 64

#define DSH_TOK_DELIM " \t\r\n\a"

void dsh_loop();
char *dsh_read_line();
char **dsh_split_line(char *line);
int exec_command(char **args);

int dsh_cd(char **args);
int dsh_help();
int dsh_exit();

#endif


