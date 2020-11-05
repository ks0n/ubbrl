#ifndef HISTORY_H
#define HISTORY_H

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
 * Get an element from the history, one up from the last time it was called
 *
 * @return Last line before the last one selected
 */
const char *history_one_up(void);

/**
 * Get an element from the history, one down from the last time it was called
 *
 * @return Last line before the last one selected
 */
const char *history_one_down(void);

/**
 * Reset the inner index that the history keeps. Call it when reading a new line
 */
void history_reset_idx(void);

#endif
