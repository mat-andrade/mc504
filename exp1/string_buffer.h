#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stddef.h>

typedef struct string_buffer {
	char *buf;
	size_t size, cap;
} string_buffer;

void sb_init(string_buffer *sb, size_t initial_cap);
void sb_free(string_buffer *sb);
void sb_append_char(string_buffer *sb, char c);
void sb_append_str(string_buffer *sb, char *str);

#endif
