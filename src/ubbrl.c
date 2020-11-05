#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "charstream.h"
#include "history.h"
#include "vec.h"
#include "wordvec.h"

#include "ubbrl.h"

static struct termios original_terminal_configuration;
static bool atexit_enabled = false;
static bool history_initialized;

#define ERASE_RIGHT "\x1B[0K" /* Erase to the right on the terminal */

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

	/* Disable break, carriage returns, newlines parity checks. No strip chars,
     * no start/stop output control */
	term_mode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

	/* Disable post processing */
	term_mode.c_oflag &= ~(OPOST);

	/* Each char is 8bit wide */
	term_mode.c_cflag |= (CS8);

	/* Disable canonical mode, echoing, extended functions and signal characters */
	term_mode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	/* Set return conditions thresholds */
	term_mode.c_cc[VMIN] = 1; /* 1 byte threshold */
	term_mode.c_cc[VTIME] = 0; /* no timer */

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
	CTRL_A = 1,
	CTRL_B = 2,
	CTRL_C = 3,
	CTRL_D = 4,
	CTRL_E = 5,
	CTRL_F = 6,
	CTRL_H = 8,
	TAB = 9,
	CTRL_K = 11,
	CTRL_L = 12,
	ENTER = 13,
	CTRL_N = 14,
	CTRL_P = 16,
	CTRL_T = 20,
	CTRL_U = 21,
	CTRL_W = 23,
	ESC = 27,
	BACKSPACE = 127
};

static void flush_line(void)
{
	write(STDIN_FILENO, "\r\n", 2);
}

static void reset_line(char *prompt, struct wordvec *vector)
{
	printf("\r%s%s" ERASE_RIGHT, prompt, wordvec_chars(vector));
}

char *ubbrl_read(char *prompt, int *status)
{
	if (enable_raw_mode())
		return NULL;

	if (!history_initialized) {
		history_init(NULL); // FIXME: Don't use NULL
		history_initialized = true;
	}

	history_reset_idx();

	struct wordvec *vector = wordvec_new();
	struct charstream stream;
	charstream_init(&stream, stdin);

	printf("%s", prompt);

	while (true) {
		char c = charstream_read(&stream);

		if (c == CHARSTREAM_EOF || c == ENTER)
			break;

		switch (c) {
		case CTRL_C:
			*status = UBBRL_CTRL_C;
			goto early_return;
		case CTRL_D:
			*status = UBBRL_CTRL_D;
			goto early_return;
		case BACKSPACE:
			wordvec_pop(vector);
			reset_line(prompt, vector);
			break;
		default:
			wordvec_append(vector, c);
			write(STDIN_FILENO, &c, 1);
			break;
		}
	}

	disable_raw_mode();

	flush_line();

	char *ret_line = strdup(wordvec_chars(vector));
	history_append(strdup(wordvec_chars(vector)));

	wordvec_del(vector);

	return ret_line;

early_return:
	disable_raw_mode();
	wordvec_del(vector);
	return NULL;
}
