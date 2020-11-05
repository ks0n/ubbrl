#include "history.h"

#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

static FILE *history_file;
static struct vec *history_lines;
static size_t history_current_idx;

static void history_fini(void)
{
	if (history_file) {
		for (size_t i = 0; i < vec_size(history_lines); i++) {
			fprintf(history_file, "%s\n",
				(char *)vec_get(history_lines, i));
		}
	}

	vec_destroy(history_lines);
}

int history_init(const char *filename)
{
	if (filename) {
		history_file = fopen(filename, "a+");
		if (history_file == NULL)
			return -1;
	}

	history_lines = vec_create(sizeof(char *), free);
	if (!history_lines)
		return -1;

	atexit(history_fini);

	return 0;
}

int history_append(const char *line)
{
	vec_push_back(history_lines, (void *)line);

	history_current_idx = vec_size(history_lines);

	return 0;
}

const char *history_one_up(void)
{
	if (history_current_idx == 0)
		return NULL;

	history_current_idx--;

	return vec_get(history_lines, history_current_idx);
}

const char *history_one_down(void)
{
	if (history_current_idx == vec_size(history_lines))
		return NULL;

	history_current_idx++;

	return vec_get(history_lines, history_current_idx);
}

void history_reset_idx(void)
{
	history_current_idx = vec_size(history_lines);
}
