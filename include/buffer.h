#ifndef BUFFER_H
#define BUFFER_H

#ifdef NEEDED_BUFFER_DEFINITION
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

extern buffer_t *buffer;
#endif

void init_list(void);
void delete_list(void);
void append_line(const char *line);
void insert_after(const char *line, size_t line_number);
void insert_before(const char *line, size_t line_number);
void delete_line(size_t line_number);
void delete_lines(void);
void print_lines(void);

//void replace(buffer_t *lines, const char *line, size_t line_number);

#endif
