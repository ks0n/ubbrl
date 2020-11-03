#ifndef UBBRL_H
#define UBBRL_H

#include <stdlib.h>

/**
 * @file ubbrl.h
 *
 * Provides a small interface for reading from the terminal and handling emacs-like
 * commands, as well as keeping an history of inputs.
 */

enum ubbrl_state {
	UBBRL_CTRL_C = -2,
	UBBRL_CTRL_D = -1,
	UBBRL_OK = 0,
};

/**
 * @brief Read a line from stdin, allocating memory for it and NULL-terminating it.
 *
 * @warn The return value MUST be freed by the caller
 *
 * @param prompt Prompt to display at the beginning of the line
 * @param status Status after the read
 *
 * @return NULL on error, the input line otherwise
 */
char *ubbrl_read(char *prompt, int *status);

#endif /* ! UBBRL_H */
