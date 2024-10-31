#ifndef UTILS_H
#define UTILS_H

int create_folder();
char *get_home_directory();
char *get_current_working_directory();
int write_config(char *path_to_backup);
char *read_config_path();
char *check_for_buk_directory();

#endif // UTILS_H
