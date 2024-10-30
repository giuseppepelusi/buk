#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include <sys/stat.h>	// For mkdir
#include <errno.h>		// For errno and error codes
#include <unistd.h>     // For getuid() function
#include <pwd.h>        // For getpwuid() and struct passwd
#include "init.h"
#include "../globals.h"

int init()
{
	const char *home_dir = get_home_directory();

	if (create_folder() == EXIT_SUCCESS)
	{
		write_config(home_dir);
	}

	return EXIT_SUCCESS;
}

int create_folder()
{
	if (mkdir(DIR_NAME, 0755) == -1)
	{
		if (errno == EEXIST)
		{
			fprintf(stderr, "The folder is already initialized\n");
			return EXIT_FAILURE;
		}
		else
		{
			perror("Error creating directory");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

const char *get_home_directory()
{
    const char *home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        struct passwd *pw = getpwuid(getuid());
        home_dir = pw ? pw->pw_dir : NULL;
    }
    return home_dir;
}

int write_config(const char *home_dir)
{
    FILE *config_file = fopen(CONFIG_PATH, "w");
    if (config_file)
    {
        fprintf(config_file, "PATH=%s/.buk_backups\n", home_dir);
        fclose(config_file);
    }
    else
    {
        perror("Error creating config file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char *read_config_path()
{
    FILE *config_file = fopen(CONFIG_PATH, "r");
    if (!config_file)
    {
        perror("Error opening config file");
        return NULL;
    }

    static char path[256];
    while (fscanf(config_file, "PATH=%255s", path) == 1)
    {
        fclose(config_file);
        return path;
    }

    fclose(config_file);
    fprintf(stderr, "Error: PATH not found in config file\n");
    return NULL;
}
