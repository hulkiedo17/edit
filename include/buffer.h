#ifndef BUFFER_H
#define BUFFER_H

struct line
{
	size_t line_number;
	size_t line_size;
	char *line;
	struct line *next;
};

typedef struct buffer
{
	size_t number_of_lines;
	struct line *head;
} buffer_t;

buffer_t* create_list(void);
void delete_list(buffer_t *list);
void append_line(buffer_t *lines, const char *line);
//void insert_after(buffer_t *lines, const char *line, size_t line_number);
//void insert_before(buffer_t *lines, const char *line, size_t line_number);
//void replace(buffer_t *lines, const char *line, size_t line_number);
void delete_line(buffer_t *lines, size_t line_number);
void delete_lines(buffer_t *lines);
void print_lines(buffer_t *lines);

#endif
