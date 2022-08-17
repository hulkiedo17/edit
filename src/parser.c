#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "misc.h"

static struct command* create_cmd(void)
{
	struct command *cmd = NULL;

	cmd = malloc(sizeof(struct command));
	if(!cmd)
		return NULL;
	
	cmd->name = NULL;
	cmd->parameter1 = NULL;
	cmd->parameter2 = NULL;

	return cmd;
}

static void delete_cmd(struct command *cmd)
{
	if(!cmd)
		return;

	free(cmd->name);
	free(cmd->parameter1);
	free(cmd->parameter2);
	free(cmd);
}

struct command* parse_command(const char *line)
{
	if(!line)
	{
#ifdef DEBUG
		p_warn("warning: get_command() - argument is null\n");
#endif
		return NULL;
	}
	
	char *sline = NULL;
	char *tok = NULL;
	struct command *cmd = NULL;

	if(strcmp(line, "\n") == 0)
	{
		fprintf(stdout, "empty line\n");
		return NULL;
	}

	sline = strdup(line);
	if(!sline)
		p_err("error: strdup() failed\n");

	cmd = create_cmd();
	if(!cmd)
	{
		free(sline);
		p_err("error: malloc() failed\n");
	}
	
	// get name
	tok = strtok(sline, " \t\n");
	if(tok)
		cmd->name = strdup(tok);
	else
	{
		delete_cmd(cmd);
		free(sline);
		return NULL;
	}

	// get parameter1
	tok = strtok(NULL, " \t\n");
	if(tok)
		cmd->parameter1 = strdup(tok);
	else
		cmd->parameter1 = NULL;

	// get parameter2
	tok = strtok(NULL, " \t\n");
	if(tok)
		cmd->parameter2 = strdup(tok);
	else
		cmd->parameter2 = NULL;

	free(sline);
	return cmd;
}

void free_command(struct command *cmd)
{
	delete_cmd(cmd);
}
