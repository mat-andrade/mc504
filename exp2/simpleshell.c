/**
 * simple-shell
 * Exp2
 * Turma MC504A
 * Alunos:
 * Mateus da Costa e Silva Rios Alves de Andrade - 230806
 * Matheus Farias Barbosa - 230888
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>

struct command {
	char *program;
	char **argv;
	int argc;
	bool background;
};



struct command parse_command(char *str) {
	char *program = strtok(str, " ");
	int cap = 8, argc = 1;
	char **argv = malloc(cap * sizeof(char *));
	argv[0] = program;
	for (str = strtok(NULL, " "); str != NULL; str = strtok(NULL, " ")) {	
		if (argc == cap) {
			cap *= 2;
			argv = realloc(argv, cap * sizeof(char *));
		}
		argv[argc++] = str;
	}
	bool background;
	if (strcmp(argv[argc - 1], "&") == 0) {
		argc--;
		puts("1");
		background = true;
	} else if (argc == cap){
		argv = realloc(argv, (cap + 1) * sizeof(char *));
		puts("2");
		background = false;
	}
	argv[argc] = NULL;
	return (struct command) {
		program,
		argv,
		argc,
		background
	};
}

bool search_path(char *path, struct command com, char *out) {
	for (path = strtok(path, ":"); path != NULL; path = strtok(NULL, ":")) {
		strncpy(out, path, 255);
		int l = strlen(out);
		out[l] = '/';
		out[l + 1] = '\0';
		strncat(out, com.program, 255);
		if (access(out, F_OK) == 0) {
			return true;
		}
	}
	return false;
}


int main(int argc, char **argv) {
	if (argc == 1) {
		fprintf(stderr, "No path received\n");
		exit(1);
	}

	for (;;) {
		char buf[256];
		do {
			printf("simpleshell$ ");
			char *p = fgets(buf, 256, stdin);
			if (feof(stdin)) {
				printf("\n");
				exit(0);
			}
			if (!p) {
				perror("Error on reading input");
				exit(1);
			}
		} while(buf[0] == '\n' || buf[0] == '\0');
		
		buf[255] = '\0';
		for (char *ptr = buf; *ptr; ptr++) {
			if (*ptr == '\n') {
				*ptr = '\0';
				break;
			}
		}
		struct command com = parse_command(buf);
		if (strcmp(com.program, "exit") == 0) {
			exit(0);
		}

		char p[512];
		if (search_path(argv[1], com, p)) {
			int pid = fork();
			if (pid < 0 ) {
				perror("Could not start process");
			} else if (pid > 0) {
				int status;
				if (!com.background) {
					puts("olá");
					waitpid(-1, &status, 0);
				}
			} else {
				com.argv[0] = p;
				execv(p, com.argv);
			}
		} else {
			fprintf(stderr, "Program '%s' not found\n", com.program);
		}
		free(com.argv);
	}
}
