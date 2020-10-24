#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "ubbrl.h"

static struct termios original_terminal_configuration;
static bool raw_mode_enabled = false;

/**
 * @brief Disable raw mode for the active terminal. Called automagically by `exit()`
 */
static void disable_raw_mode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_configuration);
}

/**
 * @brief Enable raw mode for the active terminal
 *
 * @return 0 on success, -1 on error
 */
static int enable_raw_mode()
{
	tcgetattr(STDIN_FILENO, &original_terminal_configuration);

	/* Register `disable_raw_mode` to be called upon exit */
	atexit(disable_raw_mode);

	struct termios term_mode = original_terminal_configuration;

	/* Handle newlines correctly, Disable Ctrl-S and Ctrl-Q, Disable 8th bit stripping */
	term_mode.c_iflag &= ~(ICRNL | IXON | ISTRIP);

	/* Disable canonical mode, Disable Ctrl-C and Ctrl-Z since ubsh uses them, Disable Ctrl-V */
	term_mode.c_cflag &= ~(ICANON | ISIG | IEXTEN);

	term_mode.c_cflag |= CS8; /* There are 8 bits in 1 char */
	term_mode.c_oflag &= ~(OPOST); /* Turn off post transformations */

	int status = tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_mode);

	if (status)
		return status;

	raw_mode_enabled = true;

	return 0;
}

/**
 * @brief Compute the length of a string taking escape sequences into account
 *
 * @param str NULL terminated string to compute the length of
 *
 * @return -1 on error, the size of the string otherwise
 */
ssize_t term_strlen(const char *str)
{
	size_t len = 0;
	while (*str++)
		len++;

	return len;
}

char *ubbrl_read(char *prompt)
{
	if (!raw_mode_enabled)
		enable_raw_mode();

	size_t prompt_len = term_strlen(prompt);

	if (fwrite(prompt, sizeof(*prompt), prompt_len, stdin) != prompt_len)
		return NULL;

	return NULL;
}
