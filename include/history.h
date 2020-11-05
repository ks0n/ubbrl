#ifndef HISTORY_H
#define HISTORY_H

#include <stdbool.h>
#include <stdlib.h>

/* Is the history initialized or not */
extern bool history_initialized;

/* @brief Initialize history module of ubbrl
 * @args filename Filename for the history file. Can be NULL if you don't want to store history in a file
 *
 * @return -1 on error, 0 otherwise
 */
int history_init(const char *filename);

/* @brief Append a new command to the history
 *
 * @args line Line to append in history
 */
int history_append(const char *line);

/**
 * Get the previous element from the history, one up from the last time it was called
 *
 * @return Last line before the last one selected
 */
const char *history_previous(void);

/**
 * Get the next element from the history, one down from the last time it was called
 *
 * @return Last line after the last one selected
 */
const char *history_next(void);

/**
 * Reset the inner index that the history keeps. Call it when reading a new line
 */
void history_reset_idx(void);

/**
 * Search through the history for a prefix
 *
 * @param prefix Input to search for
 * @param len Length of the input
 *
 * @return The closest most recent input that match if one matches, NULL otherwise
 */
const char *history_search(const char *prefix, size_t len);

#endif
