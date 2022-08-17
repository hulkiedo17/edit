#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "buffer.h"
#include "exec.h"
#include "misc.h"

#define PROMPT(str) fprintf(stdout, str);	\
	fflush(stdout)

static void parse_arguments(int argc, char **argv)
{
	int result;

	while((result = getopt(argc, argv, "hvf:")) != -1) {
		switch(result) {
		case 'h':
			help();
			exit(EXIT_SUCCESS);
		case 'v':
			version();
			exit(EXIT_SUCCESS);
		default:
			printf("-%c - unknown option\n", optopt);
			break;
		}
	}
}

static void editor_loop(void)
{
	size_t size = 0;
	char *line = NULL;

	PROMPT("> ");
	while(getline(&line, &size, stdin) != -1)
	{
		int result = execute_command(line);

		if(result == QUIT_EXECUTE)
			break;
		if(result == FAILED_EXECUTE)
			p_warn("command failed\n");

		PROMPT("> ");
	}

	free(line);
}

int main(int argc, char **argv)
{
	parse_arguments(argc, argv);

	init_list();

	editor_loop();

	delete_list();
	return EXIT_SUCCESS;
}
