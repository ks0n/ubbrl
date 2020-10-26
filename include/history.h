#ifndef HISTORY_H
#define HISTORY_H

/* @brief Initialize history module of ubbrl
 * @args filename Filename for the history file. Can be NULL if you don't want to store history in a file
 *
 * @return -1 on error, 0 otherwise
 */
int history_init(const char *filename);

/* @brief Append a new command to the history
 * @args line Line to append in history */
int history_append(const char *line);

#endif
