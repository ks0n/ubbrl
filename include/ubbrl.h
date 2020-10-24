#ifndef UBBRL_H
#define UBBRL_H

#include <stdlib.h>

/**
 * @file ubbrl.h
 *
 * Provides a small interface for reading from the terminal and handling emacs-like
 * commands, as well as keeping an history of inputs.
 */

/**
 * @brief Read a line from stdin, allocating memory for it and NULL-terminating it.
 *
 * @warn The return value MUST be freed by the caller
 *
 * @param prompt Prompt to display at the beginning of the line
 *
 * @return NULL on error, the input line otherwise
 */
const char *ubbrl_read(char *prompt);

#endif /* ! UBBRL_H */
