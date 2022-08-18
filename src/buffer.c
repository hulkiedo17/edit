#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "misc.h"

#define NEEDED_BUFFER_DEFINITION
#include "buffer.h"

buffer_t *buffer = NULL;

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

	return line_node;
}

void init_list(void)
{
	buffer = malloc(sizeof(buffer_t));
	if(!buffer)
		p_err("error: malloc() failed\n");

	buffer->head = NULL;
	buffer->number_of_lines = 0;
}

void delete_list(void)
{
	delete_lines();

	free(buffer);
	buffer = NULL;
}

void append_line(const char *line)
{
	if(!buffer || !line)
	{
#ifdef DEBUG
		p_warn("warning: append_line() - arguments or buffer is null\n");
#endif
		return;
	}

	size_t line_counter = 1;
	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	if(!buffer->head)
	{
		node->line_number = line_counter;
		buffer->head = node;
		buffer->number_of_lines++;
		return;
	}

	struct line *head = buffer->head;

	while(head->next != NULL)
	{
		head = head->next;
		line_counter++;
	}

	node->line_number = ++line_counter;
	head->next = node;
	buffer->number_of_lines++;
}

void insert_after(const char *line, size_t line_number)
{
	if(!buffer || !line)
	{
#ifdef DEBUG
		p_warn("warning: insert_after() - arguments or buffer is null\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	size_t line_counter = 1;

	if(!buffer->head)
	{
		node->line_number = line_counter;
		buffer->head = node;
		buffer->number_of_lines++;
		return;
	}

	struct line *prev = NULL;
	struct line *head = buffer->head;

	while(head != NULL && head->line_number <= line_number)
	{
		prev = head;
		head = head->next;
		line_counter++;
	}

	if(!head)
	{
		node->line_number = line_counter;
		prev->next = node;
		buffer->number_of_lines++;
		return;
	}

	node->line_number = line_counter;
	prev->next = node;
	node->next = head;
	buffer->number_of_lines++;

	// increase others line numbers after inserted line
	while(head != NULL)
	{
		head->line_number++;
		head = head->next;
	}
}

void insert_before(const char *line, size_t line_number)
{
	if(!buffer || !line)
	{
#ifdef DEBUG
		p_warn("warning: insert_before() - arguments or buffer is null\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *node = NULL;

	node = create_line(line);
	if(!node)
		return;

	size_t line_counter = 1;

	if(!buffer->head)
	{
		node->line_number = line_counter;
		buffer->head = node;
		buffer->number_of_lines++;
		return;
	}

	struct line *prev = NULL;
	struct line *head = buffer->head;

	if(line_number == 1)
	{
		node->line_number = line_counter;
		buffer->head = node;
		buffer->number_of_lines++;
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
		buffer->number_of_lines++;
		return;
	}

	node->line_number = line_counter;
	prev->next = node;
	node->next = head;
	buffer->number_of_lines++;

	// increase others line numbers after inserted line
increase:
	while(head != NULL)
	{
		head->line_number++;
		head = head->next;
	}
}

void delete_line(size_t line_number)
{
	if(!buffer || !buffer->head)
	{
#ifdef DEBUG
		p_warn("warning: delete_line() - buffer is empty\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *prev = NULL;
	struct line *head = buffer->head;

	if(head->line_number == line_number)
	{
		buffer->head = head->next;

		free(head->line);
		free(head);
		buffer->number_of_lines--;
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
		buffer->number_of_lines--;
		goto decrease_last;
	}

decrease_all:
	head = buffer->head;
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
}

void delete_lines(void)
{
	if(!buffer)
	{
#ifdef DEBUG
		p_warn("warning: delete_lines() - buffer is null\n");
#endif
		return;
	}

	if(!buffer->head)
	{
		buffer->number_of_lines = 0;
		return;
	}

	struct line *temp = NULL;
	struct line *head = buffer->head;

	while(head != NULL)
	{
		temp = head;
		head = head->next;
		buffer->head = head;

		free(temp->line);
		free(temp);
	}

	buffer->head = NULL;
	buffer->number_of_lines = 0;
}

void print_lines(void)
{
	if(!buffer || !buffer->head)
		return;

	struct line *head = buffer->head;

	while(head != NULL)
	{
#ifdef DEBUG
		printf("(%4zd) %s", head->line_number, head->line);
#else
		printf("%s", head->line);
#endif
		head = head->next;
	}
}

void swap(char* line_num1, char* line_num2)
{
	if(!line_num1 || !line_num2)
	{
#ifdef DEBUG
		p_warn("warning: swap() - arguments is null\n");
#endif
		return;
	}

	size_t line_number1, line_number2;

	line_number1 = get_number(line_num1);
	line_number2 = get_number(line_num2);

	if(line_number1 >= line_number2)
	{
		printf("out of lines\n");
		return;
	}

	if(!buffer || !buffer->head)
	{
#ifdef DEBUG
		p_warn("warning: swap() - buffer is empty\n");
#endif
		return;
	}

	if(line_number1 < 1 || line_number1 > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	if(line_number2 < 1 || line_number2 > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *temp1 = NULL;
	struct line *temp2 = NULL;
	struct line *head = NULL;

	head = buffer->head;
	while(head != NULL)
	{
		if(head->line_number == line_number1)
		{
			temp1 = head;
		}
		if(head->line_number == line_number2)
		{
			temp2 = head;
			break;
		}
		head = head->next;
	}

	char *sline1 = NULL;
	char *sline2 = NULL;

	sline1 = strdup(temp1->line);
	if(!sline1)
		p_err("err: strdup() failed\n");

	sline2 = strdup(temp2->line);
	if(!sline2)
		p_err("err: strdup() failed\n");

	free(temp1->line);
	free(temp2->line);

	temp1->line = sline2;
	temp2->line = sline1;
}

// replace() for future.
/*void replace(const char *line, size_t line_number)
{
	if(!line)
	{
#ifdef DEBUG
		p_warn("warning: replace() - argument is null\n");
#endif
		return;
	}

	if(!buffer || !buffer->head)
	{
#ifdef DEBUG
		p_warn("warning: replace() - buffer is empty\n");
#endif
		return;
	}

	if(line_number < 1 || line_number > buffer->number_of_lines)
	{
		printf("out of lines\n");
		return;
	}

	struct line *head = buffer->head;

	if(head->line_number == line_number)
	{
		free(head->line);
		head->line = strdup(line);
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
		return;
	}
}*/

