#include "history.h"
#include "ubbrl.h"
#include <stdio.h>

int main(void)
{
	int status = UBBRL_OK;
	char *line = NULL;

	history_init(NULL);

	while (1) {
		line = ubbrl_read("\x1B[33mubsh\x1B[0m > ", &status);

		if (status == UBBRL_OK)
			printf("%s\n", line);

		switch (status) {
		case UBBRL_CTRL_C:
			printf("\nCTRL-C\n");
			break;
		case UBBRL_CTRL_D:
			printf("\nCTRL-D\n");
			return 0;
		default:
			break;
		}

		free(line);
	}

	return 0;
}
