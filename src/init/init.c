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

int create_folder()
{
	if (mkdir(DIR_NAME, 0755) == -1)
	{
		if (errno == EEXIST)
		{
			fprintf(stderr, "%s: The folder is already initialized\n", NAME);
			return EXIT_FAILURE;
		}
		else
		{
			char errorMsg[100];
			snprintf(errorMsg, sizeof(errorMsg), "%s: Error creating directory", NAME);
        	perror(errorMsg);
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

char *get_current_working_directory()
{
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "%s: Error getting current working directory", NAME);
        perror(errorMsg);
    }
    return cwd;
}

int write_config(const char *home_dir)
{
    FILE *config_file = fopen(CONFIG_PATH, "w");
    if (config_file)
    {
        fprintf(config_file, "PATH=%s/%s\n", home_dir, DEFAULT_BACKUP_DIR);
        fclose(config_file);
    }
    else
    {
    	char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "%s: Error creating config file", NAME);
        perror(errorMsg);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char *read_config_path()
{
    FILE *config_file = fopen(CONFIG_PATH, "r");
    if (!config_file)
    {
    	char errorMsg[100];
    	snprintf(errorMsg, sizeof(errorMsg), "%s: Error opening config file", NAME);
    	perror(errorMsg);
        return NULL;
    }

    static char path[256];
    while (fscanf(config_file, "PATH=%255s", path) == 1)
    {
        fclose(config_file);
        return path;
    }

    fclose(config_file);
    fprintf(stderr, "%s: Error: PATH not found in config file\n", NAME);
    return NULL;
}
