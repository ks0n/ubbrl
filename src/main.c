#include "ubbrl.h"
#include <stdio.h>

int main(void)
{
	const char *line = NULL;
	while ((line = ubbrl_read("\x1B[33mubsh\x1B[0m > "))) {
		printf("%s\n", line);

		free(line);
	}

	return 0;
}
