#include <stdio.h>		// For printf, fprintf
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>		// For strcmp
#include "utils/globals.h"
#include "help/help.h"
#include "init/init.h"
#include "config/config.h"
#include "save/save.h"
// #include "delete/delete.h"
// #include "restore/restore.h"

int main(int argc, char *argv[])
{
	const char *option = argv[1];

	if (argc < 2)
	{
		help();
		return EXIT_FAILURE;
	}
	else if (strcmp(option, "help") == 0)
	{
		help();
	}
	else if (strcmp(option, "--version") == 0)
	{
		printf("%s version %s\n", NAME, VERSION);
	}
	else if (strcmp(option, "init") == 0)
	{
		if (argc > 2)
		{
			help_init();
			return EXIT_FAILURE;
		}
		init();
	}
	else if (strcmp(option, "config") == 0)
	{
		if (argc != 3)
		{
			help_config();
			return EXIT_FAILURE;
		}
		config(argv);
	}
	else if (strcmp(option, "save") == 0)
	{
		if (argc < 3)
		{
			help_save();
			return EXIT_FAILURE;
		}
		save(argc, argv);
	}
	else if (strcmp(option, "delete") == 0)
	{
		if (argc < 3)
		{
			help_delete();
			return EXIT_FAILURE;
		}
		// delete();
	}
	else if (strcmp(option, "restore") == 0)
	{
		if (argc < 3)
		{
			help_restore();
			return EXIT_FAILURE;
		}
		// restore();
	}
	else
	{
		fprintf(stderr, "%s: \'%s\' is not a %s command. See '%s help'.\n", NAME, argv[1], NAME, NAME);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
