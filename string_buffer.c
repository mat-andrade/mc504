#include "string_buffer.h"

#include <stdlib.h>
#include <string.h>

void sb_init(string_buffer *sb, size_t initial_cap) {
	sb->buf = malloc(initial_cap);
	sb->cap = initial_cap;
	sb->size = 0;
}

void sb_free(string_buffer *sb) {
	free(sb->buf);
}

void sb_append_char(string_buffer *sb, char c) {
	if (sb->size == sb->cap) {
		sb->buf = realloc(sb->buf, sb->cap * 2);
	}
	sb->buf[sb->size++] = c;
	sb->cap++;
}

void sb_append_str(string_buffer *sb, char *str) {
	while (*str) {
		sb_append_char(sb, str++);
	}
}

