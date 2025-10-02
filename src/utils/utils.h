#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include "../miniz/miniz.h"

void print_perror(const char *message);
int mkdir_p(const char *directory_path, mode_t permissions);
char *get_home_directory();
char *get_current_working_directory();
int write_config_path(char *path_to_backup);
char *read_config_path();
char *get_project_root();
int check_if_is_directory(const char *path);
int check_if_is_file(const char *path);
char *extract_dir_name(const char *path);
int copy_directory(const char *source, const char *destination);
int copy_file(const char *source_path, const char *destination_path);
int remove_directory(const char *path);
int zip_directory(const char *source_directory, const char *output_zip);
int add_files_to_zip(const char *directory, mz_zip_archive *zip, const char *base_name);
int unzip_directory(const char *zip_file, const char *output_folder);

#endif // UTILS_H
