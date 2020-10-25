#include "history.h"

#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

static FILE *history_file;
static struct vec *history_lines;

static void history_fini(void)
{
	if (history_file) {
		for (size_t i = 0; i < history_lines->size; i++) {
			fprintf(history_file, "%s\n",
				(const char *)history_lines->items[i]);
		}
	}

	fclose(history_file);

	vec_destroy(history_lines);
}

int history_init(const char *filename)
{
	if (filename) {
		history_file = fopen(filename, "a+");
		if (history_file == NULL)
			return -1;
	}

	history_lines = vec_create();

	atexit(history_fini);

	return 0;
}

int history_append(const char *line)
{
	vec_push_back(history_lines, (void *)line);

	return 0;
}
