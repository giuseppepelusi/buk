#include <stdio.h>      // For printf, fprintf, perror, snprintf
#include <stdlib.h>     // For EXIT_SUCCESS, EXIT_FAILURE, malloc, free, getenv
#include <sys/stat.h>   // For mkdir, stat, S_ISDIR, S_ISREG
#include <sys/types.h>  // For mode_t, struct stat
#include <unistd.h>     // For getuid, getcwd, close, read, write, unlink, rmdir
#include <pwd.h>        // For getpwuid, struct passwd
#include <string.h>     // For strcmp, strrchr, strdup, strncpy
#include <limits.h>     // For PATH_MAX
#include "../miniz/miniz.h"  // For miniz zip functions
#include <dirent.h>     // For opendir, readdir, closedir, struct dirent
#include <fcntl.h>      // For O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, open
#include "utils.h"
#include "globals.h"

void print_perror(const char *message)
{
    char errorMsg[100];
    snprintf(errorMsg, sizeof(errorMsg), "%s: %s", NAME, message);
    perror(errorMsg);
}

int mkdir_p(const char *directory_path, mode_t permissions)
{
    char path_buffer[PATH_MAX];

    snprintf(path_buffer, sizeof(path_buffer), "%s", directory_path);

    for (char *current_position = path_buffer + 1; *current_position; current_position++)
    {
        if (*current_position == '/')
        {
            *current_position = '\0';

            if (check_if_is_directory(path_buffer) == EXIT_SUCCESS)
            {
                *current_position = '/';
                continue;
            }

            if (mkdir(path_buffer, permissions) == -1)
            {
                return EXIT_FAILURE;
            }

            *current_position = '/';
        }
    }

    if (check_if_is_directory(path_buffer) == EXIT_FAILURE)
    {
        if (mkdir(path_buffer, permissions) == -1)
        {
            print_perror("Error creating directory");
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
        print_perror("Error getting current working directory");
    }
    return cwd;
}

int write_config_path(char *path_to_backup)
{
	char *project_root = get_project_root();
	if (project_root == NULL)
	{
		return EXIT_FAILURE;
	}

	char path_to_config[PATH_MAX];
	snprintf(path_to_config, PATH_MAX, "%s/%s/%s", project_root, DIR_NAME, CONFIG_FILE);

    FILE *config_file = fopen(path_to_config, "w");
    if (!config_file)
    {
        print_perror("Error opening config file");
        return EXIT_FAILURE;
    }
    fprintf(config_file, "PATH=%s\n", path_to_backup);
    fclose(config_file);

    return EXIT_SUCCESS;
}

char *read_config_path()
{
	char *project_root = get_project_root();
	if (project_root == NULL)
	{
		return NULL;
	}

	char path_to_config[PATH_MAX];
	snprintf(path_to_config, PATH_MAX, "%s/%s/%s", project_root, DIR_NAME, CONFIG_FILE);

    FILE *config_file = fopen(path_to_config, "r");
    if (!config_file)
    {
   		print_perror("Error opening config file");
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

char *get_project_root()
{
	char *current_working_dir = get_current_working_directory();
	char *buk_dir = malloc(PATH_MAX);

	while(current_working_dir[0] != '\0')
	{
		snprintf(buk_dir, PATH_MAX,  "%s/%s", current_working_dir, DIR_NAME);

		struct stat buffer;

	    if (check_if_is_directory(buk_dir) == EXIT_SUCCESS)
	    {
			char *last_slash = strrchr(buk_dir, '/');
            if (last_slash)
            {
                *last_slash = '\0';
            }
            free(current_working_dir);
            return buk_dir;
	    }

	    char *last_slash = strrchr(current_working_dir, '/');
		if (last_slash)
    	{
        	*last_slash = '\0';
		}
		else
    	{
        	break;
		}
    }

    fprintf(stderr, "%s: Not a %s repository (or any of the parent directories)\n", NAME, NAME);
    free(buk_dir);
    free(current_working_dir);

    return NULL;
}

int check_if_is_directory(const char *path)
{
    struct stat buffer;
    if (stat(path, &buffer) != 0)
    {
        return EXIT_FAILURE;
    }
    return S_ISDIR(buffer.st_mode) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int check_if_is_file(const char *path)
{
    struct stat buffer;
    if (stat(path, &buffer) != 0)
    {
        return EXIT_FAILURE;
    }
    return S_ISREG(buffer.st_mode) ? EXIT_SUCCESS : EXIT_FAILURE;
}

char *extract_dir_name(const char *path)
{
    const char *last_slash = strrchr(path, '/');
    if (last_slash == NULL)
    {
        return strdup(path);
    }

    const char *dir_name = last_slash + 1;

    return strdup(dir_name);
}

int copy_directory(const char *source, const char *destination)
{
    struct dirent *entry;
    DIR *dp = opendir(source);
    if (dp == NULL)
    {
        fprintf(stderr, "%s: Cannot open directory %s\n", NAME, source);
        return EXIT_FAILURE;
    }

    if (mkdir_p(destination, 0755) == EXIT_FAILURE)
    {
        fprintf(stderr, "%s: Cannot create directory %s\n", NAME, destination);
        closedir(dp);
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dp)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if (strcmp(entry->d_name, DIR_NAME) == 0)
        {
            continue;
        }

        char full_source_path[PATH_MAX];
        snprintf(full_source_path, sizeof(full_source_path), "%s/%s", source, entry->d_name);

        char full_dest_path[PATH_MAX];
        snprintf(full_dest_path, sizeof(full_dest_path), "%s/%s", destination, entry->d_name);

        if (check_if_is_directory(full_source_path) == EXIT_SUCCESS)
        {
            if (copy_directory(full_source_path, full_dest_path) != EXIT_SUCCESS)
            {
            	fprintf(stderr, "%s: Failed to copy directory %s\n", NAME, full_source_path);
            }
        }
        else if (check_if_is_file(full_source_path) == EXIT_SUCCESS)
        {
            if (copy_file(full_source_path, full_dest_path) != EXIT_SUCCESS)
            {
                fprintf(stderr, "%s: Failed to copy file %s\n", NAME, full_source_path);
            }
        }
    }

    closedir(dp);

    return EXIT_SUCCESS;
}

int copy_file(const char *source_path, const char *destination_path)
{
	char destination_dir[PATH_MAX];
    strncpy(destination_dir, destination_path, PATH_MAX);
    char *last_slash = strrchr(destination_dir, '/');
    if (last_slash != NULL)
    {
        *last_slash = '\0';

        if (mkdir_p(destination_dir, 0755) == EXIT_FAILURE)
        {
            fprintf(stderr, "%s: Cannot create directory %s\n", NAME, destination_dir);
            return EXIT_FAILURE;
        }
    }

    int source_fd, dest_fd;
    char buffer[BUFSIZ];
    ssize_t bytes_read, bytes_written;

    source_fd = open(source_path, O_RDONLY);
    if (source_fd < 0)
    {
        print_perror("Error opening source file");
        return EXIT_FAILURE;
    }

    dest_fd = open(destination_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0)
    {
        print_perror("Error opening destination file");
        close(source_fd);
        return EXIT_FAILURE;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            print_perror("Error writing to destination file");
            close(source_fd);
            close(dest_fd);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read < 0)
    {
        print_perror("Error reading from source file");
        close(source_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }

    close(source_fd);
    close(dest_fd);

    return EXIT_SUCCESS;
}

int remove_directory(const char *path)
{
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (dp == NULL)
    {
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dp)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat path_stat;
        if (stat(full_path, &path_stat) == -1)
        {
            closedir(dp);
            return EXIT_FAILURE;
        }

        if (S_ISDIR(path_stat.st_mode))
        {
            if (remove_directory(full_path) == EXIT_FAILURE)
            {
                closedir(dp);
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (unlink(full_path) == -1)
            {
                closedir(dp);
                return EXIT_FAILURE;
            }
        }
    }

    closedir(dp);
    return rmdir(path) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int zip_directory(const char *source_directory, const char *output_zip)
{
    mz_zip_archive zip;
    mz_zip_zero_struct(&zip);

    if (!mz_zip_writer_init_file(&zip, output_zip, 0))
    {
        fprintf(stderr, "%s: Error opening zip file %s\n", NAME, output_zip);
        return EXIT_FAILURE;
    }

    if (add_files_to_zip(source_directory, &zip, "") == EXIT_FAILURE)
    {
        mz_zip_writer_end(&zip);
        return EXIT_FAILURE;
    }

    if (!mz_zip_writer_finalize_archive(&zip))
    {
        fprintf(stderr, "%s: Error finalizing zip archive %s\n", NAME, output_zip);
        mz_zip_writer_end(&zip);
        return EXIT_FAILURE;
    }

    mz_zip_writer_end(&zip);
    return EXIT_SUCCESS;
}

int add_files_to_zip(const char *directory, mz_zip_archive *zip, const char *base_name)
{
    struct dirent *entry;
    DIR *dp = opendir(directory);
    if (dp == NULL)
    {
        fprintf(stderr, "%s: Cannot open directory %s\n", NAME, directory);
        return EXIT_FAILURE;
    }

    int is_empty = 1;
    while ((entry = readdir(dp)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        is_empty = 0;

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);
        char zip_path[PATH_MAX];

        if (strlen(base_name) > 0)
        {
            snprintf(zip_path, sizeof(zip_path), "%s/%s", base_name, entry->d_name);
        }
        else
        {
            snprintf(zip_path, sizeof(zip_path), "%s", entry->d_name);
        }

        if (entry->d_type == DT_DIR)
        {
            if (add_files_to_zip(full_path, zip, zip_path) == EXIT_FAILURE)
            {
                closedir(dp);
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!mz_zip_writer_add_file(zip, zip_path, full_path, NULL, 0, MZ_DEFAULT_COMPRESSION))
            {
                fprintf(stderr, "%s: Error adding file %s\n", NAME, full_path);
                closedir(dp);
                return EXIT_FAILURE;
            }
        }
    }

    if (is_empty && strlen(base_name) > 0)
    {
        char zip_path[PATH_MAX];
        snprintf(zip_path, sizeof(zip_path), "%s/", base_name);
        if (!mz_zip_writer_add_mem(zip, zip_path, "", 0, MZ_DEFAULT_COMPRESSION))
        {
            fprintf(stderr, "%s: Error adding empty directory %s\n", NAME, directory);
            closedir(dp);
            return EXIT_FAILURE;
        }
    }

    closedir(dp);
    return EXIT_SUCCESS;
}

int unzip_directory(const char *zip_file, const char *output_folder)
{
    mz_zip_archive zip;
    mz_zip_zero_struct(&zip);

    if (!mz_zip_reader_init_file(&zip, zip_file, 0))
    {
        fprintf(stderr, "Cannot open zip archive %s\n", zip_file);
        return EXIT_FAILURE;
    }

    if (mkdir_p(output_folder, 0755) != 0)
    {
        fprintf(stderr, "Cannot create output directory %s\n", output_folder);
        mz_zip_reader_end(&zip);
        return EXIT_FAILURE;
    }

    mz_uint num_entries = mz_zip_reader_get_num_files(&zip);
    for (mz_uint i = 0; i < num_entries; i++)
    {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
        {
            fprintf(stderr, "Cannot get file stat for entry %u\n", i);
            mz_zip_reader_end(&zip);
            return EXIT_FAILURE;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", output_folder, file_stat.m_filename);

        if (file_stat.m_is_directory)
        {
       		mode_t dir_mode = 0755;
            if (file_stat.m_external_attr != 0)
            {
                dir_mode = (file_stat.m_external_attr >> 16) & 0777;
            }

            if (mkdir_p(full_path, 0755) != 0)
            {
                fprintf(stderr, "Cannot create directory %s\n", full_path);
                mz_zip_reader_end(&zip);
                return EXIT_FAILURE;
            }
        }
        else
        {
            char *last_slash = strrchr(full_path, '/');
            if (last_slash != NULL)
            {
                *last_slash = '\0';
                if (mkdir_p(full_path, 0755) != 0)
                {
                    fprintf(stderr, "Cannot create directory %s\n", full_path);
                    mz_zip_reader_end(&zip);
                    return EXIT_FAILURE;
                }
                *last_slash = '/';
            }

            if (!mz_zip_reader_extract_to_file(&zip, i, full_path, 0))
            {
                fprintf(stderr, "Cannot extract file %s\n", file_stat.m_filename);
                mz_zip_reader_end(&zip);
                return EXIT_FAILURE;
            }

            if (file_stat.m_external_attr != 0)
            {
                mode_t mode = (file_stat.m_external_attr >> 16) & 0777;
                if (chmod(full_path, mode) != 0)
                {
                    fprintf(stderr, "%s: Warning: Could not set permissions for %s\n", NAME, full_path);
                }
            }
        }
    }

    mz_zip_reader_end(&zip);
    return EXIT_SUCCESS;
}
