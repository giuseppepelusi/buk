#include "utils.h"
#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include <sys/stat.h>	// For mkdir, stat
#include <errno.h>		// For errno and error codes
#include <unistd.h>     // For getuid() function
#include <pwd.h>        // For getpwuid() and struct passwd
#include <string.h>		// For strrchr
#include "../utils/globals.h"

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

char *get_home_directory()
{
    char *home_dir = getenv("HOME");
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

int write_config(char *path_to_backup)
{
	char *path_to_buk = check_for_buk_directory();
	char *path_to_config = (char*)malloc(1000);
	snprintf(path_to_config, 1000, "%s%s", path_to_buk, CONFIG_PATH);
    FILE *config_file = fopen(path_to_config, "w");
    if (config_file)
    {
        fprintf(config_file, "PATH=%s\n", path_to_backup);
        fclose(config_file);
    }
    else
    {
    	char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "%s: Error opening config file", NAME);
        perror(errorMsg);
        fclose(config_file);
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

    static char path[512];
    while (fscanf(config_file, "PATH=%511s", path) == 1)
    {
        fclose(config_file);
        return path;
    }

    fclose(config_file);
    fprintf(stderr, "%s: PATH not found in config file\n", NAME);
    return NULL;
}

char *check_for_buk_directory()
{
	char *current_working_dir = get_current_working_directory();
	char *buk_dir = (char*)malloc(1000);

	while(current_working_dir[0] != '\0')
	{
		snprintf(buk_dir, 1000,  "%s/%s", current_working_dir, DIR_NAME);

	    struct stat sb;

	    if (stat(buk_dir, &sb) == 0 && S_ISDIR(sb.st_mode))
	    {
	    	return buk_dir;
	    }
	    else
	    {
			char *last_slash = strrchr(current_working_dir, '/');
			if (last_slash)
      		{
        		*last_slash = '\0';
       		}
			else
          	{
           		fprintf(stderr, "%s: Not a %s repository (or any of the parent directories)\n", NAME, NAME);
           		break;
           	}
	    }
	}
    return "";
}
