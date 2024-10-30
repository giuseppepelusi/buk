#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include "init.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

int init()
{
	const char *home_dir = get_home_directory();
	char *current_working_dir = get_current_working_directory();

	if (create_folder() == EXIT_SUCCESS)
	{
		write_config(home_dir);
		printf("%s: Initialized buk repository in %s/.%s/\n", NAME, current_working_dir, NAME);
		free(current_working_dir);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
