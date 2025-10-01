#include <stdio.h>		// For printf, fprintf, perror
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE
#include <limits.h>     // For PATH_MAX
#include <string.h>     // For strlen, strncpy, strstr
#include <unistd.h>     // For access
#include "config.h"
#include "../utils/utils.h"
#include "../utils/globals.h"

int buk_config(char *argv[])
{
	char *project_root = get_project_root();
	if (project_root == NULL)
	{
		return EXIT_FAILURE;
	}

	char *path_to_backup = argv[2];
	char resolved_path[PATH_MAX];

    if (validate_and_resolve_path(path_to_backup, resolved_path) == EXIT_SUCCESS)
    {
        if (write_config_path(resolved_path) == EXIT_SUCCESS)
        {
        	printf("%s: Backup path updated successfully. New path: %s\n", NAME, resolved_path);
            return EXIT_SUCCESS;
        }
        else
        {
            printf("%s: Error writing config file\n", NAME);
            return EXIT_FAILURE;
        }
    }
    else
    {
        return EXIT_FAILURE;
    }
}

int is_path_in_safe_location(const char *path)
{
    char *home_dir = get_home_directory();

    if (home_dir != NULL && strstr(path, home_dir) == path)
    {
        return EXIT_SUCCESS;
    }

    if (strstr(path, "/tmp") == path)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

char* get_first_existing_parent(const char *path)
{
    char *temp_path = malloc(PATH_MAX);
    if (temp_path == NULL)
    {
        return NULL;
    }

    strncpy(temp_path, path, PATH_MAX);
    temp_path[PATH_MAX - 1] = '\0';

    while (strlen(temp_path) > 1)
    {
        if (access(temp_path, F_OK) == 0)
        {
            return temp_path;
        }

        char *last_slash = strrchr(temp_path, '/');
        if (last_slash == NULL || last_slash == temp_path)
        {
            strcpy(temp_path, "/");
            break;
        }

        *last_slash = '\0';
    }

    return temp_path;
}

int validate_and_resolve_path(const char *input_path, char *resolved_path)
{
    char working_path[PATH_MAX];

    if (input_path[0] != '/')
    {
        char *cwd = get_current_working_directory();
        if (cwd == NULL)
        {
            return EXIT_FAILURE;
        }
        snprintf(working_path, PATH_MAX, "%s/%s", cwd, input_path);
        free(cwd);
    }
    else
    {
        strncpy(working_path, input_path, PATH_MAX);
        working_path[PATH_MAX - 1] = '\0';
    }

    if (realpath(working_path, resolved_path) != NULL)
    {
        if (check_if_is_directory(resolved_path) != EXIT_SUCCESS)
        {
            printf("%s: \"%s\" exists but is not a directory. Backup path must be a directory\n", NAME, input_path);
            return EXIT_FAILURE;
        }

        if (is_path_in_safe_location(resolved_path) != EXIT_SUCCESS)
        {
            printf("%s: \"%s\" is not in a safe location. Path must be under your home directory or /tmp\n", NAME, resolved_path);
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    char *parent_path = get_first_existing_parent(working_path);
    if (parent_path == NULL)
    {
        return EXIT_FAILURE;
    }

    char resolved_parent[PATH_MAX];
    if (realpath(parent_path, resolved_parent) == NULL)
    {
        free(parent_path);
        return EXIT_FAILURE;
    }
    free(parent_path);

    if (access(resolved_parent, W_OK) != 0)
    {
        printf("%s: Cannot write to parent directory \"%s\". Permission denied\n", NAME, resolved_parent);
        return EXIT_FAILURE;
    }

    if (is_path_in_safe_location(working_path) != EXIT_SUCCESS)
    {
        printf("%s: \"%s\" is not in a safe location. Path must be under your home directory or /tmp\n", NAME, input_path);
        return EXIT_FAILURE;
    }

    strncpy(resolved_path, working_path, PATH_MAX);
    resolved_path[PATH_MAX - 1] = '\0';

    return EXIT_SUCCESS;
}
