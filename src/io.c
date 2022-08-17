#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"

#define NEEDED_BUFFER_DEFINITION
#include "buffer.h"

void write_lines(const char *filename)
{
	if(!filename)
	{
#ifdef DEBUG
		p_warn("warning: write_lines() - argument is null\n");
#endif
		return;
	}

	size_t written = 0;
	FILE *fp = NULL;
	struct line *head = buffer->head;

	if(!head)
	{
		printf("buffer is empty\n");
		return;
	}

	fp = fopen(filename, "w");
	if(!fp)
	{
		printf("unable to open file\n");
		return;
	}

	while(head != NULL)
	{
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
	printf("written %zd bytes, %zd lines\n", written, buffer->number_of_lines);
}

void read_lines(const char *filename)
{
	if(!filename)
	{
#ifdef DEBUG
		p_warn("warning: read_lines() - argument is null\n");
#endif
		return;
	}

	size_t line_counter = 0;
	size_t readed = 0;
	size_t len = 0;
	char *line = NULL;
	FILE *fp = NULL;

	fp = fopen(filename, "r");
	if(!fp)
	{
		printf("unable to open file\n");
		return;
	}

	while(getline(&line, &len, fp) != -1)
	{
		append_line(line);

		readed += strlen(line);
		line_counter++;
	}

	free(line);
	fclose(fp);
	printf("readed %zd bytes, %zd lines\n", readed, line_counter);
}
