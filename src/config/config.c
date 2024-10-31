#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include "config.h"
#include "../utils/utils.h"
#include "../utils/globals.h"

int config(char *argv[])
{
	char *path_to_buk = check_for_buk_directory();
	if (path_to_buk == NULL)
	{
		return EXIT_FAILURE;
	}

	char *path_to_backup = argv[2];
	if (write_config(path_to_backup) == EXIT_SUCCESS)
	{
		printf("%s: Backup path updated successfully. New path: %s\n", NAME, path_to_backup);
	}

	return EXIT_SUCCESS;
}
