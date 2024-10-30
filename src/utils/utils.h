#ifndef UTILS_H
#define UTILS_H

int create_folder();
const char *get_home_directory();
char *get_current_working_directory();
int write_config(const char *home_dir);
char *read_config_path();

#endif // UTILS_H
