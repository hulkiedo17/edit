#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "parser.h"
#include "io.h"
#include "exec.h"
#include "misc.h"

static void append_data(void)
{
	char *line = NULL;
	size_t len = 0;

	while(getline(&line, &len, stdin) != -1)
	{
		if(strcmp(line, ".\n") == 0)
			break;

		append_line(line);
	}

	free(line);
}

static void inserta_data(const char *number)
{
	if(!number)
	{
		printf("out of lines\n");
		return;
	}

	size_t line_number = get_number(number);
	if(line_number == 0)
	{
		printf("out of lines\n");
		return;
	}

	char *line = NULL;
	size_t len = 0;

	if(getline(&line, &len, stdin) == -1)
	{
		printf("invalid input\n");
		return;
	}

	insert_after(line, line_number);

	free(line);
}

static void insertb_data(const char *number)
{
	if(!number)
	{
		printf("out of lines\n");
		return;
	}

	size_t line_number = get_number(number);
	if(line_number == 0)
	{
		printf("out of lines\n");
		return;
	}

	char *line = NULL;
	size_t len = 0;

	if(getline(&line, &len, stdin) == -1)
	{
		printf("invalid input\n");
		return;
	}

	insert_before(line, line_number);

	free(line);
}

static void delete_data(const char *line)
{
	if(!line)
	{
		printf("out of lines\n");
		return;
	}

	size_t line_number = get_number(line);
	if(line_number == 0)
	{
		printf("out of lines\n");
		return;
	}

	delete_line(line_number);
}

static void builtin_help(void)
{
	printf("quit   - quit from program\n");
	printf("help   - prints this help message\n");
	printf("clear  - clean buffer\n");
	printf("print  - print buffer\n");
	printf("append - append data to buffer\n");
	printf("write   [file] - safe buffer to file\n");
	printf("read    [file] - read file to buffer\n");
	printf("delete  [line] - delete the specified line\n");
	printf("inserta [line] - insert after specified line\n");
	printf("insertb [line] - insert before specified line\n");
}

static int execute(struct command *cmd)
{
	if(cmd == NULL)
		return SUCCESS_EXECUTE;
	else if(strcmp(cmd->name, "quit") == 0)
		return QUIT_EXECUTE;
	else if(strcmp(cmd->name, "append") == 0)
		append_data();
	else if(strcmp(cmd->name, "delete") == 0)
		delete_data(cmd->parameter1);
	else if(strcmp(cmd->name, "inserta") == 0)
		inserta_data(cmd->parameter1);
	else if(strcmp(cmd->name, "insertb") == 0)
		insertb_data(cmd->parameter1);
	else if(strcmp(cmd->name, "print") == 0)
		print_lines();
	else if(strcmp(cmd->name, "write") == 0)
		write_lines(cmd->parameter1);
	else if(strcmp(cmd->name, "read") == 0)
		read_lines(cmd->parameter1);
	else if(strcmp(cmd->name, "clear") == 0)
		delete_lines();
	else if(strcmp(cmd->name, "help") == 0)
		builtin_help();
	else
		printf("unknown command: %s\n", cmd->name);

	return SUCCESS_EXECUTE;
}

int execute_command(const char *line)
{
	int result = FAILED_EXECUTE;
	struct command *cmd = NULL;

	cmd = parse_command(line);

	result = execute(cmd);

	free_command(cmd);

	return result;
}
