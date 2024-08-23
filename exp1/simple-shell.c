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
	int cap = 8, argc = 0;
	char **argv = NULL;
	for (str = strtok(NULL, " "); str != NULL; str = strtok(NULL, " ")) {
		if (argc == 0) {
			argv = malloc(cap * sizeof(char *));
		}		
		if (argc == cap) {
			cap *= 2;
			argv = realloc(argv, cap * sizeof(char *));
		}
		argv[argc++] = str;
	}
	return (struct command) {program, argv, argc};
}



int main(int argc, char **argv) {
	if (argc == 1) {
		perror("No path received");
		exit(1);
	}
	printf("simpleshell$ ");

	char buf[256];
	fgets(buf, 256, stdin);
	buf[255] = '\0';

	char *paths = argv[1];
	for (char *p = strtok(paths, ":"); p != NULL; p = strtok(NULL, ":")) {
		DIR *d = opendir(p);
		for (struct dirent *de = readdir(d); de != NULL; de = readdir(d)) {
			
		}
		readdir(d);
	}

	int pid = fork();
	if (pid == -1) {
		perror("no fork");
		exit(1);
	} else if (pid == 0) {

		execv("", NULL);
	} else {
		int status;
		waitpid(-1, &status, 0);
	}
}