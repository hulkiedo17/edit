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
	printf("q - quit from program\n");
	printf("h - prints this help message\n");
	printf("c - clean buffer\n");
	printf("p - print buffer\n");
	printf("a - append data to buffer\n");
	printf("w  [file] - safe buffer to file\n");
	printf("r  [file] - read file to buffer\n");
	printf("d  [line] - delete the specified line\n");
	printf("ia [line] - insert after specified line\n");
	printf("ib [line] - insert before specified line\n");
	printf("s  [line] [line] - swaps two lines\n");
}

static int execute(struct command *cmd)
{
	if(cmd == NULL)
		return SUCCESS_EXECUTE;
	else if(strcmp(cmd->name, "q") == 0)
		return QUIT_EXECUTE;
	else if(strcmp(cmd->name, "a") == 0)
		append_data();
	else if(strcmp(cmd->name, "d") == 0)
		delete_data(cmd->parameter1);
	else if(strcmp(cmd->name, "ia") == 0)
		inserta_data(cmd->parameter1);
	else if(strcmp(cmd->name, "ib") == 0)
		insertb_data(cmd->parameter1);
	else if(strcmp(cmd->name, "s") == 0)
		swap(cmd->parameter1, cmd->parameter2);
	else if(strcmp(cmd->name, "p") == 0)
		print_lines();
	else if(strcmp(cmd->name, "w") == 0)
		write_lines(cmd->parameter1);
	else if(strcmp(cmd->name, "r") == 0)
		read_lines(cmd->parameter1);
	else if(strcmp(cmd->name, "c") == 0)
		delete_lines();
	else if(strcmp(cmd->name, "h") == 0)
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
