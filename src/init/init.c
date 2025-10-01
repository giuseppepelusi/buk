#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include <limits.h>		// For PATH_MAX
#include <sys/stat.h>	// For mkdir
#include "init.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

int buk_init()
{
	char *home_dir = get_home_directory();
	if (home_dir == NULL)
    {
        return EXIT_FAILURE;
    }

	char *current_working_dir = get_current_working_directory();
	if (current_working_dir == NULL)
    {
        return EXIT_FAILURE;
    }

	char default_backup_path[PATH_MAX];
	snprintf(default_backup_path, PATH_MAX , "%s/%s", home_dir, DEFAULT_BACKUP_DIR);

	if (check_if_is_directory(DIR_NAME) == EXIT_SUCCESS)
	{
		fprintf(stderr, "%s: The directory is already initialized\n", NAME);
  		free(current_working_dir);
		return EXIT_FAILURE;
	}
	else if (mkdir_p(DIR_NAME, 0755) == EXIT_FAILURE)
	{
		print_perror("Error creating directory");
  		free(current_working_dir);
		return EXIT_FAILURE;
	}

	write_config_path(default_backup_path);
	printf("%s: Initialized %s repository in %s/.%s/\n", NAME, NAME, current_working_dir, NAME);
	free(current_working_dir);

	return EXIT_SUCCESS;
}
