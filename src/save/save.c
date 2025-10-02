#include <stdio.h>        // For snprintf
#include <stdlib.h>       // For EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>       // For strcmp, strlen
#include <limits.h>       // For PATH_MAX
#include "save.h"
#include "../utils/utils.h"
#include "../utils/globals.h"

int buk_save(int argc, char *argv[])
{
    char *project_root = get_project_root();
    if (project_root == NULL)
    {
        return EXIT_FAILURE;
    }

    char *config_path = read_config_path();
    if (config_path == NULL)
    {
        return EXIT_FAILURE;
    }

    char *dir_name = extract_dir_name(project_root);
    if (dir_name == NULL)
    {
        return EXIT_FAILURE;
    }

    char output_zip[PATH_MAX];
    snprintf(output_zip, PATH_MAX, "%s/%s.zip", config_path, dir_name);

    char temp_backup_dir[PATH_MAX];
    snprintf(temp_backup_dir, PATH_MAX, "%s/%s/backup", project_root, DIR_NAME);

    if (check_if_is_file(output_zip) == EXIT_SUCCESS)
    {
    	unzip_directory(output_zip, temp_backup_dir);
    }
    else
    {
    	if (mkdir_p(temp_backup_dir, 0755) == EXIT_FAILURE)
    	{
    	    return EXIT_FAILURE;
    	}
    }

    int valid_paths_found = 0;
    for (int i = 2; i < argc; i++)
    {
        if (process_path(argv[i], temp_backup_dir, project_root, &valid_paths_found) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
    }

    if (valid_paths_found)
    {
    	if (mkdir_p(config_path, 0755) == EXIT_FAILURE)
    	{
    	    return EXIT_FAILURE;
    	}

    	zip_directory(temp_backup_dir, output_zip);
    }

    remove_directory(temp_backup_dir);

    return EXIT_SUCCESS;
}

static int process_path(const char *path, const char *temp_backup_dir, const char *project_root, int *valid_paths_found)
{
    char absolute_path[PATH_MAX];
    realpath(path, absolute_path);

    if (strncmp(absolute_path, project_root, strlen(project_root)) != 0)
    {
    	printf("%s: \"%s\" is outside repository at \"%s\"\n", NAME, path, project_root);
     	return EXIT_SUCCESS;
    }

    char buk_path[PATH_MAX];
    snprintf(buk_path, PATH_MAX, "%s/%s", project_root, DIR_NAME);
    if (strncmp(absolute_path, buk_path, strlen(buk_path)) == 0)
    {
        printf("%s: \"%s\" directory or any of its contents cannot be saved\n", NAME, DIR_NAME);
        return EXIT_SUCCESS;
    }

    char destination_path[PATH_MAX * 2];
    char relative_path[PATH_MAX];
    if (strcmp(absolute_path, project_root) == 0)
    {
        snprintf(relative_path, PATH_MAX, "%s", extract_dir_name(project_root));
        snprintf(destination_path, PATH_MAX * 2, "%s", temp_backup_dir);
    }
    else
    {
        snprintf(relative_path, PATH_MAX, "%s", absolute_path + strlen(project_root) + 1);
        snprintf(destination_path, PATH_MAX * 2, "%s/%s", temp_backup_dir, relative_path);
    }

    if (check_if_is_directory(path) == EXIT_SUCCESS)
    {
        if (copy_directory(path, destination_path) == EXIT_SUCCESS)
        {
            printf("%s: %s directory saved successfully\n", NAME, relative_path);
            *valid_paths_found = 1;

            return EXIT_SUCCESS;
        }

        return EXIT_FAILURE;
    }
    else if (check_if_is_file(path) == EXIT_SUCCESS)
    {
        if (copy_file(path, destination_path) == EXIT_SUCCESS)
        {
            printf("%s: %s file saved successfully\n", NAME, relative_path);
            *valid_paths_found = 1;

            return EXIT_SUCCESS;
        }

        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, "%s: \"%s\" is not a valid file or directory\n", NAME, path);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
