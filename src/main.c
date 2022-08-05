#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "command.h"
#include "buffer.h"
#include "exec.h"
#include "misc.h"

#define PROMPT(str) fprintf(stdout, str);	\
	fflush(stdout)

buffer_t *buffer_list = NULL;

void parse_arguments(int argc, char **argv)
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

void editor_loop(void)
{
	size_t size = 0;
	char *line = NULL;
	struct command *cmd = NULL;

	PROMPT("> ");

	while(getline(&line, &size, stdin) != -1)
	{
		cmd = get_command(line);
		if(!cmd)
		{
			p_warn("parsing command failed");
		}
		else
		{
			int result = execute(cmd);

			if(result == QUIT_EXECUTE)
				break;
			else if(result == FAILED_EXECUTE)
				p_warn("execution failed\n");
		}

		delete_command(cmd);
		PROMPT("> ");
	}

	free(line);
	delete_command(cmd);
}

int main(int argc, char **argv)
{
	parse_arguments(argc, argv);

	buffer_list = create_list();

	editor_loop();

	delete_list(buffer_list);
	return EXIT_SUCCESS;
}
