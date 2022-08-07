#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "buffer.h"
#include "misc.h"

static struct line* create_line(const char *line)
{
	if(!line)
	{
#ifdef DEBUG
		p_warn("warning: create_line() - argument is null\n");
#endif
		return NULL;
	}

	struct line *line_node = NULL;

	line_node = malloc(sizeof(struct line));
	if(!line_node)
		p_err("error: malloc() failed\n");

	line_node->next = NULL;
	line_node->line = strdup(line);
	line_node->line_number = 0;
	line_node->line_size = strlen(line);

	return line_node;
}

buffer_t* create_list(void)
{
	buffer_t *lines = NULL;

	lines = malloc(sizeof(buffer_t));
	if(!lines)
		p_err("error: malloc() failed\n");

	lines->head = NULL;
	lines->number_of_lines = 0;

	return lines;
}

void delete_list(buffer_t *list)
{
	if(!list)
	{
#ifdef DEBUG
		p_warn("warning: delete_list() - argument is null\n");
#endif
		return;
	}

	delete_lines(list);

	free(list);
}

void append_line(buffer_t *lines, const char *line)
{
	if(!lines || !line)
	{
#ifdef DEBUG
		p_warn("warning: append_line() - arguments is null\n");
#endif
		return;
	}

	size_t line_counter = 1;
	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	if(!lines->head)
	{
		node->line_number = line_counter;
		lines->head = node;
		lines->number_of_lines++;
		return;
	}

	struct line *head = lines->head;

	while(head->next != NULL)
	{
		head = head->next;
		line_counter++;
	}

	node->line_number = ++line_counter;
	head->next = node;
	lines->number_of_lines++;
	return;
}

void insert_after(buffer_t *lines, const char *line, size_t line_number)
{
	if(!lines || !line)
	{
#ifdef DEBUG
		p_warn("warning: insert_after() - arguments is null\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > lines->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	size_t line_counter = 1;

	if(!lines->head)
	{
		node->line_number = line_counter;
		lines->head = node;
		lines->number_of_lines++;
		return;
	}

	struct line *prev = NULL;
	struct line *head = lines->head;

	while(head != NULL && head->line_number <= line_number)
	{
		prev = head;
		head = head->next;
		line_counter++;
	}

	if(!head)
	{
		node->line_number = ++line_counter;
		prev->next = node;
		lines->number_of_lines++;
		return;
	}

	node->line_number = line_counter;
	prev->next = node;
	node->next = head;
	lines->number_of_lines++;

	// increase others line numbers after inserted line
	while(head != NULL)
	{
		head->line_number++;
		head = head->next;
	}
}

void insert_before(buffer_t *lines, const char *line, size_t line_number)
{
	if(!lines || !line)
	{
#ifdef DEBUG
		p_warn("warning: insert_before() - arguments is null\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > lines->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	size_t line_counter = 1;

	if(!lines->head)
	{
		node->line_number = line_counter;
		lines->head = node;
		lines->number_of_lines++;
		return;
	}

	struct line *prev = NULL;
	struct line *head = lines->head;

	if(line_number == 1)
	{
		node->line_number = line_counter;
		lines->head = node;
		node->next = head;
		goto increase;
	}

	while(head != NULL && head->line_number < line_number)
	{
		prev = head;
		head = head->next;
		line_counter++;
	}

	if(!head)
	{
		node->line_number = ++line_counter;
		prev->next = node;
		lines->number_of_lines++;
		return;
	}

	node->line_number = line_counter;
	prev->next = node;
	node->next = head;
	lines->number_of_lines++;

	// increase others line numbers after inserted line
increase:
	while(head != NULL)
	{
		head->line_number++;
		head = head->next;
	}
}

/*void replace(buffer_t *lines, const char *line, size_t line_number)
{
	if(!lines || !line)
	{
#ifdef DEBUG
		p_warn("warning: replace() - arguments is null\n");
#endif
		return;
	}

	if(!lines->head)
	{
#ifdef DEBUG
		p_warn("warning: replace() - list is empty\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > lines->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *head = lines->head;

	if(head->line_number == line_number)
	{
		free(head->line);
		head->line = strdup(line);
		head->line_size = strlen(line);
		return;
	}

	while(head != NULL && head->line_number != line_number)
	{
		head = head->next;
	}

	if(!head)
	{
		printf("out of lines\n");
		return;
	}

	if(head->line_number == line_number)
	{
		free(head->line);
		head->line = strdup(line);
		head->line_size = strlen(line);
		return;
	}
}
*/

void delete_line(buffer_t *lines, size_t line_number)
{
	if(!lines)
	{
#ifdef DEBUG
		p_warn("warning: delete_line() - argument is null\n");
#endif
		return;
	}

	if(!lines->head)
	{
#ifdef DEBUG
		p_warn("warning: delete_line() - list is empty\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > lines->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *prev = NULL;
	struct line *head = lines->head;

	if(head->line_number == line_number)
	{
		lines->head = head->next;

		free(head->line);
		free(head);
		lines->number_of_lines--;
		goto decrease_all;
	}

	while(head != NULL && head->line_number != line_number)
	{
		prev = head;
		head = head->next;
	}

	if(!head)
	{
		printf("out of lines\n");
		return;
	}

	if(head->line_number == line_number)
	{
		prev->next = head->next;

		free(head->line);
		free(head);
		lines->number_of_lines--;
		goto decrease_last;
	}

decrease_all:
	head = lines->head;
	if(!head)
		return;

	while(head != NULL)
	{
		head->line_number--;
		head = head->next;
	}
	return;

decrease_last:
	head = prev->next;
	if(!head)
		return;
	
	while(head != NULL)
	{
		head->line_number--;
		head = head->next;
	}
	return;
}

void delete_lines(buffer_t *lines)
{
	if(!lines)
	{
#ifdef DEBUG
		p_warn("warning: delete_lines() - argument is null\n");
#endif
		return;
	}

	if(!lines->head)
	{
		lines->number_of_lines = 0;
		return;
	}

	struct line *temp = NULL;
	struct line *head = lines->head;

	while(head != NULL)
	{
		temp = head;
		head = head->next;
		lines->head = head;

		free(temp->line);
		free(temp);
	}

	lines->head = NULL;
	lines->number_of_lines = 0;
	return;
}

void print_lines(buffer_t *lines)
{
	if(!lines)
	{
		printf("buffer is empty\n");
		return;
	}

	if(!lines->head)
	{
		printf("buffer is empty\n");
		return;
	}

	struct line *head = lines->head;

	while(head != NULL)
	{
#ifdef DEBUG
		printf("(%zd, %zd) %s", head->line_number, head->line_size, head->line);
#else
		printf("%s", head->line);
#endif
		head = head->next;
	}

	return;
}
