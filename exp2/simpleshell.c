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

struct path {
	char **p;
	size_t size;
};

struct path parse_path(char *path) {
	size_t cap = 8, size = 0;
	char **ret = malloc(cap * sizeof(char *));
	for (char *p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":")) {
		if (size == cap) {
			cap *= 2;
			ret = realloc(ret, cap * sizeof(char *));
		}
		ret[size++] = p;
	}
	return (struct path){ret, size};
}

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
	bool background = false;
	if (strcmp(argv[argc - 1], "&") == 0) {
		argc--;
		background = true;
	} else if (argc == cap){
		argv = realloc(argv, (cap + 1) * sizeof(char *));
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

bool search_path(struct path path, struct command com, char *out) {
	for (int i = 0; i < path.size; i++) {
		char *p = path.p[i];
		strncpy(out, p, 255);
		int l = strlen(out);
		out[l] = '/';
		out[l + 1] = '\0';
		strncat(out, com.program, 255);
		out[511] = '\0';
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

	struct path path = parse_path(argv[1]);

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
		if (search_path(path, com, p)) {
			int pid = fork();
			if (pid < 0 ) {
				perror("Could not start process");
			} else if (pid > 0) {
				int status = 0;
				if (!com.background) {
					waitpid(pid, &status, 0);
				}
			} else {
				com.argv[0] = p;
				execv(p, com.argv);
			}
		} else {
			fprintf(stderr, "Program '%s' not found\n", com.program);
		}
		free(com.argv);
		waitpid(-1, NULL, WNOHANG);
	}
}
