#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

static const char* const  program_version = "0.1";

void p_err(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

int p_warn(const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	return -1;
}

void help(void)
{
	printf("usage: editor [options]\n\n");

	printf("[cli options]:\n");
	printf("\t-h - prints this help message\n");
	printf("\t-v - shows the program version\n\n");

	printf("[program commands]:\n");
	printf("\tquit   - quit from program\n");
	printf("\tclear  - clean buffer\n");
	printf("\tprint  - print buffer\n");
	printf("\tappend - append data to buffer\n");
	printf("\twrite   [file] - safe buffer to file\n");
	printf("\tread    [file] - read file to buffer\n");
	printf("\tdelete  [line] - delete the specified line\n");
	printf("\tinserta [line] - insert after specified line\n");
	printf("\tinsertb [line] - insert before specified line\n");
}

void version(void)
{
	printf("editor version - v%s\n", program_version);
}

size_t get_number(const char *line)
{
	if(!line)
		return 0;

	size_t number = strtoul(line, NULL, 10);
	if(number == 0 || number == ULONG_MAX)
		return 0;

	return number;
}
