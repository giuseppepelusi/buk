#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "help/help.h"
// #include "init/init.h"
// #include "save/save.h"
// #include "delete/delete.h"
// #include "restore/restore.h"

int main(int argc, char *argv[])
{
	const char *option = argv[1];

	if (argc < 2)
	{
		// help();
		return EXIT_FAILURE;
	}
	else if (strcmp(option, "help") == 0)
	{
		// help();
	}
	else if (strcmp(option, "init") == 0)
	{
		// init();
	}
	else if (strcmp(option, "save") == 0)
	{
		// save();
	}
	else if (strcmp(option, "delete") == 0)
	{
		// delete();
	}
	else if (strcmp(option, "restore") == 0)
	{
		// restore();
	}
	else
	{
		fprintf(stderr, "Unknown command: \"%s\", use \"buk help\" for the list of commands.\n", argv[1]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
