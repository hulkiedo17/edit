#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

static const char* const  program_version = "0.3";

void p_err(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

void p_warn(const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

void help(void)
{
	printf("usage: edit [options]\n\n");

	printf("[cli options]:\n");
	printf("\t-h - prints this help message\n");
	printf("\t-v - shows the program version\n\n");

	printf("[program commands]:\n");
	printf("\tq - quit from program\n");
	printf("\th - prints short help message\n");
	printf("\tc - clean buffer\n");
	printf("\tp - print buffer\n");
	printf("\ta - append data to buffer\n");
	printf("\tw  [file] - safe buffer to file\n");
	printf("\tr  [file] - read file to buffer\n");
	printf("\td  [line] - delete the specified line\n");
	printf("\tia [line] - insert after specified line\n");
	printf("\tib [line] - insert before specified line\n");
	printf("\ts  [line] [line] - swaps two lines\n");
}

void version(void)
{
	printf("edit version - v%s\n", program_version);
}

size_t get_number(const char *line)
{
	if(!line)
	{
#ifdef DEBUG
		p_warn("warning: get_number() - arguments is null\n");
#endif
		return 0;
	}

	size_t number = strtoul(line, NULL, 10);

	if(number == 0 || number == ULONG_MAX)
	{
#ifdef DEBUG
		p_warn("warning: get_number() - strtoul() failed\n");
#endif
		return 0;
	}

	return number;
}
