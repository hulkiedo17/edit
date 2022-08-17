#ifndef MISC_H
#define MISC_H

void p_err(const char* fmt, ...);
void p_warn(const char *fmt, ...);
void help(void);
void version(void);
size_t get_number(const char *line);

#endif
