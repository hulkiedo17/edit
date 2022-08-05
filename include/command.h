#ifndef COMMAND_H
#define COMMAND_H

struct command
{
	char *name;
	char *parameter1;
	char *parameter2;
};

struct command* get_command(const char *line);
void delete_command(struct command *cmd);

#endif
