#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "charstream.h"
#include "wordvec.h"

#include "ubbrl.h"

static struct termios original_terminal_configuration;
static bool atexit_enabled = false;

/**
 * @brief Disable raw mode for the active terminal. Called automagically by `exit()`
 */
static void disable_raw_mode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_configuration);
}

/**
 * Enable raw mode for the active terminal. 1960 magic shit.
 *
 * @return 0 on success, -1 on error
 */
static int enable_raw_mode()
{
	tcgetattr(STDIN_FILENO, &original_terminal_configuration);

	struct termios term_mode = original_terminal_configuration;

    if (!atexit_enabled) {
        atexit(disable_raw_mode);
        atexit_enabled = true;
    }

	///* Handle newlines correctly, Disable Ctrl-S and Ctrl-Q, Disable 8th bit stripping */
	//term_mode.c_iflag &= ~(IXON | ISTRIP);

	///* Disable canonical mode, Disable Ctrl-C and Ctrl-Z since ubbrl uses them, Disable Ctrl-V */
	//term_mode.c_cflag &= ~(ICANON | ISIG | IEXTEN);

	//term_mode.c_cflag |= CS8; /* There are 8 bits in 1 char */
	//term_mode.c_oflag &= ~(OPOST); /* Turn off post transformations */

	term_mode.c_cflag &= ~ICANON;
	term_mode.c_lflag &= ~ISIG;

	int status = tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_mode);

	if (status)
		return status;

	return 0;
}

/**
 * Is the character a terminal escape sequence or not
 *
 * @return true for '\x1B', false otherwise
 */
static inline bool is_escape_seq(char c)
{
	return c == '\x1B';
}

/**
 * Is the character the end of a terminal escape sequence or not
 *
 * @return true for 'm', false otherwise
 */
static inline bool is_escape_seq_end(char c)
{
	return c == 'm';
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

	/* Old position before the escape sequence. -1 means "unset" */
	ssize_t start_escape_pos = -1;

	for (size_t i = 0; str[i]; i++) {
		if (is_escape_seq(str[i])) {
			start_escape_pos = i;
		} else if (is_escape_seq_end(str[i])) {
			start_escape_pos = -1;
		} else if (start_escape_pos == -1) {
			len++;
		}
	}

	/* The escape sequence was unfinished. Do a normal strlen from there */
	if (start_escape_pos != -1) {
		for (size_t i = start_escape_pos; str[i]; i++)
			len++;
	}

	return len;
}

enum special_chars {
	CTRL_C = 3,
};

const char *ubbrl_read(char *prompt)
{
	enable_raw_mode();

	struct wordvec *vector = wordvec_new();
	struct charstream stream;
	charstream_init(&stream, stdin);

	printf("%s", prompt);

	while (true) {
		char c = charstream_read(&stream);

		if (c == CHARSTREAM_EOF)
			break;

		if (c == CTRL_C) {
			wordvec_del(vector);
			return NULL;
		}

		if (c == '\n')
			break;

		wordvec_append(vector, c);
	}

	disable_raw_mode();

	char *ret_line = strdup(wordvec_chars(vector));

	wordvec_del(vector);

	return ret_line;
}
