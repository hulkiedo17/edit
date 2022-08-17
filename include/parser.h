#ifndef PARSER_H
#define PARSER_H

struct command
{
	char *name;
	char *parameter1;
	char *parameter2;
};

struct command* parse_command(const char *line);
void free_command(struct command *cmd);

#endif
