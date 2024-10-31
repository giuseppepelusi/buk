#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include "init.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

int init()
{
	char *home_dir = get_home_directory();
	char *current_working_dir = get_current_working_directory();
	char *default_backup_path = (char*)malloc(1000);
	snprintf(default_backup_path, 1000 , "%s/%s", home_dir, DEFAULT_BACKUP_DIR);

	if (create_folder() == EXIT_SUCCESS)
	{
		write_config(default_backup_path);
		printf("%s: Initialized %s repository in %s/.%s/\n", NAME, NAME, current_working_dir, NAME);
		free(current_working_dir);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
