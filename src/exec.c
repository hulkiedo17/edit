#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "buffer.h"
#include "command.h"
#include "exec.h"
#include "main.h"
#include "io.h"

#ifdef DEBUG
#include "misc.h"
#endif

static void append_data(void)
{
	char *line = NULL;
	size_t len = 0;

	while(getline(&line, &len, stdin) != -1)
	{
		if(strcmp(line, ".\n") == 0)
			break;

		append_line(buffer_list, line);
	}

	free(line);
}

static void delete_data(const char *line)
{
	if(!line)
	{
		printf("out of lines\n");
		return;
	}

	size_t line_number = strtoul(line, NULL, 10);
	if(line_number == 0 || line_number == ULONG_MAX)
	{
		printf("out of lines\n");
		return;
	}

	delete_line(buffer_list, line_number);
}

int execute(struct command *cmd)
{
	if(!cmd)
	{
#ifdef DEBUG
		p_warn("warning: execute() - argument is null\n");
#endif
		return FAILED_EXECUTE;
	}

	if(strcmp(cmd->name, "quit") == 0)
		return QUIT_EXECUTE;
	else if(strcmp(cmd->name, "append") == 0)
		append_data();
	else if(strcmp(cmd->name, "delete") == 0)
		delete_data(cmd->parameter1);
	else if(strcmp(cmd->name, "print") == 0)
		print_lines(buffer_list);
	else if(strcmp(cmd->name, "write") == 0)
		write_lines(buffer_list, cmd->parameter1, "w");
	else if(strcmp(cmd->name, "read") == 0)
		read_lines(buffer_list, cmd->parameter1);
	else if(strcmp(cmd->name, "clear") == 0)
		delete_lines(buffer_list);
	else
		printf("unknown command: %s\n", cmd->name);

	return SUCCESS_EXECUTE;
}
