#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "misc.h"

void write_lines(buffer_t *lines, const char *filename, const char *mode)
{
	if(!lines || !filename || !mode)
	{
#ifdef DEBUG
		p_warn("warning: write_lines() - arguments is null\n");
#endif
		return;
	}

	FILE *fp = NULL;
	struct line *head = lines->head;
	size_t written = 0;

	if(!head)
	{
		printf("list is empty\n");
		return;
	}

	fp = fopen(filename, mode);
	if(!fp)
	{
		printf("unable to open file\n");
		return;
	}

	while(head != NULL)
	{
		// TODO: make normal error check for i/o
		// temporary solution for now:
		if(fwrite(head->line, head->line_size, 1, fp) != 1)
		{
			p_warn("warning: fwrite() failed\n");
			fclose(fp);
			return;
		}
		written += head->line_size;
		head = head->next;
	}

	fclose(fp);
	printf("written %zd bytes, %zd lines\n", written, lines->number_of_lines);
}

void read_lines(buffer_t *lines, const char *filename)
{
	if(!lines || !filename)
	{
#ifdef DEBUG
		p_warn("warning: read_lines() - arguments is null\n");
#endif
		return;
	}

	FILE *fp = NULL;
	char *line = NULL;
	size_t size = 0;
	size_t line_counter = 0;
	size_t readed = 0;

	fp = fopen(filename, "r");
	if(!fp)
	{
		printf("unable to open file\n");
		return;
	}

	while(getline(&line, &size, fp) != -1)
	{
		append_line(lines, line);

		readed += strlen(line);
		line_counter++;
	}

	free(line);
	fclose(fp);
	printf("readed %zd bytes, %zd lines\n", readed, line_counter);
}
