/**
 * simple-shell
 * Exp1
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

struct command {
	char *program;
	char **argv;
	int argc;
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
	if (argc == cap) {
		argv = realloc(argv, (cap + 1) * sizeof(char *));
		argv[argc++] = NULL;
	}
	return (struct command) {program, argv, argc};
}



int main(int argc, char **argv) {
	if (argc == 1) {
		fprintf(stderr, "No path received\n");
		exit(1);
	}

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

	char *paths = argv[1];

	for (char *path = strtok(paths, ":"); path != NULL; path = strtok(NULL, ":")) {
		char p[512];
		strncpy(p, path, 255);
		int l = strlen(p);
		p[l] = '/';
		p[l + 1] = '\0';
		strncat(p, com.program, 255);
		p[511] = '\0';
		if (access(p, F_OK) == 0) {
			execv(p, com.argv);
		}
	}
	fprintf(stderr, "Program '%s' not found\n", com.program);
}
