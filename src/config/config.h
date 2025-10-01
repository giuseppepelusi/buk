#ifndef CONFIG_H
#define CONFIG_H

int buk_config(char *argv[]);
int is_path_in_safe_location(const char *path);
char* get_first_existing_parent(const char *path);
int validate_and_resolve_path(const char *input_path, char *resolved_path);

#endif // CONFIG_H
