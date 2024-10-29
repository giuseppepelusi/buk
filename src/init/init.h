#ifndef INIT_H
#define INIT_H

int init();
int create_folder();
const char *get_home_directory();
int write_config(const char *home_dir);
char *read_config_path();

#endif // INIT_H
